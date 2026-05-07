#ifndef QRPAYMENTDIALOG_H
#define QRPAYMENTDIALOG_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SimplePayServer;

class QrPaymentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QrPaymentDialog(const QString &ref,
                             const QString &amount,
                             const QString &clientName,
                             QWidget *parent = nullptr);

private slots:
    void onQrImageLoaded(QNetworkReply *reply);
    void openInBrowser();
    void copyLink();
    void saveQrImage();   // Sauvegarde le QR Code en PNG sur le disque

private:
    SimplePayServer *m_server   = nullptr;
    QString  m_ref, m_amount, m_clientName;
    QString  m_stripeUrl;

    QPixmap      m_qrPixmap;          // Image QR stockée pour la sauvegarde
    QLabel      *m_qrLabel  = nullptr;
    QLabel      *m_urlLabel = nullptr;
    QPushButton *m_btnCopy  = nullptr;
    QPushButton *m_btnSave  = nullptr;

    void buildUi();
    void buildUiStatic(const QString &url);
    void loadQrImage(const QString &url);
    void applyQrPixmap();             // Applique m_qrPixmap dans le label
};

#endif // QRPAYMENTDIALOG_H
