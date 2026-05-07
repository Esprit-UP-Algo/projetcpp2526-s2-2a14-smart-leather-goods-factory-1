#ifndef SIMPLEPAYSERVER_H
#define SIMPLEPAYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

/**
 * Minimal HTTP server — serves one static HTML payment page.
 * Works on any phone connected to the same Wi-Fi as the PC.
 */
class SimplePayServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SimplePayServer(const QString &ref,
                             const QString &amount,
                             const QString &clientName,
                             QObject *parent = nullptr);

    /** Start the server. Returns false if no port is available. */
    bool startListening();

    /** URL principale encodée dans le QR : http://IP_WIFI:PORT */
    QString url() const;

    /** Toutes les IPs locales disponibles (pour affichage dans le dialog) */
    static QStringList allLocalIps();

private slots:
    void handleConnection();

private:
    QString m_ref, m_amount, m_clientName;
    QString buildPage() const;
    static QString localIp(); // choisit la meilleure IP (préférence Wi-Fi)
};

#endif // SIMPLEPAYSERVER_H
