/********************************************************************************
** Form generated from reading UI file 'matieres.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATIERES_H
#define UI_MATIERES_H

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

class Ui_Matieres
{
public:
    QLineEdit *searchIdEdit;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_10;
    QLineEdit *searchTypeEdit;
    QGroupBox *groupBox;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_11;
    QPushButton *pushButton_20;
    QPushButton *pushButton_22;
    QPushButton *pushButton_21;
    QPushButton *pushButton_23;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_10;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QLabel *label;
    QTableWidget *tableWidget;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *Matieres)
    {
        if (Matieres->objectName().isEmpty())
            Matieres->setObjectName("Matieres");
        Matieres->resize(1342, 781);
        Matieres->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
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
        searchIdEdit = new QLineEdit(Matieres);
        searchIdEdit->setObjectName("searchIdEdit");
        searchIdEdit->setGeometry(QRect(460, 160, 131, 31));
        label_9 = new QLabel(Matieres);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(310, 160, 141, 31));
        label_8 = new QLabel(Matieres);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(470, 70, 471, 81));
        label_10 = new QLabel(Matieres);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(650, 160, 171, 31));
        searchTypeEdit = new QLineEdit(Matieres);
        searchTypeEdit->setObjectName("searchTypeEdit");
        searchTypeEdit->setGeometry(QRect(820, 160, 131, 31));
        groupBox = new QGroupBox(Matieres);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(30, 150, 241, 601));
        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(11, 492, 221, 51));
        pushButton_6 = new QPushButton(groupBox);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(11, 48, 221, 51));
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(11, 122, 221, 51));
        pushButton_20 = new QPushButton(groupBox);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(11, 196, 221, 51));
        pushButton_22 = new QPushButton(groupBox);
        pushButton_22->setObjectName("pushButton_22");
        pushButton_22->setGeometry(QRect(11, 344, 221, 51));
        pushButton_21 = new QPushButton(groupBox);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(11, 270, 221, 51));
        pushButton_23 = new QPushButton(groupBox);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(11, 418, 221, 51));
        groupBox_2 = new QGroupBox(Matieres);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(-10, -20, 1541, 921));
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
        pushButton_10 = new QPushButton(groupBox_2);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(990, 178, 141, 41));
        pushButton_7 = new QPushButton(groupBox_2);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(1150, 178, 161, 41));
        pushButton_9 = new QPushButton(groupBox_2);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(1090, 120, 131, 41));
        label = new QLabel(groupBox_2);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 60, 211, 121));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        label->setScaledContents(true);
        tableWidget = new QTableWidget(groupBox_2);
        if (tableWidget->columnCount() < 8)
            tableWidget->setColumnCount(8);
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
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(300, 230, 1051, 491));
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
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(920, 730, 101, 41));
        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(760, 730, 101, 41));
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(590, 730, 101, 41));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(440, 730, 101, 41));
        groupBox_2->raise();
        searchIdEdit->raise();
        label_9->raise();
        label_8->raise();
        label_10->raise();
        searchTypeEdit->raise();
        groupBox->raise();

        retranslateUi(Matieres);

        QMetaObject::connectSlotsByName(Matieres);
    } // setupUi

    void retranslateUi(QDialog *Matieres)
    {
        Matieres->setWindowTitle(QCoreApplication::translate("Matieres", "Dialog", nullptr));
        label_9->setText(QCoreApplication::translate("Matieres", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:700;\">ID Cuir:</span></p><p><br/></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("Matieres", "<html><head/><body><p><span style=\" font-size:16pt; font-style:italic; text-decoration: underline;\">Gestion des Mati\303\251res Premi\303\251res :</span></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("Matieres", "<html><head/><body><p>Type</p></body></html>", nullptr));
        groupBox->setTitle(QString());
        pushButton_5->setText(QCoreApplication::translate("Matieres", "quitter", nullptr));
        pushButton_6->setText(QCoreApplication::translate("Matieres", "Commandes", nullptr));
        pushButton_11->setText(QCoreApplication::translate("Matieres", "Employe", nullptr));
        pushButton_20->setText(QCoreApplication::translate("Matieres", "Fournisseur", nullptr));
        pushButton_22->setText(QCoreApplication::translate("Matieres", "Matieres Premieres", nullptr));
        pushButton_21->setText(QCoreApplication::translate("Matieres", "Products", nullptr));
        pushButton_23->setText(QCoreApplication::translate("Matieres", "Machine", nullptr));
        groupBox_2->setTitle(QString());
        pushButton_10->setText(QCoreApplication::translate("Matieres", "Recherche", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Matieres", "Exporter", nullptr));
        pushButton_9->setText(QCoreApplication::translate("Matieres", "Statistiques", nullptr));
        label->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Matieres", "id_cuir", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Matieres", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Matieres", "qualit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Matieres", "couleur", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Matieres", "quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Matieres", "prix_unitaire", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Matieres", "Actualiser", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Matieres", "Supprimer", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Matieres", "Modifier", nullptr));
        pushButton->setText(QCoreApplication::translate("Matieres", "Ajouter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Matieres: public Ui_Matieres {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATIERES_H
