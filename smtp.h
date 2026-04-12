#ifndef SMTP_H
#define SMTP_H

#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

class Smtp : public QObject
{
    Q_OBJECT

public:
    Smtp(const QString &user, const QString &pass, const QString &host = "smtp.gmail.com", int port = 465);
    ~Smtp();

    void sendMail(const QString &from, const QString &to, const QString &subject, const QString &body);

signals:
    void status(const QString &);
    void error(const QString &);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

private:
    QString m_user;
    QString m_pass;
    QString m_host;
    int m_port;

    QSslSocket *socket;
    QTextStream *t;
    
    QString m_from;
    QString m_to;
    QString m_subject;
    QString m_body;

    enum states {
        Tls, HandShake, Auth, User, Pass, Rcpt, Mail, Data, Init, Body, Quit, Close
    };

    int state;
};

#endif // SMTP_H
