/********************************************************************************
** Form generated from reading UI file 'commandes.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDES_H
#define UI_COMMANDES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_commandes
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QTableWidget *tableWidget;
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
    QWidget *page_2;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *commandes)
    {
        if (commandes->objectName().isEmpty())
            commandes->setObjectName("commandes");
        commandes->resize(1740, 716);
        commandes->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
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
        centralwidget = new QWidget(commandes);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(260, 20, 1461, 691));
        page = new QWidget();
        page->setObjectName("page");
        tableWidget = new QTableWidget(page);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        __qtablewidgetitem->setForeground(brush);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(0, 170, 1021, 411));
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
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
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
        pushButton_7->setGeometry(QRect(780, 60, 141, 29));
        label_8 = new QLabel(page);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(70, 50, 381, 81));
        pushButton_8 = new QPushButton(page);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(180, 520, 52, 41));
        pushButton_13 = new QPushButton(page);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(320, 520, 52, 41));
        pushButton_14 = new QPushButton(page);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(250, 520, 52, 41));
        pushButton_15 = new QPushButton(page);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(450, 520, 52, 41));
        pushButton_16 = new QPushButton(page);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(390, 520, 52, 41));
        pushButton_9 = new QPushButton(page);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(950, 60, 131, 31));
        pushButton_10 = new QPushButton(page);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(800, 130, 111, 29));
        label_9 = new QLabel(page);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 130, 141, 31));
        lineEdit_5 = new QLineEdit(page);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(180, 130, 131, 31));
        lineEdit_6 = new QLineEdit(page);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(540, 130, 131, 31));
        label_10 = new QLabel(page);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(370, 130, 171, 31));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 20, 211, 121));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        label->setScaledContents(true);
        commandes->setCentralWidget(centralwidget);
        menubar = new QMenuBar(commandes);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1740, 22));
        commandes->setMenuBar(menubar);
        statusbar = new QStatusBar(commandes);
        statusbar->setObjectName("statusbar");
        commandes->setStatusBar(statusbar);

        retranslateUi(commandes);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(commandes);
    } // setupUi

    void retranslateUi(QMainWindow *commandes)
    {
        commandes->setWindowTitle(QCoreApplication::translate("commandes", "commandes", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("commandes", "id_produit", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("commandes", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("commandes", "cat\303\251gorie", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("commandes", "date de creation", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("commandes", "date fin creation", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("commandes", "statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("commandes", "prix", nullptr));
        pushButton->setText(QCoreApplication::translate("commandes", "Ajouter", nullptr));
        pushButton_2->setText(QCoreApplication::translate("commandes", "Modifier", nullptr));
        pushButton_3->setText(QCoreApplication::translate("commandes", "Supprimer", nullptr));
        pushButton_4->setText(QCoreApplication::translate("commandes", "Actualiser", nullptr));
        pushButton_7->setText(QCoreApplication::translate("commandes", "Exporter PDF", nullptr));
        label_8->setText(QCoreApplication::translate("commandes", "<html><head/><body><p><span style=\" font-size:16pt; font-style:italic; text-decoration: underline;\">Gestion des produits :</span></p></body></html>", nullptr));
        pushButton_8->setText(QCoreApplication::translate("commandes", "<", nullptr));
        pushButton_13->setText(QCoreApplication::translate("commandes", "2", nullptr));
        pushButton_14->setText(QCoreApplication::translate("commandes", "1", nullptr));
        pushButton_15->setText(QCoreApplication::translate("commandes", ">", nullptr));
        pushButton_16->setText(QCoreApplication::translate("commandes", "3", nullptr));
        pushButton_9->setText(QCoreApplication::translate("commandes", "Statistiques", nullptr));
        pushButton_10->setText(QCoreApplication::translate("commandes", "Recherche", nullptr));
        label_9->setText(QCoreApplication::translate("commandes", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">Nom produit :</span></p><p><br/></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("commandes", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">prix du produit :</span></p><p><br/></p></body></html>", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class commandes: public Ui_commandes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDES_H
