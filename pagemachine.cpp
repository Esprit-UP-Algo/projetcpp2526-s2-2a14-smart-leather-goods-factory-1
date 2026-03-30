#include "pagemachine.h"
#include "ui_pagemachine.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include "commandes.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include "smtp.h"


pagemachine::pagemachine(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagemachine)
{
    ui->setupUi(this);
    setupMachinesTable();
    loadMachines();
    setupSearch();
}

pagemachine::~pagemachine()
{
    delete ui;
}

void pagemachine::setupSearch()
{
    ui->searchIdEdit->setPlaceholderText("Chercher par État...");
    ui->searchNomEdit->setPlaceholderText("Chercher par Réf...");
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
}

void pagemachine::filterTable()
{
    QString etatFilter = ui->searchIdEdit->text();
    QString refFilter = ui->searchNomEdit->text();

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;

        if (!etatFilter.isEmpty()) {
            QString etat = ui->tableWidget->item(row, 4)->text();
            if (!etat.contains(etatFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        if (match && !refFilter.isEmpty()) {
            QString ref = ui->tableWidget->item(row, 1)->text();
            if (!ref.contains(refFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

void pagemachine::setupMachinesTable()
{
    QStringList headers = {"ID Machine", "Référence", "Nom Machine", "Type Machine", "État Machine",
                           "Capacité", "Fréquence", "Niveau de Charge"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 130);
    ui->tableWidget->setColumnWidth(3, 130);
    ui->tableWidget->setColumnWidth(4, 110);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 100);
    ui->tableWidget->setColumnWidth(7, 120);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void pagemachine::loadMachines()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE FROM MACHINE");
    
    if (query.exec()) {
        while (query.next()) {
            addMachineToTable(
                query.value(0).toString(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString(),
                query.value(7).toString()
            );
        }
    } else {
        qDebug() << "Erreur de chargement des machines: " << query.lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les machines depuis la base de données.");
    }
}

void pagemachine::addMachineToTable(const QString &id, const QString &ref, const QString &nom,
                                    const QString &type, const QString &etat,
                                    const QString &capacite, const QString &frequence,
                                    const QString &niveauCharge)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ref));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(nom));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(etat));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(capacite));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(frequence));
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(niveauCharge + "%"));

    updateRowColors(row);
}

void pagemachine::updateRowColors(int row)
{
    QTableWidgetItem *etatItem = ui->tableWidget->item(row, 4);
    if (etatItem) {
        QString etat = etatItem->text();
        if (etat == "Fonctionnelle") {
            etatItem->setBackground(QColor(200, 255, 200));
            etatItem->setForeground(QColor(0, 100, 0));
        } else if (etat == "En panne") {
            etatItem->setBackground(QColor(255, 200, 200));
            etatItem->setForeground(QColor(139, 0, 0));
        } else if (etat == "Maintenance") {
            etatItem->setBackground(QColor(255, 255, 200));
            etatItem->setForeground(QColor(128, 128, 0));
        } else if (etat == "Arrêtée") {
            etatItem->setBackground(QColor(200, 200, 200));
            etatItem->setForeground(QColor(70, 70, 70));
        }
    }

    QTableWidgetItem *chargeItem = ui->tableWidget->item(row, 7);
    if (chargeItem) {
        QString chargeStr = chargeItem->text().replace("%", "");
        int charge = chargeStr.toInt();
        if (charge > 80) {
            chargeItem->setBackground(QColor(255, 200, 200));
        } else if (charge > 50) {
            chargeItem->setBackground(QColor(255, 255, 200));
        } else {
            chargeItem->setBackground(QColor(200, 255, 200));
        }
    }
}

void pagemachine::updateMachineInTable(int row, const QString &ref, const QString &nom,
                                       const QString &type, const QString &etat,
                                       const QString &capacite, const QString &frequence,
                                       const QString &niveauCharge)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(ref);
        ui->tableWidget->item(row, 2)->setText(nom);
        ui->tableWidget->item(row, 3)->setText(type);
        ui->tableWidget->item(row, 4)->setText(etat);
        ui->tableWidget->item(row, 5)->setText(capacite);
        ui->tableWidget->item(row, 6)->setText(frequence);
        ui->tableWidget->item(row, 7)->setText(niveauCharge + "%");

        updateRowColors(row);
    }
}

void pagemachine::on_pushButton_6_clicked()
{
    hide();
    commandes *lg = new commandes(this);
    lg->show();
}

void pagemachine::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
    pl->show();
}

void pagemachine::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void pagemachine::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void pagemachine::on_pushButton_22_clicked()
{
    hide();
    Matieres *mm = new Matieres(this);
    mm->show();
}

void pagemachine::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(this);
    ss->show();
}

void pagemachine::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Ajouter une machine");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setModal(true);

    ajoutDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   margin-bottom: 15px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QLineEdit, QComboBox, QSpinBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 15px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "   font-size: 15px;"
        "   min-width: 150px;"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("AJOUTER UNE MACHINE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID machine (ex: M001)");
    idEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("Référence");
    refEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom de la machine");
    nomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});

    QLineEdit *capaciteEdit = new QLineEdit();
    capaciteEdit->setPlaceholderText("Capacité (ex: 1000 unités/h)");
    capaciteEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *frequenceEdit = new QLineEdit();
    frequenceEdit->setPlaceholderText("Fréquence (ex: 50 Hz)");
    frequenceEdit->setAlignment(Qt::AlignCenter);

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setValue(50);
    chargeSpinBox->setSuffix(" %");
    chargeSpinBox->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel("🔑 ID Machine :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📌 Référence :"));
    mainLayout->addWidget(refEdit);
    mainLayout->addWidget(new QLabel("🏭 Nom Machine :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("⚙️ Type Machine :"));
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(new QLabel("📊 État Machine :"));
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(new QLabel("📦 Capacité :"));
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(new QLabel("📈 Fréquence :"));
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(new QLabel("🔋 Niveau de Charge :"));
    mainLayout->addWidget(chargeSpinBox);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, refEdit, nomEdit, typeCombo,
                                              etatCombo, capaciteEdit, frequenceEdit,
                                              chargeSpinBox, this]() {
        if (idEdit->text().isEmpty() || refEdit->text().isEmpty() || nomEdit->text().isEmpty() ||
            capaciteEdit->text().isEmpty() || frequenceEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                               "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO MACHINE (ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE) "
                      "VALUES (:id, :ref, :nom, :type, :etat, :capacite, :frequence, :charge)");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":ref", refEdit->text());
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":capacite", capaciteEdit->text());
        query.bindValue(":frequence", frequenceEdit->text());
        query.bindValue(":charge", QString::number(chargeSpinBox->value()));

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ La machine a été ajoutée avec succès!");
            QString etatMachine = etatCombo->currentText();
            if (etatMachine == "En panne") {
                Smtp* smtp = new Smtp("Dkhileya5@gmail.com", "fstqthkaprwianwt", "smtp.gmail.com", 465);
                QString subject = "Alert: Machine ajoutee en panne";
                QString body = "<html><body><h2>Alerte: Machine en panne</h2>"
                               "<p>La machine <b>" + nomEdit->text() + "</b> (Ref: " + refEdit->text() + ") "
                               "a ete declaree en panne des son ajout.</p></body></html>";
                smtp->sendMail("Dkhileya5@gmail.com", "eyadkhil@aiesec.net", subject, body);
            }
            loadMachines(); // Refresh the table
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void pagemachine::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString etat = ui->tableWidget->item(currentRow, 4)->text();
    QString capacite = ui->tableWidget->item(currentRow, 5)->text();
    QString frequence = ui->tableWidget->item(currentRow, 6)->text();
    QString niveauCharge = ui->tableWidget->item(currentRow, 7)->text().replace("%", "");

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier machine");
    modifierDialog->setFixedSize(650, 800);
    modifierDialog->setModal(true);

    modifierDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   margin-bottom: 15px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QLineEdit, QComboBox, QSpinBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 15px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "   font-size: 15px;"
        "   min-width: 150px;"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(modifierDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("MODIFIER MACHINE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(id);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setText(ref);
    refEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setText(nom);
    nomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});
    typeCombo->setCurrentText(type);

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});
    etatCombo->setCurrentText(etat);

    QLineEdit *capaciteEdit = new QLineEdit();
    capaciteEdit->setText(capacite);
    capaciteEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *frequenceEdit = new QLineEdit();
    frequenceEdit->setText(frequence);
    frequenceEdit->setAlignment(Qt::AlignCenter);

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setValue(niveauCharge.toInt());
    chargeSpinBox->setSuffix(" %");
    chargeSpinBox->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel("🔑 ID Machine :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📌 Référence :"));
    mainLayout->addWidget(refEdit);
    mainLayout->addWidget(new QLabel("🏭 Nom Machine :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("⚙️ Type Machine :"));
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(new QLabel("📊 État Machine :"));
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(new QLabel("📦 Capacité :"));
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(new QLabel("📈 Fréquence :"));
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(new QLabel("🔋 Niveau de Charge :"));
    mainLayout->addWidget(chargeSpinBox);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [modifierDialog, id, refEdit, nomEdit, typeCombo,
                                              etatCombo, capaciteEdit, frequenceEdit,
                                              chargeSpinBox, this]() {
        if (refEdit->text().isEmpty() || nomEdit->text().isEmpty() || capaciteEdit->text().isEmpty() || frequenceEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants",
                               "Veuillez remplir tous les champs.");
            return;
        }

        QSqlQuery query;
        query.prepare("UPDATE MACHINE SET REF = :ref, NOM = :nom, TYPE = :type, ETAT = :etat, "
                      "CAPACITE = :capacite, FREQUENCE = :frequence, NIVEAU_DE_CHARGE = :charge "
                      "WHERE ID_MACHINE = :id");
        query.bindValue(":ref", refEdit->text());
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":capacite", capaciteEdit->text());
        query.bindValue(":frequence", frequenceEdit->text());
        query.bindValue(":charge", QString::number(chargeSpinBox->value()));
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ La machine a été mise à jour avec succès!");
            QString etatMachine = etatCombo->currentText();
            if (etatMachine == "En panne") {
                Smtp* smtp = new Smtp("Dkhileya5@gmail.com", "fstqthkaprwianwt", "smtp.gmail.com", 465);
                QString subject = "Alerte Panne Machine";
                QString body = "<html><body><h2>Alerte: Machine en panne</h2>"
                               "<p>La machine <b>" + nomEdit->text() + "</b> (Ref: " + refEdit->text() + ") "
                               "vient d'etre declaree en panne suite a une modification.</p></body></html>";
                smtp->sendMail("Dkhileya5@gmail.com", "eyadkhil@aiesec.net", subject, body);
            }
            loadMachines();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
    modifierDialog->deleteLater();
}

void pagemachine::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString nom = ui->tableWidget->item(currentRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer la machine '%1' (ID: %2) ?")
                                 .arg(nom).arg(id),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM MACHINE WHERE ID_MACHINE = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Machine supprimée avec succès!");
            loadMachines();
            qDebug() << "Machine supprimée:" << id << "-" << nom;
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        }
    }
}

void pagemachine::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNomEdit->clear();
    loadMachines();
    QMessageBox::information(this, "Actualisation", "✅ Liste des machines actualisée !");
}

void pagemachine::on_pushButton_7_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine dans le tableau pour générer la demande de maintenance.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString etat = ui->tableWidget->item(currentRow, 4)->text();

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Demande de Maintenance",
                                                    "Demande_Maintenance_" + ref + ".pdf",
                                                    "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument doc;
    QString html = "<h1>Demande de Maintenance / Diagnostique</h1>"
                   "<hr>"
                   "<h3>Détails de la machine</h3>"
                   "<table border='1' cellspacing='0' cellpadding='5' width='100%'>"
                   "<tr><th align='left'>ID Machine</th><td>" + id + "</td></tr>"
                   "<tr><th align='left'>Référence</th><td>" + ref + "</td></tr>"
                   "<tr><th align='left'>Nom de la Machine</th><td>" + nom + "</td></tr>"
                   "<tr><th align='left'>Type</th><td>" + type + "</td></tr>"
                   "<tr><th align='left'>État Actuel</th><td style='color:red;'><b>" + etat + "</b></td></tr>"
                   "</table>"
                   "<br><br>"
                   "<h3>Description du problème (Diagnostique)</h3>"
                   "<p><i>Remplir manuellement ou suite à l'inspection:</i></p>"
                   "<div style='border: 1px solid black; height: 300px;'></div>"
                   "<br><br>"
                   "<p align='right'><b>Date : </b>" + QDate::currentDate().toString("dd/MM/yyyy") + "</p>"
                   "<p align='right'><b>Signature : </b>___________________</p>";
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "✅ Demande de maintenance générée avec succès !");
}

void pagemachine::on_pushButton_9_clicked()
{
    int fonctionnelle = 0, maintenance = 0, panne = 0, arretee = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString etat = ui->tableWidget->item(row, 4)->text();

            if (etat == "Fonctionnelle") fonctionnelle++;
            else if (etat == "Maintenance") maintenance++;
            else if (etat == "En panne") panne++;
            else if (etat == "Arrêtée") arretee++;
        }
    }

    int totalEtat = fonctionnelle + maintenance + panne + arretee;

    if (totalEtat == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune machine à afficher.");
        return;
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des machines (Par État)");
    statsDialog->resize(900, 650);
    statsDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f1e7dc;"
        "   border: 2px dashed #b08a6b;"
        "   border-radius: 20px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6b3e26, stop:1 #4a2717);"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   padding: 10px;"
        "}"
    );
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES MACHINES (PAR ÉTAT)");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *totalLabel = new QLabel(QString("Total machines: %1").arg(totalEtat));
    totalLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #fffaf5;"
        "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
        "border: 1px solid #c49a6c;"
    );
    headerLayout->addWidget(totalLabel);

    mainLayout->addWidget(headerFrame);

    QWidget *etatTab = new QWidget();
    QVBoxLayout *etatLayout = new QVBoxLayout(etatTab);

    QFrame *chartFrame = new QFrame();
    chartFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #fffaf5;"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   padding: 10px;"
        "}"
    );
    QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
    chartLayout->setSpacing(20);
    chartLayout->setAlignment(Qt::AlignBottom);

    int maxCount = qMax(qMax(qMax(fonctionnelle, maintenance), panne), arretee);
    maxCount = qMax(maxCount, 1);
    const int maxBarHeight = 300;

    struct EtatData { QString name; int count; QColor color; QString icon; };
    QList<EtatData> etats = {
        {"Fonctionnelle", fonctionnelle, QColor(100, 200, 100), "✅"},
        {"Maintenance", maintenance, QColor(255, 255, 100), "🔧"},
        {"En panne", panne, QColor(200, 100, 100), "⚠️"},
        {"Arrêtée", arretee, QColor(150, 150, 150), "⏹️"}
    };

    for (const EtatData &ed : etats) {
        if (ed.count == 0) continue;
        double percent = 100.0 * ed.count / totalEtat;
        int barHeight = (ed.count * maxBarHeight) / maxCount;
        if (barHeight < 10) barHeight = 10;

        QWidget *container = new QWidget();
        QVBoxLayout *barLayout = new QVBoxLayout(container);
        barLayout->setSpacing(8);
        barLayout->setAlignment(Qt::AlignHCenter);

        QLabel *countLabel = new QLabel(QString::number(ed.count));
        countLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #3a2a20;");
        countLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(countLabel);

        QLabel *percentLabel = new QLabel(QString("%1%").arg(percent, 0, 'f', 1));
        percentLabel->setStyleSheet("font-size: 16px; color: #5b2f1d; font-weight: bold;");
        percentLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(percentLabel);

        QFrame *bar = new QFrame();
        bar->setFixedWidth(100);
        bar->setFixedHeight(barHeight);
        bar->setStyleSheet(QString(R"(
            QFrame {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 %1,
                                            stop:1 %2);
                border: 2px solid #3a1f14;
                border-radius: 12px;
                border-bottom: 4px solid #2a150e;
            }
        )").arg(ed.color.lighter(110).name()).arg(ed.color.name()));
        barLayout->addWidget(bar, 0, Qt::AlignCenter);

        QLabel *nameLabel = new QLabel(ed.icon + " " + ed.name);
        nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #3a2a20;");
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setWordWrap(true);
        barLayout->addWidget(nameLabel);

        barLayout->addStretch();
        chartLayout->addWidget(container);
    }

    etatLayout->addWidget(chartFrame);
    mainLayout->addWidget(etatTab);

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a4a2e, stop:1 #5b2f1d);"
        "   border: 2px solid #3a1f14;"
        "   border-radius: 20px;"
        "   padding: 12px 40px;"
        "   font-weight: bold;"
        "   font-size: 16px;"
        "   color: #fffaf5;"
        "   border-bottom: 4px solid #2a150e;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5a3a, stop:1 #6b3e26);"
        "}"
        "QPushButton:pressed {"
        "   margin-top: 2px;"
        "   border-bottom: 2px solid #2a150e;"
        "}"
    );
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    statsDialog->exec();
    delete statsDialog;
}
