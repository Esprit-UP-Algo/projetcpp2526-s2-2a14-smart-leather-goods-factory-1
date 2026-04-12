/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QFrame *mainContainer;
    QFrame *loginForm;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnSignup;
    QPushButton *btnLogin;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLineEdit *username;
    QLineEdit *password;
    QPushButton *btnFaceId;
    QLabel *label;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName("login");
        login->resize(1408, 838);
        login->setMinimumSize(QSize(1200, 800));
        login->setStyleSheet(QString::fromUtf8("\n"
"/* =====================================================\n"
"   PREMIUM LEATHER THEME - MASTER CRAFTSMAN EDITION\n"
"===================================================== */\n"
"\n"
"/* Global Background - Distressed Leather */\n"
"QDialog {\n"
"    background-color: #2c1a0e;\n"
"    background-image: url(:/images/leather-texture.png);\n"
"    background-repeat: repeat;\n"
"}\n"
"\n"
"/* Main Container - Embossed Leather Panel */\n"
"#mainContainer {\n"
"    background-color: rgba(235, 215, 190, 0.95);\n"
"    border: 3px solid #8b5a2b;\n"
"    border-radius: 30px;\n"
"    border-bottom: 5px solid #6b3e1a;\n"
"}\n"
"\n"
"/* Logo/Title Area - Tooled Leather */\n"
"#titleFrame {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #c49a6c,\n"
"        stop:0.5 #a67c52,\n"
"        stop:1 #8b5a2b);\n"
"    border: 2px solid #e9d4b0;\n"
"    border-radius: 20px;\n"
"    padding: 15px;\n"
"}\n"
"\n"
"#titleFrame QLabel {\n"
"    color: #f5e6d3;\n"
"    font-size: 28px;\n"
"    font-we"
                        "ight: bold;\n"
"    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);\n"
"}\n"
"\n"
"/* Login Form Container - Hand-Stitched Leather */\n"
"#loginForm {\n"
"    background-color: #f8efe4;\n"
"    border: 3px solid #b2825b;\n"
"    border-radius: 25px;\n"
"    border-left: 6px solid #8b5a2b;\n"
"    border-bottom: 6px solid #6b3e1a;\n"
"}\n"
"\n"
"/* Form Title */\n"
"#formTitle {\n"
"    color: #5d3a1b;\n"
"    font-size: 24px;\n"
"    font-weight: bold;\n"
"    font-family: \"Segoe UI\", \"Times New Roman\", serif;\n"
"    padding: 10px 0;\n"
"    border-bottom: 2px dashed #b2825b;\n"
"}\n"
"\n"
"/* Input Fields - Burnished Leather */\n"
"QLineEdit {\n"
"    background-color: #fff9f0;\n"
"    border: 2px solid #b2825b;\n"
"    border-radius: 12px;\n"
"    padding: 12px 15px;\n"
"    font-size: 14px;\n"
"    color: #3d2b1a;\n"
"    selection-background-color: #c49a6c;\n"
"    margin: 5px 0;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 3px solid #8b5a2b;\n"
"    background-color: #ffffff;\n"
"    border-left: 8p"
                        "x solid #c49a6c;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: #9b7a5c;\n"
"    font-style: italic;\n"
"}\n"
"\n"
"/* Password Toggle Button - Metal Rivet Style */\n"
"#btnTogglePassword {\n"
"    background-color: #d4b28c;\n"
"    border: 2px solid #6b3e1a;\n"
"    border-radius: 12px;\n"
"    color: #3d2b1a;\n"
"    font-weight: bold;\n"
"    padding: 8px 15px;\n"
"    border-bottom: 4px solid #4a2a12;\n"
"}\n"
"\n"
"#btnTogglePassword:hover {\n"
"    background-color: #e4c4a4;\n"
"    border-bottom: 2px solid #4a2a12;\n"
"}\n"
"\n"
"/* Checkbox - Leather Toggle Style */\n"
"QCheckBox {\n"
"    color: #3d2b1a;\n"
"    font-weight: bold;\n"
"    spacing: 8px;\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 20px;\n"
"    height: 20px;\n"
"    background-color: #fff9f0;\n"
"    border: 2px solid #b2825b;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"    background-color: #8b5a2b;\n"
"    border: 2px solid #5d3a1b;\n"
"}\n"
"\n"
"/* Links - Tooled Leather Style */\n"
""
                        ".linkLabel {\n"
"    color: #6b3e1a;\n"
"    font-size: 13px;\n"
"    font-weight: bold;\n"
"    text-decoration: underline;\n"
"    padding: 8px;\n"
"    background-color: rgba(255, 255, 255, 0.3);\n"
"    border-radius: 8px;\n"
"}\n"
"\n"
".linkLabel:hover {\n"
"    color: #c49a6c;\n"
"    background-color: rgba(180, 130, 90, 0.2);\n"
"}\n"
"\n"
"/* Action Buttons - Hand-Stitched Leather Patches */\n"
"QPushButton {\n"
"    border: 3px solid #5d3a1b;\n"
"    border-radius: 15px;\n"
"    font-size: 16px;\n"
"    font-weight: bold;\n"
"    padding: 12px 25px;\n"
"    min-width: 150px;\n"
"}\n"
"\n"
"#btnSignup {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #a67c52,\n"
"        stop:1 #7b4f2a);\n"
"    color: #f5e6d3;\n"
"    border-bottom: 6px solid #4a2a12;\n"
"}\n"
"\n"
"#btnSignup:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #b88e64,\n"
"        stop:1 #8b5a2b);\n"
"    border-bottom: 4px solid #4a2a12;\n"
"}\n"
"\n"
"#btnLogin {\n"
""
                        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #6f8f3d,\n"
"        stop:1 #4c6328);\n"
"    color: #f5e6d3;\n"
"    border-bottom: 6px solid #2c3d18;\n"
"}\n"
"\n"
"#btnLogin:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #8ab048,\n"
"        stop:1 #5f7a32);\n"
"    border-bottom: 4px solid #2c3d18;\n"
"}\n"
"\n"
"/* Decorative Elements - Leather Stitching */\n"
".stitching {\n"
"    background-color: transparent;\n"
"    border: 2px dashed #b2825b;\n"
"    border-radius: 30px;\n"
"}\n"
"\n"
"/* Help Section */\n"
"#helpSection {\n"
"    background-color: rgba(180, 130, 90, 0.2);\n"
"    border-top: 2px solid #b2825b;\n"
"    border-radius: 0 0 25px 25px;\n"
"    padding: 15px;\n"
"}\n"
"   "));
        mainContainer = new QFrame(login);
        mainContainer->setObjectName("mainContainer");
        mainContainer->setGeometry(QRect(240, 90, 900, 631));
        mainContainer->setFrameShape(QFrame::Shape::StyledPanel);
        loginForm = new QFrame(mainContainer);
        loginForm->setObjectName("loginForm");
        loginForm->setGeometry(QRect(43, 183, 814, 421));
        layoutWidget = new QWidget(loginForm);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(80, 260, 641, 57));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnSignup = new QPushButton(layoutWidget);
        btnSignup->setObjectName("btnSignup");

        horizontalLayout->addWidget(btnSignup);

        btnLogin = new QPushButton(layoutWidget);
        btnLogin->setObjectName("btnLogin");

        horizontalLayout->addWidget(btnLogin);

        layoutWidget1 = new QWidget(loginForm);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(56, 76, 721, 127));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        username = new QLineEdit(layoutWidget1);
        username->setObjectName("username");

        verticalLayout->addWidget(username);

        password = new QLineEdit(layoutWidget1);
        password->setObjectName("password");
        password->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(password);

        btnFaceId = new QPushButton(loginForm);
        btnFaceId->setObjectName("btnFaceId");
        btnFaceId->setGeometry(QRect(250, 330, 288, 71));
        btnFaceId->setStyleSheet(QString::fromUtf8("#btnFaceId {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #5d3a1b,\n"
"        stop:0.35 #8b5a2b,\n"
"        stop:0.7 #c49a6c,\n"
"        stop:1 #8b5a2b);\n"
"    color: #fff8ee;\n"
"    border: 2px solid #f0d3ab;\n"
"    border-radius: 18px;\n"
"    font-size: 15px;\n"
"    font-weight: 700;\n"
"    font-family: \"Segoe UI\", serif;\n"
"    padding: 10px 22px;\n"
"    min-width: 240px;\n"
"    min-height: 44px;\n"
"    border-bottom: 5px solid #4a2a12;\n"
"}\n"
"\n"
"#btnFaceId:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #6b4421,\n"
"        stop:0.35 #9b6735,\n"
"        stop:0.7 #ddb98d,\n"
"        stop:1 #9b6735);\n"
"    border: 2px solid #fff0dc;\n"
"}\n"
"\n"
"#btnFaceId:pressed {\n"
"    background: #6b3e1a;\n"
"    border-bottom: 2px solid #4a2a12;\n"
"}"));
        label = new QLabel(mainContainer);
        label->setObjectName("label");
        label->setGeometry(QRect(330, 30, 231, 121));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        label->setScaledContents(true);

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Smart Leather Factory - Connexion", nullptr));
#if QT_CONFIG(whatsthis)
        mainContainer->setWhatsThis(QCoreApplication::translate("login", "<html><head/><body><p><span style=\" font-size:28pt;\">LEATHERCRAFT</span></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        btnSignup->setText(QCoreApplication::translate("login", "\360\237\223\235 S'INSCRIRE", nullptr));
        btnLogin->setText(QCoreApplication::translate("login", "\360\237\224\223 CONNEXION", nullptr));
        username->setPlaceholderText(QCoreApplication::translate("login", "Email", nullptr));
        password->setPlaceholderText(QCoreApplication::translate("login", "Mot de passe", nullptr));
        btnFaceId->setText(QCoreApplication::translate("login", "Face Id", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
