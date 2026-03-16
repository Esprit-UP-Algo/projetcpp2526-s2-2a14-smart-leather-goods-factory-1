#ifndef PAGECHAT_H
#define PAGECHAT_H

#include <QWidget>
#include <QTimer>

class pageemployee;

namespace Ui {
class pagechat;
}

class pagechat : public QWidget
{
    Q_OBJECT

public:
    explicit pagechat(int idEmployeConnecte, pageemployee *pageEmp = nullptr, QWidget *parent = nullptr);
    ~pagechat();

private slots:
    void on_btnSend_clicked();
    void loadMessages();
    void on_pushButton_5_clicked();

private:
    Ui::pagechat *ui;
    int m_idEmploye;
    QTimer *m_timer;
    pageemployee *m_pageEmployee;
};

#endif // PAGECHAT_H
