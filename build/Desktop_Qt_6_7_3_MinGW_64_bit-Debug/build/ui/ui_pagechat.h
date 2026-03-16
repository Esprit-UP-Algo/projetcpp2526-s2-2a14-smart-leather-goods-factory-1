/********************************************************************************
** Form generated from reading UI file 'pagechat.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGECHAT_H
#define UI_PAGECHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pagechat
{
public:
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *searchIdEdit;
    QLabel *label_4;
    QLineEdit *searchNomEdit;
    QGroupBox *groupBox_2;
    QLabel *label_5;
    QLabel *label_8;
    QListWidget *listMessages;
    QLineEdit *inputMessage;
    QPushButton *btnSend;
    QPushButton *pushButton_5;

    void setupUi(QWidget *pagechat)
    {
        if (pagechat->objectName().isEmpty())
            pagechat->setObjectName("pagechat");
        pagechat->resize(1430, 818);
        horizontalLayoutWidget_4 = new QWidget(pagechat);
        horizontalLayoutWidget_4->setObjectName("horizontalLayoutWidget_4");
        horizontalLayoutWidget_4->setGeometry(QRect(459, 279, 481, 41));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_4);
        label_3->setObjectName("label_3");

        horizontalLayout_5->addWidget(label_3);

        searchIdEdit = new QLineEdit(horizontalLayoutWidget_4);
        searchIdEdit->setObjectName("searchIdEdit");
        searchIdEdit->setStyleSheet(QString::fromUtf8("background-color : black;\n"
"color : white;"));

        horizontalLayout_5->addWidget(searchIdEdit);

        label_4 = new QLabel(horizontalLayoutWidget_4);
        label_4->setObjectName("label_4");

        horizontalLayout_5->addWidget(label_4);

        searchNomEdit = new QLineEdit(horizontalLayoutWidget_4);
        searchNomEdit->setObjectName("searchNomEdit");
        searchNomEdit->setStyleSheet(QString::fromUtf8("background-color : black;\n"
"color : white;"));

        horizontalLayout_5->addWidget(searchNomEdit);

        groupBox_2 = new QGroupBox(pagechat);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(-10, 10, 1541, 921));
        groupBox_2->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
"   THEME ERP \342\200\223 STITCHED LEATHER / CUIR PREMIUM\n"
"===================================================== */\n"
"\n"
"/* =======================\n"
"   GLOBAL\n"
"======================= */\n"
"QMainWindow, QWidget {\n"
"    background-color: #f1e7dc;\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 13px;\n"
"    color: #3a2a20;\n"
"}\n"
"\n"
"/* =======================\n"
"   LABELS\n"
"======================= */\n"
"QLabel {\n"
"    color: #3a2a20;\n"
"}\n"
"\n"
"/* Main title */\n"
"QLabel#label_8 {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #5b2f1d;\n"
"    letter-spacing: 1px;\n"
"}\n"
"\n"
"/* =======================\n"
"   GROUPBOX \342\200\223 STITCHED LEATHER\n"
"======================= */\n"
"QGroupBox {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #fff6ec,\n"
"        stop:1 #ead7c6\n"
"    );\n"
"    border: 2px dashed #b08a6b;   /* stitched effect"
                        " */\n"
"    border-radius: 14px;\n"
"    margin-top: 22px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 16px;\n"
"    top: -12px;\n"
"    padding: 2px 10px;\n"
"    background-color: #f1e7dc;\n"
"    color: #5b2f1d;\n"
"    font-weight: bold;\n"
"    letter-spacing: 0.5px;\n"
"}\n"
"\n"
"/* =======================\n"
"   INPUTS \342\200\223 EMBEDDED LEATHER\n"
"======================= */\n"
"QLineEdit, QDateTimeEdit, QComboBox {\n"
"    background-color: rgba(255, 250, 245, 0.6);\n"
"    border: 1px solid #c6a88d;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"    color: #3a2a20;\n"
"}\n"
"\n"
"QLineEdit:focus,\n"
"QDateTimeEdit:focus,\n"
"QComboBox:focus {\n"
"    border: 2px solid #6b3e26;\n"
"    background-color: #fffaf5;\n"
"}\n"
"\n"
"QLineEdit:disabled,\n"
"QDateTimeEdit:disabled {\n"
"    color: #8a7a6e;\n"
"    background-color: #e6d8cc;\n"
"}\n"
"\n"
"/* =======================\n"
"   COMBOBOX DROPDOWN\n"
"======================= */\n"
"QComboBox::drop-dow"
                        "n {\n"
"    border-left: 1px solid #c6a88d;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #fffaf5;\n"
"    selection-background-color: #d8b59c;\n"
"    border: 1px solid #c6a88d;\n"
"}\n"
"\n"
"/* =======================\n"
"   TABLE \342\200\223 LEATHER PANEL\n"
"======================= */\n"
"QTableWidget {\n"
"    background-color: #fffaf5;\n"
"    border: 2px solid #b08a6b;\n"
"    border-radius: 14px;\n"
"    gridline-color: #e0d2c5;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #6b3e26,\n"
"        stop:1 #4a2717\n"
"    );\n"
"    color: #fffaf5;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"    letter-spacing: 0.5px;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #d8b59c;\n"
"    color: #2a1a12;\n"
"}\n"
"\n"
"/* =======================\n"
"   BUTTONS \342\200\223 LEATHER PATCH\n"
""
                        "======================= */\n"
"QPushButton {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #7a4a2e,\n"
"        stop:1 #5b2f1d\n"
"    );\n"
"    border: 2px solid #3a1f14;\n"
"    border-radius: 10px;\n"
"    padding: 8px 16px;\n"
"    font-weight: bold;\n"
"    color: #fffaf5;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #8b5a3a,\n"
"        stop:1 #6b3e26\n"
"    );\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #4a2717;\n"
"}\n"
"\n"
"/* Accent buttons */\n"
"QPushButton#pushButton {\n"
"    background-color: #6f8f3d;\n"
"    border-color: #4c6328;\n"
"}\n"
"\n"
"QPushButton#pushButton_3 {\n"
"    background-color: #a23b2a;\n"
"    border-color: #6e2418;\n"
"}\n"
"\n"
"/* Pagination buttons */\n"
"QPushButton#pushButton_8,\n"
"QPushButton#pushButton_13,\n"
"QPushButton#pushButton_14,\n"
"QPushButton#pushButton_15,\n"
"QPushButton#pushButton_16 {\n"
"   "
                        " background-color: #a47148;\n"
"    border-color: #6b3e26;\n"
"}\n"
"\n"
"/* =======================\n"
"   MENU BAR \342\200\223 DARK LEATHER\n"
"======================= */\n"
"QMenuBar {\n"
"    background-color: #4a2717;\n"
"    color: #fffaf5;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background-color: #7a4a2e;\n"
"}\n"
"\n"
"/* =======================\n"
"   STATUS BAR\n"
"======================= */\n"
"QStatusBar {\n"
"    background-color: #e9dccf;\n"
"    color: #3a2a20;\n"
"    border-top: 1px solid #c6a88d;\n"
"}\n"
""));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(200, 60, 211, 121));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        label_5->setScaledContents(true);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(590, 110, 371, 81));
        label_8->setStyleSheet(QString::fromUtf8("/* ===== GLOBAL ===== */\n"
"QMainWindow {\n"
"    background-color: #f4efe9;\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 11pt;\n"
"}\n"
"\n"
"/* ===== GROUP BOX ===== */\n"
"QGroupBox {\n"
"    background-color: #f8f3ed;\n"
"    border: 1px solid #c7b299;\n"
"    border-radius: 6px;\n"
"    margin-top: 20px;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 12px;\n"
"    padding: 0 6px;\n"
"    color: #4b2e1f;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* ===== LABELS ===== */\n"
"QLabel {\n"
"    color: #3b2a1a;\n"
"}\n"
"\n"
"/* ===== INPUTS ===== */\n"
"QLineEdit, QTextBrowser, QComboBox, QDateEdit {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #c7b299;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border-left: 1px solid #c7b299;\n"
"}\n"
"\n"
"/* ===== TABLE ===== */\n"
"QTableWidget {\n"
"    background-color: #ffffff;\n"
"    border: 1px solid #c7b299;\n"
"    gridline-color"
                        ": #e0d6c9;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #6b3f2b;\n"
"    color: white;\n"
"    padding: 6px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #d8c2aa;\n"
"    color: black;\n"
"}\n"
"\n"
"/* ===== BUTTONS (DEFAULT) ===== */\n"
"QPushButton {\n"
"    background-color: #6b3f2b;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px 14px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #8a5a44;\n"
"}\n"
"\n"
"/* ===== ACTION BUTTONS ===== */\n"
"QPushButton#pushButton_3 { /* Ajouter */\n"
"    background-color: #5a7d3b;\n"
"}\n"
"\n"
"QPushButton#pushButton_4 { /* Modifier */\n"
"    background-color: #d07a2d;\n"
"}\n"
"\n"
"QPushButton#pushButton_5 { /* Supprimer */\n"
"    background-color: #b23a2b;\n"
"}\n"
"\n"
"QPushButton#pushButton_6 { /* Actualiser */\n"
"    background-color: #7b4a32;\n"
"}\n"
"\n"
"/* ===== MENU BAR ===== */\n"
"QMenuBar {\n"
"    background-color: #6b3f"
                        "2b;\n"
"    color: white;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background-color: #8a5a44;\n"
"}\n"
"\n"
"/* ===== STATUS BAR ===== */\n"
"QStatusBar {\n"
"    background-color: #e7dccf;\n"
"}\n"
""));
        listMessages = new QListWidget(groupBox_2);
        listMessages->setObjectName("listMessages");
        listMessages->setGeometry(QRect(70, 230, 1321, 411));
        inputMessage = new QLineEdit(groupBox_2);
        inputMessage->setObjectName("inputMessage");
        inputMessage->setGeometry(QRect(100, 570, 971, 51));
        btnSend = new QPushButton(groupBox_2);
        btnSend->setObjectName("btnSend");
        btnSend->setGeometry(QRect(1130, 570, 201, 51));
        pushButton_5 = new QPushButton(groupBox_2);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(30, 670, 231, 51));

        retranslateUi(pagechat);

        QMetaObject::connectSlotsByName(pagechat);
    } // setupUi

    void retranslateUi(QWidget *pagechat)
    {
        pagechat->setWindowTitle(QCoreApplication::translate("pagechat", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("pagechat", "CIN :", nullptr));
        label_4->setText(QCoreApplication::translate("pagechat", "Nom :", nullptr));
        groupBox_2->setTitle(QString());
        label_5->setText(QString());
        label_8->setText(QCoreApplication::translate("pagechat", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:696; font-style:italic; text-decoration: underline; color:#550000;\">Chat des employ\303\251s :</span></p></body></html>", nullptr));
        inputMessage->setPlaceholderText(QCoreApplication::translate("pagechat", "\"\303\211crire un message...\"", nullptr));
        btnSend->setText(QCoreApplication::translate("pagechat", "Envoyer", nullptr));
        pushButton_5->setText(QCoreApplication::translate("pagechat", "quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pagechat: public Ui_pagechat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGECHAT_H
