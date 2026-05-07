/********************************************************************************
** Form generated from reading UI file 'fournisseurs.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURNISSEURS_H
#define UI_FOURNISSEURS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fournisseurs
{
public:
    QWidget *centralwidget;
    QGroupBox *sidebarGroup;
    QLabel *sidebarLogo;
    QPushButton *pushButton_15;
    QPushButton *pushButton_21;
    QPushButton *pushButton_20;
    QPushButton *pushButton_fournisseurs_label;
    QPushButton *pushButton_22;
    QPushButton *pushButton_23;
    QPushButton *pushButton_16;
    QLabel *label_8;
    QLabel *label_11;
    QComboBox *comboBox;
    QLabel *label_9;
    QLineEdit *lineEdit;
    QLabel *label_10;
    QComboBox *comboTypeRecherche;
    QPushButton *pushButton_7;
    QPushButton *pushButton_maps;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_recommandation;
    QTableWidget *tableWidget;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *fournisseurs)
    {
        if (fournisseurs->objectName().isEmpty())
            fournisseurs->setObjectName("fournisseurs");
        fournisseurs->resize(1300, 740);
        fournisseurs->setStyleSheet(QString::fromUtf8("\n"
"/* GLOBAL BACKGROUND */\n"
"QMainWindow, QWidget#centralwidget { background-color: #f1e7dc; font-family: \"Segoe UI\"; }\n"
"\n"
"/* TITRE PRINCIPAL */\n"
"QLabel#label_8 { font-size: 20px; font-weight: bold; color: #5b2f1d; letter-spacing: 1px; }\n"
"\n"
"/* LABELS */\n"
"QLabel { color: #3a2a20; font-weight: 600; }\n"
"\n"
"/* INPUTS */\n"
"QLineEdit, QComboBox { background-color: rgba(255, 250, 245, 0.6); border: 1px solid #c6a88d; border-radius: 6px; padding: 6px; color: #3a2a20; }\n"
"QLineEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; background-color: #fffaf5; }\n"
"\n"
"/* TABLEAU STYLE LEATHER */\n"
"QTableWidget { background-color: #fffaf5; border: 2px solid #b08a6b; border-radius: 14px; gridline-color: #e0d2c5; }\n"
"QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6b3e26, stop:1 #4a2717); color: #fffaf5; padding: 8px; border: none; font-weight: bold; }\n"
"QTableWidget::item:selected { background-color: #d8b59c; color: #2a1a12; }\n"
"\n"
"/* BO"
                        "UTONS STANDARDS (Buns Marron) */\n"
"QPushButton { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a4a2e, stop:1 #5b2f1d); border: 2px solid #3a1f14; border-radius: 10px; padding: 8px 12px; font-weight: bold; color: #fffaf5; }\n"
"QPushButton:hover { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5a3a, stop:1 #6b3e26); }\n"
"\n"
"/* BOUTONS ACCENTU\303\211S (Couleurs exactes de la page Machine) */\n"
"QPushButton#pushButton_3 { background-color: #6f8f3d; border-color: #4c6328; } /* Ajouter (Vert) */\n"
"QPushButton#pushButton_4 { background-color: #d07a2d; border-color: #a35d1e; } /* Modifier (Orange) */\n"
"QPushButton#pushButton_5 { background-color: #a23b2a; border-color: #6e2418; } /* Supprimer (Rouge) */\n"
"QPushButton#pushButton_6 { background-color: #7a4a2e; border-color: #3a1f14; } /* Actualiser (Marron) */\n"
"\n"
"/* PAGINATION */\n"
"QPushButton#pushButton_10, QPushButton#pushButton_11, QPushButton#pushButton_12, QPushButton#pushButton_13, QPushButton#pushBut"
                        "ton_14 { background-color: #a47148; border-color: #6b3e26; }\n"
"   "));
        centralwidget = new QWidget(fournisseurs);
        centralwidget->setObjectName("centralwidget");
        sidebarGroup = new QGroupBox(centralwidget);
        sidebarGroup->setObjectName("sidebarGroup");
        sidebarGroup->setGeometry(QRect(0, 0, 210, 740));
        sidebarGroup->setStyleSheet(QString::fromUtf8("\n"
"QGroupBox { background-color: #3d2512; border: none; border-radius: 0px; margin:0; padding:0; }\n"
"QPushButton { background-color: transparent; border: none; color: #d4c4b0; font-size: 13px; font-weight: 600; text-align: left; padding: 14px 20px; border-radius: 0px; }\n"
"QPushButton:hover { background-color: rgba(255,255,255,0.05); color: #ffffff; }\n"
"QPushButton#pushButton_fournisseurs_label { background-color: #4e3422; color: #ffffff; border-left: 4px solid #b08a6b; }\n"
"     "));
        sidebarLogo = new QLabel(sidebarGroup);
        sidebarLogo->setObjectName("sidebarLogo");
        sidebarLogo->setGeometry(QRect(15, 15, 180, 90));
        sidebarLogo->setPixmap(QPixmap(QString::fromUtf8(":/Logo.png")));
        sidebarLogo->setScaledContents(true);
        pushButton_15 = new QPushButton(sidebarGroup);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(0, 120, 210, 50));
        pushButton_21 = new QPushButton(sidebarGroup);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(0, 175, 210, 50));
        pushButton_20 = new QPushButton(sidebarGroup);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(0, 230, 210, 50));
        pushButton_fournisseurs_label = new QPushButton(sidebarGroup);
        pushButton_fournisseurs_label->setObjectName("pushButton_fournisseurs_label");
        pushButton_fournisseurs_label->setGeometry(QRect(0, 285, 210, 50));
        pushButton_22 = new QPushButton(sidebarGroup);
        pushButton_22->setObjectName("pushButton_22");
        pushButton_22->setGeometry(QRect(0, 340, 210, 50));
        pushButton_23 = new QPushButton(sidebarGroup);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(0, 395, 210, 50));
        pushButton_16 = new QPushButton(sidebarGroup);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(0, 640, 210, 50));
        pushButton_16->setStyleSheet(QString::fromUtf8("color: #e0a0a0;"));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(350, 30, 400, 50));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(240, 125, 80, 30));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(310, 125, 160, 32));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(490, 125, 110, 30));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(600, 125, 100, 32));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(720, 125, 110, 30));
        comboTypeRecherche = new QComboBox(centralwidget);
        comboTypeRecherche->setObjectName("comboTypeRecherche");
        comboTypeRecherche->setGeometry(QRect(820, 125, 130, 32));
        pushButton_7 = new QPushButton(centralwidget);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(1060, 125, 110, 35));
        pushButton_maps = new QPushButton(centralwidget);
        pushButton_maps->setObjectName("pushButton_maps");
        pushButton_maps->setGeometry(QRect(1020, 20, 130, 38));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(1160, 20, 120, 38));
        pushButton_9 = new QPushButton(centralwidget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(1160, 65, 120, 38));
        pushButton_recommandation = new QPushButton(centralwidget);
        pushButton_recommandation->setObjectName("pushButton_recommandation");
        pushButton_recommandation->setGeometry(QRect(1020, 65, 130, 38));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 12)
            tableWidget->setColumnCount(12);
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
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(240, 170, 1030, 370));
        pushButton_10 = new QPushButton(centralwidget);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(650, 555, 40, 32));
        pushButton_11 = new QPushButton(centralwidget);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(700, 555, 40, 32));
        pushButton_12 = new QPushButton(centralwidget);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(750, 555, 40, 32));
        pushButton_13 = new QPushButton(centralwidget);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(800, 555, 40, 32));
        pushButton_14 = new QPushButton(centralwidget);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(850, 555, 40, 32));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(400, 610, 120, 42));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(540, 610, 120, 42));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(680, 610, 120, 42));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(820, 610, 120, 42));
        fournisseurs->setCentralWidget(centralwidget);
        menubar = new QMenuBar(fournisseurs);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1300, 26));
        fournisseurs->setMenuBar(menubar);
        statusbar = new QStatusBar(fournisseurs);
        statusbar->setObjectName("statusbar");
        fournisseurs->setStatusBar(statusbar);

        retranslateUi(fournisseurs);

        QMetaObject::connectSlotsByName(fournisseurs);
    } // setupUi

    void retranslateUi(QMainWindow *fournisseurs)
    {
        fournisseurs->setWindowTitle(QCoreApplication::translate("fournisseurs", "Gestion des Fournisseurs", nullptr));
        sidebarGroup->setTitle(QString());
        pushButton_15->setText(QCoreApplication::translate("fournisseurs", "Employ\303\251s", nullptr));
        pushButton_21->setText(QCoreApplication::translate("fournisseurs", "Produits", nullptr));
        pushButton_20->setText(QCoreApplication::translate("fournisseurs", "Commandes", nullptr));
        pushButton_fournisseurs_label->setText(QCoreApplication::translate("fournisseurs", "Fournisseurs", nullptr));
        pushButton_22->setText(QCoreApplication::translate("fournisseurs", "Mati\303\250res", nullptr));
        pushButton_23->setText(QCoreApplication::translate("fournisseurs", "Machines", nullptr));
        pushButton_16->setText(QCoreApplication::translate("fournisseurs", "D\303\251connexion", nullptr));
        label_8->setText(QCoreApplication::translate("fournisseurs", "Gestion des fournisseurs :", nullptr));
        label_11->setText(QCoreApplication::translate("fournisseurs", "Trier par:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("fournisseurs", "Aucun Tri", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("fournisseurs", "D\303\251lai croissant", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("fournisseurs", "D\303\251lai d\303\251croissant", nullptr));

        label_9->setText(QCoreApplication::translate("fournisseurs", "ID fournisseur :", nullptr));
        label_10->setText(QCoreApplication::translate("fournisseurs", "Type mati\303\250re:", nullptr));
        pushButton_7->setText(QCoreApplication::translate("fournisseurs", "Recherche", nullptr));
        pushButton_maps->setText(QCoreApplication::translate("fournisseurs", "Maps", nullptr));
        pushButton_8->setText(QCoreApplication::translate("fournisseurs", "Exporter PDF", nullptr));
        pushButton_9->setText(QCoreApplication::translate("fournisseurs", "Statistiques", nullptr));
        pushButton_recommandation->setText(QCoreApplication::translate("fournisseurs", "Recommandation", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("fournisseurs", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("fournisseurs", "Ref", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("fournisseurs", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("fournisseurs", "Matiere", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("fournisseurs", "Telephone", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("fournisseurs", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("fournisseurs", "Delai", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("fournisseurs", "Qualite", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("fournisseurs", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("fournisseurs", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("fournisseurs", "Capacite", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("fournisseurs", "Fiabilite", nullptr));
        pushButton_10->setText(QCoreApplication::translate("fournisseurs", "<", nullptr));
        pushButton_11->setText(QCoreApplication::translate("fournisseurs", "1", nullptr));
        pushButton_12->setText(QCoreApplication::translate("fournisseurs", "2", nullptr));
        pushButton_13->setText(QCoreApplication::translate("fournisseurs", "3", nullptr));
        pushButton_14->setText(QCoreApplication::translate("fournisseurs", ">", nullptr));
        pushButton_3->setText(QCoreApplication::translate("fournisseurs", "Ajouter", nullptr));
        pushButton_4->setText(QCoreApplication::translate("fournisseurs", "Modifier", nullptr));
        pushButton_5->setText(QCoreApplication::translate("fournisseurs", "Supprimer", nullptr));
        pushButton_6->setText(QCoreApplication::translate("fournisseurs", "Actualiser", nullptr));
    } // retranslateUi

};

namespace Ui {
    class fournisseurs: public Ui_fournisseurs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURNISSEURS_H
