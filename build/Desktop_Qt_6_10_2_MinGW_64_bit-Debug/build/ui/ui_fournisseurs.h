/********************************************************************************
** Form generated from reading UI file 'fournisseurs.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURNISSEURS_H
#define UI_FOURNISSEURS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_fournisseurs
{
public:
    QPushButton *pushButton_14;
    QPushButton *pushButton_10;
    QPushButton *pushButton_13;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *searchNomEdit;
    QPushButton *pushButton;
    QLabel *label_8;
    QLabel *label;
    QPushButton *pushButton_3;
    QPushButton *pushButton_6;
    QTableWidget *tableWidget;
    QLineEdit *searchIdEdit;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QPushButton *pushButton_2;
    QGroupBox *groupBox;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton_20;
    QPushButton *pushButton_22;
    QPushButton *pushButton_21;
    QPushButton *pushButton_23;

    void setupUi(QDialog *fournisseurs)
    {
        if (fournisseurs->objectName().isEmpty())
            fournisseurs->setObjectName("fournisseurs");
        fournisseurs->resize(1352, 782);
        fournisseurs->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
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
        pushButton_14 = new QPushButton(fournisseurs);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(730, 520, 51, 31));
        pushButton_10 = new QPushButton(fournisseurs);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(450, 520, 51, 31));
        pushButton_13 = new QPushButton(fournisseurs);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(660, 520, 51, 31));
        label_9 = new QLabel(fournisseurs);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(310, 150, 161, 141));
        label_9->setStyleSheet(QString::fromUtf8("/* ===== GLOBAL ===== */\n"
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
        label_10 = new QLabel(fournisseurs);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(670, 180, 161, 141));
        label_10->setStyleSheet(QString::fromUtf8("/* ===== GLOBAL ===== */\n"
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
        searchNomEdit = new QLineEdit(fournisseurs);
        searchNomEdit->setObjectName("searchNomEdit");
        searchNomEdit->setGeometry(QRect(800, 200, 161, 42));
        pushButton = new QPushButton(fournisseurs);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(430, 730, 90, 29));
        label_8 = new QLabel(fournisseurs);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(430, 110, 371, 81));
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
        label = new QLabel(fournisseurs);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 40, 251, 121));
        label->setStyleSheet(QString::fromUtf8("border-image: url(:/Logo.png);"));
        label->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/projet2626/projet2626/Logo.png")));
        label->setScaledContents(true);
        pushButton_3 = new QPushButton(fournisseurs);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(830, 730, 111, 29));
        pushButton_6 = new QPushButton(fournisseurs);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(1010, 730, 101, 31));
        tableWidget = new QTableWidget(fournisseurs);
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
        tableWidget->setGeometry(QRect(310, 260, 801, 461));
        searchIdEdit = new QLineEdit(fournisseurs);
        searchIdEdit->setObjectName("searchIdEdit");
        searchIdEdit->setGeometry(QRect(460, 200, 161, 42));
        pushButton_7 = new QPushButton(fournisseurs);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(990, 208, 131, 31));
        pushButton_9 = new QPushButton(fournisseurs);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(1000, 130, 121, 29));
        pushButton_2 = new QPushButton(fournisseurs);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(630, 730, 101, 29));
        groupBox = new QGroupBox(fournisseurs);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 150, 271, 621));
        pushButton_15 = new QPushButton(groupBox);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(21, 516, 231, 51));
        pushButton_16 = new QPushButton(groupBox);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(21, 60, 231, 51));
        pushButton_17 = new QPushButton(groupBox);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(21, 136, 231, 51));
        pushButton_20 = new QPushButton(groupBox);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(21, 212, 231, 51));
        pushButton_22 = new QPushButton(groupBox);
        pushButton_22->setObjectName("pushButton_22");
        pushButton_22->setGeometry(QRect(21, 364, 231, 51));
        pushButton_21 = new QPushButton(groupBox);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(21, 288, 231, 51));
        pushButton_23 = new QPushButton(groupBox);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(21, 440, 231, 51));
        label_10->raise();
        pushButton_14->raise();
        pushButton_10->raise();
        pushButton_13->raise();
        label_9->raise();
        searchNomEdit->raise();
        pushButton->raise();
        label_8->raise();
        pushButton_3->raise();
        pushButton_6->raise();
        tableWidget->raise();
        searchIdEdit->raise();
        pushButton_7->raise();
        pushButton_9->raise();
        pushButton_2->raise();
        groupBox->raise();
        label->raise();

        retranslateUi(fournisseurs);

        QMetaObject::connectSlotsByName(fournisseurs);
    } // setupUi

    void retranslateUi(QDialog *fournisseurs)
    {
        fournisseurs->setWindowTitle(QCoreApplication::translate("fournisseurs", "Dialog", nullptr));
        pushButton_14->setText(QCoreApplication::translate("fournisseurs", ">", nullptr));
        pushButton_10->setText(QCoreApplication::translate("fournisseurs", "<", nullptr));
        pushButton_13->setText(QCoreApplication::translate("fournisseurs", "3", nullptr));
        label_9->setText(QCoreApplication::translate("fournisseurs", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:700;\">ID commande :</span></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("fournisseurs", "<html><head/><body><p><span style=\" font-size:11pt; font-weight:696;\">Name</span></p><p><br/></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("fournisseurs", "Ajouter ", nullptr));
        label_8->setText(QCoreApplication::translate("fournisseurs", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700; font-style:italic; text-decoration: underline; color:#550000;\">Gestion des fournisseurs :</span></p></body></html>", nullptr));
        label->setText(QString());
        pushButton_3->setText(QCoreApplication::translate("fournisseurs", "Supprimer ", nullptr));
        pushButton_6->setText(QCoreApplication::translate("fournisseurs", "Actualiser", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("fournisseurs", "Nom fournisseur ", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("fournisseurs", "Type matiere", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("fournisseurs", "Tel\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("fournisseurs", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("fournisseurs", "d\303\251lai de livraison", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("fournisseurs", "Quali\303\251 de mati\303\251re", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("fournisseurs", "Status ", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("fournisseurs", "Nouvelle colonne", nullptr));
        pushButton_7->setText(QCoreApplication::translate("fournisseurs", "Exporter ", nullptr));
        pushButton_9->setText(QCoreApplication::translate("fournisseurs", "Statistiques", nullptr));
        pushButton_2->setText(QCoreApplication::translate("fournisseurs", "Modifier ", nullptr));
        groupBox->setTitle(QString());
        pushButton_15->setText(QCoreApplication::translate("fournisseurs", "quitter", nullptr));
        pushButton_16->setText(QCoreApplication::translate("fournisseurs", "Commandes", nullptr));
        pushButton_17->setText(QCoreApplication::translate("fournisseurs", "Employe", nullptr));
        pushButton_20->setText(QCoreApplication::translate("fournisseurs", "Fournisseur", nullptr));
        pushButton_22->setText(QCoreApplication::translate("fournisseurs", "Matieres Premieres", nullptr));
        pushButton_21->setText(QCoreApplication::translate("fournisseurs", "Products", nullptr));
        pushButton_23->setText(QCoreApplication::translate("fournisseurs", "Machine", nullptr));
    } // retranslateUi

};

namespace Ui {
    class fournisseurs: public Ui_fournisseurs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURNISSEURS_H
