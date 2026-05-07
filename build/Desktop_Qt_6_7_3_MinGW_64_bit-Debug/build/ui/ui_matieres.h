/********************************************************************************
** Form generated from reading UI file 'matieres.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Matieres
{
public:
    QWidget *centralwidget;
    QGroupBox *sidebarGroup;
    QLabel *sidebarLogo;
    QPushButton *pushButton_11;
    QPushButton *pushButton_21;
    QPushButton *pushButton_6;
    QPushButton *pushButton_20;
    QPushButton *pushButton_22;
    QPushButton *pushButton_23;
    QPushButton *pushButton_5;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *searchIdEdit;
    QLabel *label_10;
    QLineEdit *searchTypeEdit;
    QPushButton *pushButton_10;
    QPushButton *btnAlertesStock;
    QPushButton *pushButton_7;
    QPushButton *btnAI;
    QPushButton *pushButton_9;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QDialog *Matieres)
    {
        if (Matieres->objectName().isEmpty())
            Matieres->setObjectName("Matieres");
        Matieres->resize(1300, 740);
        Matieres->setStyleSheet(QString::fromUtf8("\n"
"QDialog { background-color: #f1e7dc; font-family: \"Segoe UI\"; }\n"
"QLabel#label_8 { font-size: 20px; font-weight: bold; color: #5b2f1d; letter-spacing: 1px; }\n"
"QLabel { color: #3a2a20; font-weight: 600; }\n"
"QLineEdit, QComboBox { background-color: rgba(255, 250, 245, 0.6); border: 1px solid #c6a88d; border-radius: 6px; padding: 6px; color: #3a2a20; }\n"
"QLineEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; background-color: #fffaf5; }\n"
"QTableWidget { background-color: #fffaf5; border: 2px solid #b08a6b; border-radius: 14px; gridline-color: #e0d2c5; }\n"
"QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6b3e26, stop:1 #4a2717); color: #fffaf5; padding: 8px; border: none; font-weight: bold; }\n"
"QTableWidget::item:selected { background-color: #d8b59c; color: #2a1a12; }\n"
"QPushButton { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a4a2e, stop:1 #5b2f1d); border: 2px solid #3a1f14; border-radius: 10px; padding: 8px 12px; font-"
                        "weight: bold; color: #fffaf5; }\n"
"QPushButton:hover { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5a3a, stop:1 #6b3e26); }\n"
"QPushButton#pushButton { background-color: #6f8f3d; border-color: #4c6328; }\n"
"QPushButton#pushButton_2 { background-color: #d07a2d; border-color: #a35d1e; }\n"
"QPushButton#pushButton_3 { background-color: #a23b2a; border-color: #6e2418; }\n"
"QPushButton#btnAI { background-color: #4c6328; border-color: #34451c; }\n"
"QPushButton#btnAlertesStock { background-color: #a23b2a; border-color: #6e2418; }\n"
"   "));
        centralwidget = new QWidget(Matieres);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(0, 0, 1300, 740));
        sidebarGroup = new QGroupBox(centralwidget);
        sidebarGroup->setObjectName("sidebarGroup");
        sidebarGroup->setGeometry(QRect(0, 0, 210, 740));
        sidebarGroup->setStyleSheet(QString::fromUtf8("\n"
"QGroupBox { background-color: #3d2512; border: none; border-radius: 0px; }\n"
"QPushButton { background-color: transparent; border: none; color: #d4c4b0; font-size: 13px; font-weight: 600; text-align: left; padding: 14px 20px; }\n"
"QPushButton:hover { background-color: rgba(255,255,255,0.05); color: #ffffff; }\n"
"QPushButton#pushButton_22 { background-color: #4e3422; color: #ffffff; border-left: 4px solid #b08a6b; }\n"
"     "));
        sidebarLogo = new QLabel(sidebarGroup);
        sidebarLogo->setObjectName("sidebarLogo");
        sidebarLogo->setGeometry(QRect(15, 15, 180, 90));
        sidebarLogo->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        sidebarLogo->setScaledContents(true);
        pushButton_11 = new QPushButton(sidebarGroup);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(0, 120, 210, 50));
        pushButton_21 = new QPushButton(sidebarGroup);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(0, 175, 210, 50));
        pushButton_6 = new QPushButton(sidebarGroup);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(0, 230, 210, 50));
        pushButton_20 = new QPushButton(sidebarGroup);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(0, 285, 210, 50));
        pushButton_22 = new QPushButton(sidebarGroup);
        pushButton_22->setObjectName("pushButton_22");
        pushButton_22->setGeometry(QRect(0, 340, 210, 50));
        pushButton_23 = new QPushButton(sidebarGroup);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(0, 395, 210, 50));
        pushButton_5 = new QPushButton(sidebarGroup);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(0, 640, 210, 50));
        pushButton_5->setStyleSheet(QString::fromUtf8("color: #e0a0a0;"));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(350, 30, 450, 50));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(240, 115, 85, 30));
        searchIdEdit = new QLineEdit(centralwidget);
        searchIdEdit->setObjectName("searchIdEdit");
        searchIdEdit->setGeometry(QRect(325, 115, 90, 32));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(425, 115, 45, 30));
        searchTypeEdit = new QLineEdit(centralwidget);
        searchTypeEdit->setObjectName("searchTypeEdit");
        searchTypeEdit->setGeometry(QRect(470, 115, 90, 32));
        pushButton_10 = new QPushButton(centralwidget);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(570, 115, 100, 32));
        btnAlertesStock = new QPushButton(centralwidget);
        btnAlertesStock->setObjectName("btnAlertesStock");
        btnAlertesStock->setGeometry(QRect(1020, 20, 120, 38));
        pushButton_7 = new QPushButton(centralwidget);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(1160, 20, 120, 38));
        btnAI = new QPushButton(centralwidget);
        btnAI->setObjectName("btnAI");
        btnAI->setGeometry(QRect(1020, 65, 120, 38));
        pushButton_9 = new QPushButton(centralwidget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(1160, 65, 120, 38));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
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
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(240, 170, 1030, 420));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(400, 620, 120, 45));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(540, 620, 120, 45));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(680, 620, 120, 45));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(820, 620, 120, 45));

        retranslateUi(Matieres);

        QMetaObject::connectSlotsByName(Matieres);
    } // setupUi

    void retranslateUi(QDialog *Matieres)
    {
        Matieres->setWindowTitle(QCoreApplication::translate("Matieres", "Gestion des Mati\303\250res Premi\303\250res", nullptr));
        pushButton_11->setText(QCoreApplication::translate("Matieres", "Employ\303\251s", nullptr));
        pushButton_21->setText(QCoreApplication::translate("Matieres", "Produits", nullptr));
        pushButton_6->setText(QCoreApplication::translate("Matieres", "Commandes", nullptr));
        pushButton_20->setText(QCoreApplication::translate("Matieres", "Fournisseurs", nullptr));
        pushButton_22->setText(QCoreApplication::translate("Matieres", "Mati\303\250res", nullptr));
        pushButton_23->setText(QCoreApplication::translate("Matieres", "Machines", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Matieres", "D\303\251connexion", nullptr));
        label_8->setText(QCoreApplication::translate("Matieres", "Gestion des mati\303\250res premi\303\250res :", nullptr));
        label_9->setText(QCoreApplication::translate("Matieres", "ID Mati\303\250re :", nullptr));
        label_10->setText(QCoreApplication::translate("Matieres", "Type :", nullptr));
        pushButton_10->setText(QCoreApplication::translate("Matieres", "Recherche", nullptr));
        btnAlertesStock->setText(QCoreApplication::translate("Matieres", "ALERTES", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Matieres", "Exporter", nullptr));
        btnAI->setText(QCoreApplication::translate("Matieres", "Contr\303\264le IA", nullptr));
        pushButton_9->setText(QCoreApplication::translate("Matieres", "Statistiques", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Matieres", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Matieres", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Matieres", "Couleur", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Matieres", "Qualit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Matieres", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Matieres", "Stock", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Matieres", "Fournisseur", nullptr));
        pushButton->setText(QCoreApplication::translate("Matieres", "Ajouter", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Matieres", "Modifier", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Matieres", "Supprimer", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Matieres", "Actualiser", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Matieres: public Ui_Matieres {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATIERES_H
