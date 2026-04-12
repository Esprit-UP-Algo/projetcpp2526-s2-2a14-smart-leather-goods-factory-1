#include "pagechat.h"
#include "ui_pagechat.h"
#include "pageemployee.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QListWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QColor>

pagechat::pagechat(int idEmployeConnecte, pageemployee *pageEmp, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pagechat)
    , m_idEmploye(idEmployeConnecte)
    , m_timer(new QTimer(this))
    , m_pageEmployee(pageEmp)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);

    qDebug() << "pagechat opened, idEmploye =" << m_idEmploye;
    qDebug() << "pagechat parent =" << parent;
    qDebug() << "pageemployee ptr =" << m_pageEmployee;

    loadMessages();

    connect(m_timer, &QTimer::timeout, this, &pagechat::loadMessages);
    m_timer->start(3000);
}

pagechat::~pagechat()
{
    delete ui;
}

void pagechat::loadMessages()
{
    ui->listMessages->clear();

    QSqlQuery query;
    query.prepare(
        "SELECT m.ID_EMPLOYE, e.NOM, e.PRENOM, m.CONTENU, "
        "TO_CHAR(m.DATE_ENVOI, 'DD/MM/YYYY HH24:MI') "
        "FROM SMARTLEATHER.MESSAGE m "
        "JOIN SMARTLEATHER.EMPLOYE e ON e.ID_EMPLOYE = m.ID_EMPLOYE "
        "ORDER BY m.DATE_ENVOI"
        );

    if (!query.exec()) {
        qDebug() << "Erreur loadMessages =" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int idEmp = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString contenu = query.value(3).toString();
        QString date = query.value(4).toString();

        QString ligne = QString("[%1] %2 %3 : %4")
                            .arg(date)
                            .arg(nom)
                            .arg(prenom)
                            .arg(contenu);

        QListWidgetItem *item = new QListWidgetItem(ligne);

        if (idEmp == m_idEmploye) {
            item->setBackground(QColor("#d9f2d9"));
        } else {
            item->setBackground(QColor("#f5f5f5"));
        }

        ui->listMessages->addItem(item);
    }

    ui->listMessages->scrollToBottom();
}

void pagechat::on_btnSend_clicked()
{
    QString msg = ui->inputMessage->text().trimmed();

    if (msg.isEmpty())
        return;

    qDebug() << "sending message with idEmploye =" << m_idEmploye;

    QSqlQuery query;
    query.prepare(
        "INSERT INTO SMARTLEATHER.MESSAGE (ID_EMPLOYE, CONTENU, DATE_ENVOI) "
        "VALUES (:idEmploye, :contenu, SYSDATE)"
        );
    query.bindValue(":idEmploye", m_idEmploye);
    query.bindValue(":contenu", msg);

    if (!query.exec()) {
        qDebug() << "Erreur sendMessage =" << query.lastError().text();
        QMessageBox::warning(this, "Erreur",
                             "Impossible d'envoyer le message.\n" + query.lastError().text());
        return;
    }

    ui->inputMessage->clear();
    loadMessages();
}

void pagechat::on_pushButton_5_clicked()
{
    if (m_pageEmployee) {
        m_pageEmployee->show();
    }
    close();
}
