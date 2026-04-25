#include "smtp.h"

Smtp::Smtp(const QString &user, const QString &pass, const QString &host, int port)
    : m_user(user), m_pass(pass), m_host(host), m_port(port)
{
    socket = new QSslSocket(this);

    connect(socket, &QSslSocket::readyRead, this, &Smtp::readyRead);
    connect(socket, &QSslSocket::connected, this, &Smtp::connected);
    connect(socket, &QSslSocket::stateChanged, this, &Smtp::stateChanged);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::errorOccurred), this, &Smtp::errorReceived);
    connect(socket, &QSslSocket::disconnected, this, &Smtp::disconnected);
    connect(socket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), [this](const QList<QSslError> &errors){
        for(const QSslError &e : errors) qDebug() << "SSL ERROR:" << e.errorString();
        socket->ignoreSslErrors();
    });
}

Smtp::~Smtp()
{
    if (socket->isOpen()) socket->close();
}

void Smtp::sendMail(const QString &from, const QString &to, const QString &subject, const QString &body)
{
    m_from = from.trimmed();
    m_to = to.trimmed();
    m_subject = subject;
    m_body = body;
    m_user = m_user.trimmed();
    m_pass = m_pass.replace(" ", "").trimmed(); // Remove internal spaces for Gmail

    state = Init;
    socket->setProtocol(QSsl::TlsV1_2OrLater);
    socket->connectToHostEncrypted(m_host, m_port);

    if (!socket->waitForConnected(10000)) {
        qDebug() << "SMTP: Connection timeout:" << socket->errorString();
        emit error("Délai de connexion dépassé : " + socket->errorString());
    }
}

void Smtp::readyRead()
{
    while (socket->canReadLine()) {
        QString line = socket->readLine();
        qDebug() << "SMTP IN [" << state << "]:" << line.trimmed();

        // 1. Check for SMTP errors (4xx or 5xx)
        if (line.at(0) == '4' || line.at(0) == '5') {
            qDebug() << "SMTP ERROR DETECTED:" << line;
            emit error("Erreur Gmail : " + line.trimmed());
            state = Quit;
            socket->write("QUIT\r\n");
            return;
        }

        // 2. Handle Multiline responses (250-...)
        if (line.length() >= 4 && line.at(3) == '-') {
            continue; // Wait for the final line (e.g., 250 )
        }

        // 3. Command Sequencer
        if (state == Init && line.startsWith("220")) {
            socket->write("EHLO localhost\r\n");
            state = HandShake;
        } else if (state == HandShake && line.startsWith("250")) {
            socket->write("AUTH LOGIN\r\n");
            state = Auth;
        } else if (state == Auth && line.startsWith("334")) {
            socket->write(QByteArray().append(m_user.toUtf8()).toBase64() + "\r\n");
            state = User;
        } else if (state == User && line.startsWith("334")) {
            socket->write(QByteArray().append(m_pass.toUtf8()).toBase64() + "\r\n");
            state = Pass;
        } else if (state == Pass && line.startsWith("235")) {
            socket->write("MAIL FROM:<" + m_from.toUtf8() + ">\r\n");
            state = Mail;
        } else if (state == Mail && line.startsWith("250")) {
            socket->write("RCPT TO:<" + m_to.toUtf8() + ">\r\n");
            state = Rcpt;
        } else if (state == Rcpt && line.startsWith("250")) {
            socket->write("DATA\r\n");
            state = Data;
        } else if (state == Data && line.startsWith("354")) {
            socket->write("To: " + m_to.toUtf8() + "\r\n");
            socket->write("From: " + m_from.toUtf8() + "\r\n");
            socket->write("Subject: " + m_subject.toUtf8() + "\r\n");
            socket->write("\r\n");
            socket->write(m_body.toUtf8() + "\r\n");
            socket->write(".\r\n");
            state = Body;
        } else if (state == Body && line.startsWith("250")) {
            socket->write("QUIT\r\n");
            state = Quit;
            emit status("Email envoyé avec succès !");
        } else if (state == Quit) {
            socket->close();
            this->deleteLater();
        }
    }
}

void Smtp::connected()
{
    qDebug() << "SMTP: SSL Connection established.";
}

void Smtp::disconnected() { qDebug() << "SMTP: Socket disconnected."; }

void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "SMTP Socket State:" << socketState;
}

void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    // Ignore "Remote host closed connection" if we were already done
    if (state == Quit || socketError == QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "SMTP: Connection closed (normal or ignored).";
        return;
    }
    
    qDebug() << "SMTP Internal Error:" << socketError << socket->errorString();
    emit error("Erreur Réseau : " + socket->errorString());
    this->deleteLater();
}
