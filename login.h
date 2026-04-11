#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "faceid.h"
#include <QProgressDialog>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    int getIdEmployeByEmail(const QString &email);

private slots:
    void on_btnLogin_clicked();
    void on_btnSignup_clicked();
    void on_btnFaceId_clicked();

private:
    Ui::login *ui;
    FaceID *faceId;

    bool verifierLogin(const QString &email, const QString &password);
    bool verifierLoginFaceId(const QString &cin);
    bool emailExiste(const QString &email);
    bool idExiste(int id);
    bool cinExiste(const QString &cin);
    QProgressDialog *loadingDialog;

    void redirigerSelonRole(int idEmp);
};

#endif // LOGIN_H
