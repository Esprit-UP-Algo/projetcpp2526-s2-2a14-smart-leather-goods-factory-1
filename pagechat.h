#ifndef PAGECHAT_H
#define PAGECHAT_H

#include <QWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class pageemployee;

namespace Ui {
class pagechat;
}

class pagechat : public QWidget
{
    Q_OBJECT

public:
    explicit pagechat(int idEmployeConnecte,
                      const QString &displayName,
                      pageemployee *pageEmp = nullptr,
                      QWidget *parent = nullptr);
    ~pagechat();

private slots:
    void on_btnSend_clicked();
    void loadMessages();
    void on_pushButton_5_clicked();

private:
    void ensureUserExists();
    void syncSendbirdNicknameThenContinue();
    void ensureChannelExists();
    void ensureUserInChannel();

    void handleCreateUserReply(QNetworkReply *reply);
    void handleCreateChannelReply(QNetworkReply *reply);
    void handleJoinChannelReply(QNetworkReply *reply);
    void handleLoadMessagesReply(QNetworkReply *reply);
    void handleSendMessageReply(QNetworkReply *reply);

private:
    Ui::pagechat *ui;
    int m_idEmploye;
    QString m_displayName;
    QTimer *m_timer;
    pageemployee *m_pageEmployee;
    QNetworkAccessManager *m_network;

    QString m_appId ="";// "3ACA12C5-32A0-40FD-A225-F22BD8E8EF2E";
    QString m_apiToken ="";// "88c406c33a15f992372f5ae68bad7bd3ddf7c208";
    QString m_channelUrl = "internal-chat";

    bool m_userReady = false;
    bool m_channelReady = false;
    bool m_membershipReady = false;
};

#endif // PAGECHAT_H
