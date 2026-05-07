#ifndef TWILIO_SMS_H
#define TWILIO_SMS_H

#include <QObject>
#include <QString>
#include <functional>

class QNetworkAccessManager;

class TwilioSms : public QObject
{
public:
    explicit TwilioSms(QObject *parent = nullptr);
    ~TwilioSms() override;

    bool isConfigured() const;

    /** Sends SMS when order status changes. @a toRaw E.164 (+...) or local digits (TN: 8 digits → +216). */
    void sendOrderStatusSms(const QString &toRaw,
                            const QString &clientName,
                            const QString &refCommande,
                            const QString &newStatus,
                            std::function<void(bool ok, const QString &detail)> onFinished = {});

private:
    QNetworkAccessManager *m_nam;
    QString m_accountSid;
    QString m_authToken;
    QString m_fromNumber;

    void loadCredentials();
    static QString toE164(const QString &raw);
};

#endif
