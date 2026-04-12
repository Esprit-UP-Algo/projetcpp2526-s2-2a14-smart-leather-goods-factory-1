/********************************************************************************
** Form generated from reading UI file 'pageemployee.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGEEMPLOYEE_H
#define UI_PAGEEMPLOYEE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pageemployee
{
public:
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *searchIdEdit;
    QLabel *label_4;
    QLineEdit *searchNomEdit;
    QPushButton *search;
    QGroupBox *groupBox_2;
    QLabel *label_5;
    QGroupBox *groupBox;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_11;
    QPushButton *pushButton_21;
    QPushButton *pushButton_20;
    QPushButton *pushButton_22;
    QPushButton *pushButton_23;
    QPushButton *pushButton_8;
    QLabel *label_8;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *actualiser;
    QTableWidget *tableWidget;
    QPushButton *pushButton_4;
    QPushButton *btnSaveFacePhoto;
    QPushButton *pushButton_9;
    QPushButton *pushButton_7;

    void setupUi(QDialog *pageemployee)
    {
        if (pageemployee->objectName().isEmpty())
            pageemployee->setObjectName("pageemployee");
        pageemployee->resize(1297, 787);
        pageemployee->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
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
        horizontalLayoutWidget_4 = new QWidget(pageemployee);
        horizontalLayoutWidget_4->setObjectName("horizontalLayoutWidget_4");
        horizontalLayoutWidget_4->setGeometry(QRect(310, 230, 481, 41));
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

        search = new QPushButton(pageemployee);
        search->setObjectName("search");
        search->setGeometry(QRect(822, 230, 341, 41));
        search->setStyleSheet(QString::fromUtf8(" background-color: #5D3724;\n"
"color : black;\n"
""));
        groupBox_2 = new QGroupBox(pageemployee);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(-150, -50, 1541, 921));
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
        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(180, 180, 251, 651));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(11, 502, 231, 51));
        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(11, 58, 231, 51));
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(11, 132, 231, 51));
        pushButton_21 = new QPushButton(groupBox);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(11, 280, 231, 51));
        pushButton_20 = new QPushButton(groupBox);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(11, 206, 231, 51));
        pushButton_22 = new QPushButton(groupBox);
        pushButton_22->setObjectName("pushButton_22");
        pushButton_22->setGeometry(QRect(11, 354, 231, 51));
        pushButton_23 = new QPushButton(groupBox);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(11, 428, 231, 51));
        pushButton_8 = new QPushButton(groupBox);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(10, 570, 231, 51));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(540, 180, 371, 81));
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
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(530, 720, 751, 61));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName("pushButton");
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setBold(true);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("background-color : #8A7A3A;\n"
"color : black;\n"
""));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setFont(font);
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color : #C46A2C;\n"
"color : black;"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setFont(font);
        pushButton_3->setStyleSheet(QString::fromUtf8("background-color : #B5523B;\n"
"color : black;"));

        horizontalLayout->addWidget(pushButton_3);

        actualiser = new QPushButton(layoutWidget);
        actualiser->setObjectName("actualiser");
        actualiser->setFont(font);
        actualiser->setStyleSheet(QString::fromUtf8("background-color : #7A4A32;\n"
"color : black;"));

        horizontalLayout->addWidget(actualiser);

        tableWidget = new QTableWidget(groupBox_2);
        if (tableWidget->columnCount() < 8)
            tableWidget->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setEnabled(true);
        tableWidget->setGeometry(QRect(440, 330, 871, 381));
        tableWidget->setStyleSheet(QString::fromUtf8("/* =======================\n"
"   TABLE \342\200\223 PERFECT ROUNDED LEATHER\n"
"======================= */\n"
"\n"
"QTableWidget {\n"
"    background-color: #fffaf5;\n"
"    border: 2px solid #b08a6b;\n"
"    border-radius: 14px;\n"
"    gridline-color: #e0d2c5;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"/* IMPORTANT: clip the viewport */\n"
"QTableWidget::viewport {\n"
"    background-color: #fffaf5;\n"
"    border-radius: 12px;\n"
"}\n"
"\n"
"/* Remove internal borders */\n"
"QTableWidget QTableCornerButton::section {\n"
"    background-color: #6b3e26;\n"
"    border: none;\n"
"}\n"
"\n"
"/* Header styling */\n"
"QHeaderView {\n"
"    background-color: transparent;\n"
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
"}\n"
"\n"
"/* Round TOP corners manually */\n"
"QHeaderView::section:fir"
                        "st {\n"
"    border-top-left-radius: 12px;\n"
"}\n"
"\n"
"QHeaderView::section:last {\n"
"    border-top-right-radius: 12px;\n"
"}\n"
"\n"
"/* Table items */\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #e0d2c5;\n"
"}\n"
"\n"
"/* Selection */\n"
"QTableWidget::item:selected {\n"
"    background-color: #d8b59c;\n"
"    color: #2a1a12;\n"
"}\n"
""));
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tableWidget->setProperty("showDropIndicator", QVariant(true));
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setShowGrid(false);
        tableWidget->setSortingEnabled(false);
        tableWidget->setWordWrap(true);
        tableWidget->setColumnCount(8);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setCascadingSectionResizes(true);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->verticalHeader()->setStretchLastSection(true);
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(930, 160, 261, 51));
        btnSaveFacePhoto = new QPushButton(groupBox_2);
        btnSaveFacePhoto->setObjectName("btnSaveFacePhoto");
        btnSaveFacePhoto->setGeometry(QRect(1200, 160, 171, 51));
        pushButton_9 = new QPushButton(pageemployee);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(994, 181, 171, 38));
        pushButton_9->setStyleSheet(QString::fromUtf8(" background-color: #1E88E5;\n"
"color : black;"));
        pushButton_7 = new QPushButton(pageemployee);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(801, 181, 181, 38));
        pushButton_7->setStyleSheet(QString::fromUtf8(" background-color: #E53935;\n"
"color : black;\n"
""));
        groupBox_2->raise();
        horizontalLayoutWidget_4->raise();
        search->raise();
        pushButton_9->raise();
        pushButton_7->raise();

        retranslateUi(pageemployee);

        QMetaObject::connectSlotsByName(pageemployee);
    } // setupUi

    void retranslateUi(QDialog *pageemployee)
    {
        pageemployee->setWindowTitle(QCoreApplication::translate("pageemployee", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("pageemployee", "CIN :", nullptr));
        label_4->setText(QCoreApplication::translate("pageemployee", "Nom :", nullptr));
        search->setText(QCoreApplication::translate("pageemployee", "Search", nullptr));
        groupBox_2->setTitle(QString());
        label_5->setText(QString());
        groupBox->setTitle(QString());
        pushButton_5->setText(QCoreApplication::translate("pageemployee", "quitter", nullptr));
        pushButton_6->setText(QCoreApplication::translate("pageemployee", "Commandes", nullptr));
        pushButton_11->setText(QCoreApplication::translate("pageemployee", "Employe", nullptr));
        pushButton_21->setText(QCoreApplication::translate("pageemployee", "Products", nullptr));
        pushButton_20->setText(QCoreApplication::translate("pageemployee", "Fournisseur", nullptr));
        pushButton_22->setText(QCoreApplication::translate("pageemployee", "Matieres Premieres", nullptr));
        pushButton_23->setText(QCoreApplication::translate("pageemployee", "Machine", nullptr));
        pushButton_8->setText(QCoreApplication::translate("pageemployee", "deconnecter", nullptr));
        label_8->setText(QCoreApplication::translate("pageemployee", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:696; font-style:italic; text-decoration: underline; color:#550000;\">Gestion des employ\303\251s :</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("pageemployee", "Ajouter", nullptr));
        pushButton_2->setText(QCoreApplication::translate("pageemployee", "Modifier", nullptr));
        pushButton_3->setText(QCoreApplication::translate("pageemployee", "Supprimer", nullptr));
        actualiser->setText(QCoreApplication::translate("pageemployee", "Actualiser", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("pageemployee", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("pageemployee", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("pageemployee", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("pageemployee", "Date_naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("pageemployee", "Poste", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("pageemployee", "Niveau", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("pageemployee", "Salaire", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("pageemployee", "Email", nullptr));
        pushButton_4->setText(QCoreApplication::translate("pageemployee", "ChatBox", nullptr));
        btnSaveFacePhoto->setText(QCoreApplication::translate("pageemployee", "Enregistrer photo Face ID", nullptr));
        pushButton_9->setText(QCoreApplication::translate("pageemployee", "Statistiques", nullptr));
        pushButton_7->setText(QCoreApplication::translate("pageemployee", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pageemployee: public Ui_pageemployee {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGEEMPLOYEE_H
