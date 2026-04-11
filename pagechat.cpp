#include "pagechat.h"
#include "ui_pagechat.h"
#include "pageemployee.h"

#include <QtSql>
#include <QListWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QColor>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>

namespace {

QString displayNameFromEmployeId(int idEmploye)
{
    QSqlQuery q;
    q.prepare("SELECT NOM, PRENOM FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", idEmploye);

    if (!q.exec() || !q.next())
        return QString();

    const QString nom = q.value(0).toString().trimmed();
    const QString prenom = q.value(1).toString().trimmed();
    return (prenom + QLatin1Char(' ') + nom).trimmed();
}

} // namespace

pagechat::pagechat(int idEmployeConnecte, const QString &displayName, pageemployee *pageEmp, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pagechat)
    , m_idEmploye(idEmployeConnecte)
    , m_displayName(displayName.trimmed().isEmpty()
                        ? (QStringLiteral("Employe ") + QString::number(idEmployeConnecte))
                        : displayName.trimmed())
    , m_timer(new QTimer(this))
    , m_pageEmployee(pageEmp)
    , m_network(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);

    qDebug() << "pagechat opened, idEmploye =" << m_idEmploye;

    ensureUserExists();

    connect(m_timer, &QTimer::timeout, this, &pagechat::loadMessages);
    m_timer->start(3000);
}

pagechat::~pagechat()
{
    delete ui;
}

void pagechat::ensureUserExists()
{
    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/users");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QJsonObject body;
    body["user_id"] = QString::number(m_idEmploye);
    body["nickname"] = m_displayName;
    body["profile_url"] = "https://via.placeholder.com/150";

    QNetworkReply *reply = m_network->post(request, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleCreateUserReply(reply);
    });
}

void pagechat::handleCreateUserReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString bodyText = QString::fromUtf8(data);

    qDebug() << "CREATE USER status =" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "CREATE USER error =" << reply->errorString();
    qDebug() << "CREATE USER body =" << bodyText;

    if (reply->error() == QNetworkReply::NoError) {
        m_userReady = true;
        syncSendbirdNicknameThenContinue();
        reply->deleteLater();
        return;
    }

    // Sendbird renvoie 400202 + "unique constraint" si le user_id existe déjà (pas "already exists")
    bool userAlreadyInSendbird = false;
    {
        QJsonDocument errDoc = QJsonDocument::fromJson(data);
        if (errDoc.isObject()) {
            const QJsonObject o = errDoc.object();
            const int code = o["code"].toInt();
            const QString msg = o["message"].toString();
            if (code == 400202 || msg.contains(QStringLiteral("unique constraint"), Qt::CaseInsensitive))
                userAlreadyInSendbird = true;
        }
    }

    if (userAlreadyInSendbird ||
        bodyText.contains(QStringLiteral("already exists"), Qt::CaseInsensitive) ||
        bodyText.contains(QStringLiteral("duplicate"), Qt::CaseInsensitive) ||
        bodyText.contains(QStringLiteral("unique constraint"), Qt::CaseInsensitive)) {
        m_userReady = true;
        syncSendbirdNicknameThenContinue();
        reply->deleteLater();
        return;
    }

    QMessageBox::warning(this,
                         "Erreur",
                         "Impossible de créer le user Sendbird.\n\n" +
                             reply->errorString() + "\n\n" + bodyText);
    reply->deleteLater();
}

void pagechat::syncSendbirdNicknameThenContinue()
{
    if (!m_userReady)
        return;

    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/users/" + QString::number(m_idEmploye));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QJsonObject body;
    body["nickname"] = m_displayName;
    body["profile_url"] = QStringLiteral("https://via.placeholder.com/150");

    QNetworkReply *reply = m_network->put(request, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray putBody = reply->readAll();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "UPDATE Sendbird nickname error =" << reply->errorString()
            << QString::fromUtf8(putBody);
        }
        ensureChannelExists();
        reply->deleteLater();
    });
}

void pagechat::ensureChannelExists()
{
    if (!m_userReady)
        return;

    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/group_channels");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QJsonObject body;
    body["channel_url"] = m_channelUrl;
    body["name"] = "Internal Chat";
    body["is_distinct"] = false;

    QJsonArray userIds;
    userIds.append(QString::number(m_idEmploye));
    body["user_ids"] = userIds;

    QNetworkReply *reply = m_network->post(request, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleCreateChannelReply(reply);
    });
}

void pagechat::handleCreateChannelReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString bodyText = QString::fromUtf8(data);

    qDebug() << "CREATE CHANNEL status =" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "CREATE CHANNEL error =" << reply->errorString();
    qDebug() << "CREATE CHANNEL body =" << bodyText;

    if (reply->error() == QNetworkReply::NoError) {
        m_channelReady = true;
        ensureUserInChannel();
        reply->deleteLater();
        return;
    }

    if (bodyText.contains("already exists", Qt::CaseInsensitive) ||
        bodyText.contains("channel_url", Qt::CaseInsensitive) ||
        bodyText.contains("distinct channel already exists", Qt::CaseInsensitive)) {
        m_channelReady = true;
        ensureUserInChannel();
        reply->deleteLater();
        return;
    }

    QMessageBox::warning(this,
                         "Erreur",
                         "Impossible de créer le channel.\n\n" +
                             reply->errorString() + "\n\n" + bodyText);
    reply->deleteLater();
}

void pagechat::ensureUserInChannel()
{
    if (!m_channelReady)
        return;

    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/group_channels/" + m_channelUrl + "/invite");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QJsonObject body;
    QJsonArray userIds;
    userIds.append(QString::number(m_idEmploye));
    body["user_ids"] = userIds;

    QNetworkReply *reply = m_network->post(request, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleJoinChannelReply(reply);
    });
}

void pagechat::handleJoinChannelReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString bodyText = QString::fromUtf8(data);

    qDebug() << "JOIN CHANNEL status =" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "JOIN CHANNEL error =" << reply->errorString();
    qDebug() << "JOIN CHANNEL body =" << bodyText;

    if (reply->error() == QNetworkReply::NoError) {
        m_membershipReady = true;
        loadMessages();
        reply->deleteLater();
        return;
    }

    // إذا user déjà membre نخليوها ok
    if (bodyText.contains("already", Qt::CaseInsensitive) ||
        bodyText.contains("member", Qt::CaseInsensitive)) {
        m_membershipReady = true;
        loadMessages();
        reply->deleteLater();
        return;
    }

    QMessageBox::warning(this,
                         "Erreur",
                         "Impossible d'ajouter l'utilisateur au groupe.\n\n" +
                             reply->errorString() + "\n\n" + bodyText);
    reply->deleteLater();
}

void pagechat::loadMessages()
{
    if (!m_channelReady || !m_membershipReady)
        return;

    ui->listMessages->clear();

    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/group_channels/" + m_channelUrl + "/messages");

    QUrlQuery query;
    query.addQueryItem("message_ts", QString::number(QDateTime::currentMSecsSinceEpoch()));
    query.addQueryItem("prev_limit", "30");
    query.addQueryItem("next_limit", "0");
    query.addQueryItem("include", "true");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QNetworkReply *reply = m_network->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleLoadMessagesReply(reply);
    });
}

void pagechat::handleLoadMessagesReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    qDebug() << "LOAD status =" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "LOAD error =" << reply->errorString();
    qDebug() << "LOAD body =" << data;

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        reply->deleteLater();
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray messages = root["messages"].toArray();

    for (const QJsonValue &val : messages) {
        QJsonObject msg = val.toObject();

        QString texte = msg["message"].toString();
        QJsonObject user = msg["user"].toObject();
        QString senderId = user["user_id"].toString();
        QString senderName = user["nickname"].toString();

        bool idOk = false;
        const int senderIdInt = senderId.toInt(&idOk);
        if (idOk) {
            const QString fromDb = displayNameFromEmployeId(senderIdInt);
            if (!fromDb.isEmpty())
                senderName = fromDb;
        }

        qint64 time = static_cast<qint64>(msg["created_at"].toDouble());
        QString date = QDateTime::fromMSecsSinceEpoch(time).toString("dd/MM HH:mm");

        QString ligne = "[" + date + "] " + senderName + " : " + texte;
        QListWidgetItem *item = new QListWidgetItem(ligne);

        if (senderId == QString::number(m_idEmploye)) {
            item->setBackground(QColor("#d9f2d9"));
        } else {
            item->setBackground(QColor("#f5f5f5"));
        }

        ui->listMessages->addItem(item);
    }

    ui->listMessages->scrollToBottom();
    reply->deleteLater();
}

void pagechat::on_btnSend_clicked()
{
    if (!m_userReady || !m_channelReady || !m_membershipReady) {
        QMessageBox::warning(this, "Erreur", "Le groupe n'est pas encore prêt.");
        return;
    }

    QString msg = ui->inputMessage->text().trimmed();

    if (msg.isEmpty())
        return;

    QUrl url("https://api-" + m_appId + ".sendbird.com/v3/group_channels/" + m_channelUrl + "/messages");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Api-Token", m_apiToken.toUtf8());

    QJsonObject body;
    body["message_type"] = "MESG";
    body["user_id"] = QString::number(m_idEmploye);
    body["message"] = msg;

    QByteArray payload = QJsonDocument(body).toJson();

    qDebug() << "SEND payload =" << payload;

    QNetworkReply *reply = m_network->post(request, payload);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleSendMessageReply(reply);
    });
}

void pagechat::handleSendMessageReply(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();

    qDebug() << "SEND status =" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "SEND error =" << reply->errorString();
    qDebug() << "SEND body =" << data;

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this,
                             "Erreur",
                             "Impossible d'envoyer le message.\n\n" +
                                 reply->errorString() + "\n\n" +
                                 QString::fromUtf8(data));
        reply->deleteLater();
        return;
    }

    ui->inputMessage->clear();
    loadMessages();
    reply->deleteLater();
}

void pagechat::on_pushButton_5_clicked()
{
    if (m_pageEmployee) {
        m_pageEmployee->show();
    }
    close();
}
