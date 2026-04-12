/********************************************************************************
** Form generated from reading UI file 'produitswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUITSWINDOW_H
#define UI_PRODUITSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_produitswindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QTableView *tableView;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_7;
    QLabel *label_8;
    QPushButton *pushButton_8;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QLabel *label_9;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLabel *label_10;
    QLabel *label_tri;
    QComboBox *comboBox_tri;
    QWidget *page_2;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *produitswindow)
    {
        if (produitswindow->objectName().isEmpty())
            produitswindow->setObjectName("produitswindow");
        produitswindow->resize(1740, 716);
        produitswindow->setStyleSheet(QString::fromUtf8("/* \342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\n"
"   SMART LEATHER \342\200\223 PREMIUM CUIR THEME v2\n"
"\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342"
                        "\225\220\342\225\220\342\225\220\342\225\220\342\225\220\342\225\220 */\n"
"\n"
"/* \342\224\200\342\224\200 GLOBAL \342\224\200\342\224\200 */\n"
"QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0.3, y2:1,\n"
"        stop:0 #f5efe8, stop:0.4 #ece3d8, stop:1 #e4d9cc);\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 13px;\n"
"    color: #3a2a20;\n"
"}\n"
"\n"
"QWidget {\n"
"    font-family: \"Segoe UI\";\n"
"    color: #3a2a20;\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 LABELS \342\224\200\342\224\200 */\n"
"QLabel {\n"
"    color: #4a3628;\n"
"    background: transparent;\n"
"}\n"
"\n"
"QLabel#label_8 {\n"
"    font-size: 20px;\n"
"    font-weight: 800;\n"
"    color: #3a1f14;\n"
"    letter-spacing: 2px;\n"
"}\n"
"\n"
"QLabel#label_tri, QLabel#label_9, QLabel#label_10 {\n"
"    font-size: 12px;\n"
"    font-weight: 700;\n"
"    color: #5b3a28;\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 INPUTS \342\224\200\342\224\200 */\n"
"QLineEdit, QDateTimeEdit, QComboBox {\n"
"    background-color"
                        ": rgba(255, 255, 255, 0.75);\n"
"    border: 2px solid #d4c4b0;\n"
"    border-radius: 10px;\n"
"    padding: 7px 12px;\n"
"    color: #3a2a20;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus {\n"
"    border: 2px solid #8b6f5a;\n"
"    background-color: rgba(255, 255, 255, 0.95);\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    padding-right: 8px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #faf6f1;\n"
"    border: 2px solid #d4c4b0;\n"
"    border-radius: 6px;\n"
"    selection-background-color: #c9a87c;\n"
"    selection-color: white;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 TABLE \342\224\200\342\224\200 */\n"
"QTableView {\n"
"    background-color: rgba(255, 255, 255, 0.7);\n"
"    border: 2px solid #c9b8a5;\n"
"    border-radius: 16px;\n"
"    gridline-color: #e8ddd0;\n"
"    alternate-background-color: #f5efe8;\n"
"    selection-background-color: #c9a87c;\n"
"    selection-color: #2a1a12;\n"
""
                        "}\n"
"\n"
"QTableView::viewport {\n"
"    background-color: transparent;\n"
"    border-radius: 14px;\n"
"}\n"
"\n"
"QHeaderView {\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #5b3020, stop:0.5 #4a2517, stop:1 #3a1a10);\n"
"    color: #f5efe8;\n"
"    padding: 10px 8px;\n"
"    border: none;\n"
"    font-weight: 700;\n"
"    font-size: 12px;\n"
"    letter-spacing: 1px;\n"
"}\n"
"\n"
"QHeaderView::section:first {\n"
"    border-top-left-radius: 14px;\n"
"}\n"
"\n"
"QHeaderView::section:last {\n"
"    border-top-right-radius: 14px;\n"
"}\n"
"\n"
"QTableView::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #e8ddd0;\n"
"}\n"
"\n"
"QTableView::item:selected {\n"
"    background-color: #c9a87c;\n"
"    color: #2a1a12;\n"
"}\n"
"\n"
"QTableView::item:hover {\n"
"    background-color: rgba(201, 168, 124, 0.3);\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 BUTTONS (default) \342\224\200\342\224\200"
                        " */\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #6b4530, stop:1 #4a2a18);\n"
"    border: none;\n"
"    border-radius: 12px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 700;\n"
"    font-size: 12px;\n"
"    color: #f5efe8;\n"
"    letter-spacing: 0.5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #7d5840, stop:1 #5b3a28);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: #3a1f14;\n"
"    padding-top: 12px;\n"
"    padding-bottom: 8px;\n"
"}\n"
"\n"
"/* Ajouter \342\200\223 vert */\n"
"QPushButton#pushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #5a8a2a, stop:1 #7aaa4a);\n"
"    border-radius: 12px;\n"
"}\n"
"QPushButton#pushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #6a9a3a, stop:1 #8aba5a);\n"
"}\n"
"\n"
"/* Modifier \342\200\223 ambre */\n"
"QPushButton#pushButton_2 {\n"
"    backgro"
                        "und: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #b06a1c, stop:1 #d08a3c);\n"
"}\n"
"QPushButton#pushButton_2:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #c07a2c, stop:1 #e09a4c);\n"
"}\n"
"\n"
"/* Supprimer \342\200\223 rouge */\n"
"QPushButton#pushButton_3 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #8a2a1a, stop:1 #b04a3a);\n"
"    border-radius: 12px;\n"
"}\n"
"QPushButton#pushButton_3:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #9a3a2a, stop:1 #c05a4a);\n"
"}\n"
"\n"
"/* Actualiser \342\200\223 cuir */\n"
"QPushButton#pushButton_4 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #6b5040, stop:1 #8b7060);\n"
"}\n"
"\n"
"/* Exporter PDF / Statistiques */\n"
"QPushButton#pushButton_7, QPushButton#pushButton_9 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #4a3a2a, stop:1 #6a5a4a);\n"
"    border: 2px solid #"
                        "c9a87c;\n"
"    font-size: 11px;\n"
"    letter-spacing: 1px;\n"
"}\n"
"QPushButton#pushButton_7:hover, QPushButton#pushButton_9:hover {\n"
"    border-color: #e0c090;\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #5a4a3a, stop:1 #7a6a5a);\n"
"}\n"
"\n"
"/* Recherche */\n"
"QPushButton#pushButton_10 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #5b3a28, stop:1 #7b5a48);\n"
"    border: 2px solid #c9a87c;\n"
"}\n"
"\n"
"/* Pagination \342\200\223 petits cercles */\n"
"QPushButton#pushButton_8,\n"
"QPushButton#pushButton_13,\n"
"QPushButton#pushButton_14,\n"
"QPushButton#pushButton_15,\n"
"QPushButton#pushButton_16 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #a08060, stop:1 #806040);\n"
"    border: none;\n"
"    border-radius: 14px;\n"
"    min-width: 28px;\n"
"    max-width: 28px;\n"
"    min-height: 28px;\n"
"    max-height: 28px;\n"
"    font-size: 10px;\n"
"    font-weight: 700;\n"
"    padding: 0px;\n"
""
                        "}\n"
"QPushButton#pushButton_8:hover,\n"
"QPushButton#pushButton_13:hover,\n"
"QPushButton#pushButton_14:hover,\n"
"QPushButton#pushButton_15:hover,\n"
"QPushButton#pushButton_16:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #b09070, stop:1 #907050);\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 CALENDAR POPUP \342\224\200\342\224\200 */\n"
"QCalendarWidget {\n"
"    background-color: #faf6f1;\n"
"    color: #3a2a20;\n"
"}\n"
"QCalendarWidget QWidget#qt_calendar_navigationbar {\n"
"    background-color: #5b3020;\n"
"    border-radius: 6px;\n"
"    min-height: 32px;\n"
"}\n"
"QCalendarWidget QToolButton {\n"
"    color: #f5efe8;\n"
"    background: transparent;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    padding: 4px 8px;\n"
"    border-radius: 4px;\n"
"}\n"
"QCalendarWidget QToolButton:hover {\n"
"    background-color: #7b5040;\n"
"}\n"
"QCalendarWidget QSpinBox {\n"
"    background-color: #4a2517;\n"
"    color: #f5efe8;\n"
"    border: none;\n"
"    borde"
                        "r-radius: 4px;\n"
"    padding: 2px 6px;\n"
"    font-size: 13px;\n"
"    font-weight: bold;\n"
"}\n"
"QCalendarWidget QAbstractItemView {\n"
"    background-color: #faf6f1;\n"
"    color: #3a2a20;\n"
"    selection-background-color: #c9a87c;\n"
"    selection-color: #2a1a12;\n"
"    alternate-background-color: #f0e8de;\n"
"    font-size: 12px;\n"
"    border: none;\n"
"}\n"
"QCalendarWidget QMenu {\n"
"    background-color: #faf6f1;\n"
"    color: #3a2a20;\n"
"    border: 1px solid #d4c4b0;\n"
"}\n"
"QCalendarWidget QMenu::item:selected {\n"
"    background-color: #c9a87c;\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 MESSAGE BOX \342\224\200\342\224\200 */\n"
"QMessageBox {\n"
"    background-color: #faf6f1;\n"
"}\n"
"QMessageBox QLabel {\n"
"    color: #3a2a20;\n"
"    font-size: 13px;\n"
"}\n"
"QMessageBox QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #6b4530, stop:1 #4a2a18);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 24px;\n"
"    fo"
                        "nt-weight: 700;\n"
"    color: #f5efe8;\n"
"    min-width: 80px;\n"
"}\n"
"QMessageBox QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #7d5840, stop:1 #5b3a28);\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 MENU BAR \342\224\200\342\224\200 */\n"
"QMenuBar {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #3a1a10, stop:1 #4a2a1a);\n"
"    color: #f5efe8;\n"
"    border-bottom: 2px solid #c9a87c;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    background-color: #6b4530;\n"
"}\n"
"\n"
"/* \342\224\200\342\224\200 STATUS BAR \342\224\200\342\224\200 */\n"
"QStatusBar {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 #e4d9cc, stop:1 #d8cbb8);\n"
"    color: #4a3628;\n"
"    border-top: 1px solid #c9b8a5;\n"
"    font-size: 11px;\n"
"}\n"
""));
        centralwidget = new QWidget(produitswindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(240, 20, 1461, 691));
        page = new QWidget();
        page->setObjectName("page");
        tableView = new QTableView(page);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(0, 170, 1021, 411));
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);
        pushButton = new QPushButton(page);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(140, 620, 101, 41));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(290, 620, 101, 41));
        pushButton_3 = new QPushButton(page);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(460, 620, 101, 41));
        pushButton_4 = new QPushButton(page);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(620, 620, 101, 41));
        pushButton_7 = new QPushButton(page);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(780, 48, 151, 41));
        label_8 = new QLabel(page);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, -10, 381, 81));
        pushButton_8 = new QPushButton(page);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(230, 528, 32, 32));
        pushButton_13 = new QPushButton(page);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(306, 528, 32, 32));
        pushButton_14 = new QPushButton(page);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(268, 528, 32, 32));
        pushButton_15 = new QPushButton(page);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(382, 528, 32, 32));
        pushButton_16 = new QPushButton(page);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(344, 528, 32, 32));
        pushButton_9 = new QPushButton(page);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(950, 50, 141, 41));
        pushButton_10 = new QPushButton(page);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(610, 118, 141, 41));
        label_9 = new QLabel(page);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 130, 141, 31));
        lineEdit_5 = new QLineEdit(page);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(140, 130, 131, 31));
        lineEdit_6 = new QLineEdit(page);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(460, 130, 131, 31));
        label_10 = new QLabel(page);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(300, 130, 171, 31));
        label_tri = new QLabel(page);
        label_tri->setObjectName("label_tri");
        label_tri->setGeometry(QRect(10, 70, 101, 31));
        comboBox_tri = new QComboBox(page);
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->setObjectName("comboBox_tri");
        comboBox_tri->setGeometry(QRect(110, 70, 181, 31));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 211, 121));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        label->setScaledContents(true);
        produitswindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(produitswindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1740, 25));
        produitswindow->setMenuBar(menubar);
        statusbar = new QStatusBar(produitswindow);
        statusbar->setObjectName("statusbar");
        produitswindow->setStatusBar(statusbar);

        retranslateUi(produitswindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(produitswindow);
    } // setupUi

    void retranslateUi(QMainWindow *produitswindow)
    {
        produitswindow->setWindowTitle(QCoreApplication::translate("produitswindow", "Gestion des Produits", nullptr));
        pushButton->setText(QCoreApplication::translate("produitswindow", "Ajouter", nullptr));
        pushButton_2->setText(QCoreApplication::translate("produitswindow", "Modifier", nullptr));
        pushButton_3->setText(QCoreApplication::translate("produitswindow", "Supprimer", nullptr));
        pushButton_4->setText(QCoreApplication::translate("produitswindow", "Actualiser", nullptr));
        pushButton_7->setText(QCoreApplication::translate("produitswindow", "Exporter PDF", nullptr));
        label_8->setText(QCoreApplication::translate("produitswindow", "<html><head/><body><p><span style=\" font-size:16pt; font-style:italic; text-decoration: underline;\">Gestion des produits :</span></p></body></html>", nullptr));
        pushButton_8->setText(QCoreApplication::translate("produitswindow", "<", nullptr));
        pushButton_13->setText(QCoreApplication::translate("produitswindow", "2", nullptr));
        pushButton_14->setText(QCoreApplication::translate("produitswindow", "1", nullptr));
        pushButton_15->setText(QCoreApplication::translate("produitswindow", ">", nullptr));
        pushButton_16->setText(QCoreApplication::translate("produitswindow", "3", nullptr));
        pushButton_9->setText(QCoreApplication::translate("produitswindow", "Statistiques", nullptr));
        pushButton_10->setText(QCoreApplication::translate("produitswindow", "Recherche", nullptr));
        label_9->setText(QCoreApplication::translate("produitswindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Nom produit :</span></p><p><br/></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("produitswindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">prix du produit :</span></p><p><br/></p></body></html>", nullptr));
        label_tri->setText(QCoreApplication::translate("produitswindow", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Trier par :</span></p></body></html>", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("produitswindow", "Aucun tri", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("produitswindow", "Prix croissant", nullptr));
        comboBox_tri->setItemText(2, QCoreApplication::translate("produitswindow", "Prix d\303\251croissant", nullptr));
        comboBox_tri->setItemText(3, QCoreApplication::translate("produitswindow", "Cat\303\251gorie A-Z", nullptr));

        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class produitswindow: public Ui_produitswindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUITSWINDOW_H
