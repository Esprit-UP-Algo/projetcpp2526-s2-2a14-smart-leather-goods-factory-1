#include "twilio_sms.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QSettings>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

static QString envOrSettings(const char *envName, const QString &settingsKey)
{
    const QByteArray v = qgetenv(envName);
    if (!v.isEmpty())
        return QString::fromUtf8(v);
    QSettings s(QStringLiteral("SmartLeather"), QStringLiteral("ERP"));
    return s.value(settingsKey).toString().trimmed();
}

TwilioSms::TwilioSms(QObject *parent) : QObject(parent), m_nam(new QNetworkAccessManager(this))
{
    loadCredentials();
}

TwilioSms::~TwilioSms() = default;

void TwilioSms::loadCredentials()
{
    // Trial project defaults (rotate credentials if this repo is shared)
    m_accountSid = QStringLiteral("VOTRE_ACCOUNT_SID_ICI");
    m_authToken = QStringLiteral("VOTRE_AUTH_TOKEN_ICI");
    // Default "From" (E.164). Override with env TWILIO_FROM_NUMBER or QSettings twilio/from_number.
    m_fromNumber = envOrSettings("TWILIO_FROM_NUMBER", QStringLiteral("twilio/from_number"));
    if (m_fromNumber.isEmpty())
        m_fromNumber = QStringLiteral("+17179288985");
}

bool TwilioSms::isConfigured() const
{
    return !m_accountSid.isEmpty() && !m_authToken.isEmpty() && !m_fromNumber.isEmpty();
}

/** Ensures Twilio From/To use E.164 with leading '+'. */
static QString ensureLeadingPlus(const QString &e164ish)
{
    QString d;
    for (QChar c : e164ish) {
        if (c.isDigit())
            d.append(c);
    }
    if (d.isEmpty())
        return e164ish.trimmed().startsWith(QLatin1Char('+')) ? e164ish.trimmed() : QString();
    return QLatin1Char('+') + d;
}

QString TwilioSms::toE164(const QString &raw)
{
    QString s = raw.trimmed();
    s.remove(' ');
    s.remove('-');
    if (s.isEmpty())
        return {};

    if (s.startsWith(QLatin1Char('+'))) {
        QString digits;
        for (QChar c : s.mid(1)) {
            if (c.isDigit())
                digits.append(c);
        }
        if (digits.length() < 8)
            return {};
        return QLatin1Char('+') + digits;
    }

    QString digits;
    for (QChar c : s) {
        if (c.isDigit())
            digits.append(c);
    }
    if (digits.isEmpty())
        return {};

    // UK mobile written as 07… (11 digits) → +44…
    if (digits.length() == 11 && digits.startsWith(QStringLiteral("07")))
        return QStringLiteral("+44") + digits.mid(1);

    // Tunisia mobile: 8 digits → +216
    if (digits.length() == 8)
        return QStringLiteral("+216") + digits;

    if (digits.startsWith(QStringLiteral("216")) && digits.length() >= 11)
        return QLatin1Char('+') + digits;

    if (digits.startsWith(QStringLiteral("00")))
        return QLatin1Char('+') + digits.mid(2);

    return QLatin1Char('+') + digits;
}

void TwilioSms::sendOrderStatusSms(const QString &toRaw,
                                   const QString &clientName,
                                   const QString &refCommande,
                                   const QString &newStatus,
                                   std::function<void(bool ok, const QString &detail)> onFinished)
{
    loadCredentials();
    if (!isConfigured()) {
        if (onFinished)
            onFinished(false, QStringLiteral("Twilio non configuré (variables d'environnement ou QSettings)."));
        return;
    }

    const QString to = toE164(toRaw);
    if (to.isEmpty()) {
        if (onFinished)
            onFinished(false, QStringLiteral("Numéro de téléphone invalide."));
        return;
    }

    const QString body = QStringLiteral("SmartLeather: Bonjour %1, votre commande %2 est maintenant : %3.")
                             .arg(clientName.isEmpty() ? QStringLiteral("client") : clientName,
                                  refCommande.isEmpty() ? QStringLiteral("(sans ref)") : refCommande,
                                  newStatus);

    const QString from = ensureLeadingPlus(m_fromNumber);
    if (from.size() < 4) {
        if (onFinished)
            onFinished(false, QStringLiteral("Numéro Twilio 'From' invalide (E.164, ex. +17179288985)."));
        return;
    }

    const QUrl url(QStringLiteral("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(m_accountSid));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    const QByteArray auth = (m_accountSid + QLatin1Char(':') + m_authToken).toUtf8().toBase64();
    req.setRawHeader("Authorization", "Basic " + auth);

    QUrlQuery q;
    q.addQueryItem(QStringLiteral("To"), to);
    q.addQueryItem(QStringLiteral("From"), from);
    q.addQueryItem(QStringLiteral("Body"), body);
    const QByteArray postData = q.query(QUrl::FullyEncoded).toUtf8();

    QNetworkReply *reply = m_nam->post(req, postData);
    connect(reply, &QNetworkReply::finished, this, [reply, onFinished]() {
        const QByteArray resp = reply->readAll();
        const bool ok = (reply->error() == QNetworkReply::NoError);

        QString detail;
        if (ok) {
            detail = QStringLiteral("SMS envoyé avec succès.");
        } else {
            // Parse la réponse JSON Twilio pour un message clair
            const QJsonDocument doc = QJsonDocument::fromJson(resp);
            if (!doc.isNull() && doc.isObject()) {
                const QJsonObject obj = doc.object();
                const int    code = obj.value("code").toInt();
                const QString msg = obj.value("message").toString();

                switch (code) {
                case 21408:
                    detail = "Permissions géographiques non activées pour la Tunisie (+216).\n\n"
                             "Solution :\n"
                             "1. Allez sur console.twilio.com\n"
                             "2. Messaging → Settings → Geo Permissions\n"
                             "3. Activez \"Tunisia (TN)\"\n\n"
                             "Sur compte Trial : vérifiez aussi le numéro dans\n"
                             "Phone Numbers → Verified Caller IDs.";
                    break;
                case 21211:
                    detail = "Numéro de téléphone invalide.\n"
                             "Format attendu : +216XXXXXXXX (11 chiffres).";
                    break;
                case 20003:
                    detail = "Identifiants Twilio incorrects.\n"
                             "Vérifiez AccountSID et AuthToken dans twilio_sms.cpp.";
                    break;
                case 21608:
                    detail = "Compte Trial : vous ne pouvez envoyer des SMS\n"
                             "qu'aux numéros vérifiés dans Verified Caller IDs.";
                    break;
                case 21614:
                    detail = "Le numéro \"To\" n'est pas valide pour SMS.\n"
                             "Vérifiez le format E.164 (+216XXXXXXXX).";
                    break;
                default:
                    detail = QString("Erreur Twilio %1 : %2").arg(code).arg(msg);
                    break;
                }
            } else {
                // Pas de JSON → erreur réseau
                detail = "Erreur réseau : " + reply->errorString() +
                         "\nVérifiez votre connexion internet.";
            }
        }

        reply->deleteLater();
        if (onFinished)
            onFinished(ok, detail);
        else if (!ok)
            qDebug() << "Twilio SMS error:" << detail;
    });
}
