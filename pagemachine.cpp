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
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QFrame>
#include <QTabWidget>

#include "commandes.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"

pagemachine::pagemachine(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagemachine)
    , m_idEmploye(idEmploye)
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
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
}

void pagemachine::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString nomFilter = ui->searchNomEdit->text();

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;

        if (!idFilter.isEmpty()) {
            QString id = ui->tableWidget->item(row, 0)->text();
            if (!id.contains(idFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        if (match && !nomFilter.isEmpty()) {
            QString nom = ui->tableWidget->item(row, 1)->text();
            if (!nom.contains(nomFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

void pagemachine::setupMachinesTable()
{
    QStringList headers = {"ID Machine", "Nom Machine", "Type Machine", "État Machine",
                           "Capacité", "Fréquence", "Niveau de Charge"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 120);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void pagemachine::loadMachines()
{
    ui->tableWidget->setRowCount(0);

    QList<QStringList> sampleData = {
        {"M001", "Fraiseuse CNC", "Fraiseuse", "Fonctionnelle", "500 unités/h", "50 Hz", "75"},
        {"M002", "Tour automatique", "Tour", "Maintenance", "300 unités/h", "60 Hz", "45"},
        {"M003", "Perceuse colonne", "Perceuse", "Fonctionnelle", "200 unités/h", "50 Hz", "90"},
        {"M004", "Presse hydraulique", "Presse", "En panne", "1000 unités/h", "50 Hz", "0"},
        {"M005", "Scie circulaire", "Scie", "Fonctionnelle", "400 unités/h", "60 Hz", "60"},
        {"M006", "Rectifieuse", "Rectifieuse", "Arrêtée", "150 unités/h", "50 Hz", "0"},
        {"M007", "Imprimante 3D", "Imprimante 3D", "Fonctionnelle", "50 unités/h", "60 Hz", "30"}
    };

    for (const QStringList &rowData : sampleData) {
        addMachineToTable(rowData[0], rowData[1], rowData[2], rowData[3],
                          rowData[4], rowData[5], rowData[6]);
    }
}

void pagemachine::addMachineToTable(const QString &id, const QString &nom,
                                    const QString &type, const QString &etat,
                                    const QString &capacite, const QString &frequence,
                                    const QString &niveauCharge)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(etat));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(capacite));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(frequence));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(niveauCharge + "%"));

    updateRowColors(row);
}

void pagemachine::updateRowColors(int row)
{
    QTableWidgetItem *etatItem = ui->tableWidget->item(row, 3);
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

    QTableWidgetItem *chargeItem = ui->tableWidget->item(row, 6);
    if (chargeItem) {
        QString chargeStr = chargeItem->text();
        chargeStr.remove("%");
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

void pagemachine::updateMachineInTable(int row, const QString &nom,
                                       const QString &type, const QString &etat,
                                       const QString &capacite, const QString &frequence,
                                       const QString &niveauCharge)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(nom);
        ui->tableWidget->item(row, 2)->setText(type);
        ui->tableWidget->item(row, 3)->setText(etat);
        ui->tableWidget->item(row, 4)->setText(capacite);
        ui->tableWidget->item(row, 5)->setText(frequence);
        ui->tableWidget->item(row, 6)->setText(niveauCharge + "%");

        updateRowColors(row);
    }
}

void pagemachine::on_pushButton_6_clicked()
{
    hide();
    commandes *lg = new commandes(m_idEmploye, this);
    lg->show();
}

void pagemachine::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(m_idEmploye, this);
    pl->show();
}

void pagemachine::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(m_idEmploye, this);
    pf->show();
}

void pagemachine::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(m_idEmploye, this);
    pd->show();
}

void pagemachine::on_pushButton_22_clicked()
{
    hide();
    Matieres *mm = new Matieres(m_idEmploye, this);
    mm->show();
}

void pagemachine::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(m_idEmploye, this);
    ss->show();
}

void pagemachine::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Ajouter une machine");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);

    QLabel *header = new QLabel("AJOUTER UNE MACHINE");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID machine");

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom de la machine");

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});

    QLineEdit *capaciteEdit = new QLineEdit();
    capaciteEdit->setPlaceholderText("Capacité");

    QLineEdit *frequenceEdit = new QLineEdit();
    frequenceEdit->setPlaceholderText("Fréquence");

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setSuffix(" %");

    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(chargeSpinBox);

    QPushButton *btnSave = new QPushButton("Enregistrer");
    QPushButton *btnCancel = new QPushButton("Annuler");
    mainLayout->addWidget(btnSave);
    mainLayout->addWidget(btnCancel);

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, nomEdit, typeCombo,
                                             etatCombo, capaciteEdit, frequenceEdit,
                                             chargeSpinBox, this]() {
        if (idEdit->text().isEmpty() || nomEdit->text().isEmpty() ||
            capaciteEdit->text().isEmpty() || frequenceEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                                 "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        addMachineToTable(idEdit->text(),
                          nomEdit->text(),
                          typeCombo->currentText(),
                          etatCombo->currentText(),
                          capaciteEdit->text(),
                          frequenceEdit->text(),
                          QString::number(chargeSpinBox->value()));

        QMessageBox::information(ajoutDialog, "Succès",
                                 "La machine a été ajoutée avec succès.");
        ajoutDialog->accept();
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
    QString nom = ui->tableWidget->item(currentRow, 1)->text();
    QString type = ui->tableWidget->item(currentRow, 2)->text();
    QString etat = ui->tableWidget->item(currentRow, 3)->text();
    QString capacite = ui->tableWidget->item(currentRow, 4)->text();
    QString frequence = ui->tableWidget->item(currentRow, 5)->text();
    QString niveauCharge = ui->tableWidget->item(currentRow, 6)->text();
    niveauCharge.remove("%");

    QDialog *modifierDialog = new QDialog(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(modifierDialog);

    QLineEdit *idEdit = new QLineEdit(id);
    idEdit->setReadOnly(true);

    QLineEdit *nomEdit = new QLineEdit(nom);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});
    typeCombo->setCurrentText(type);

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});
    etatCombo->setCurrentText(etat);

    QLineEdit *capaciteEdit = new QLineEdit(capacite);
    QLineEdit *frequenceEdit = new QLineEdit(frequence);

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setValue(niveauCharge.toInt());
    chargeSpinBox->setSuffix(" %");

    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(chargeSpinBox);

    QPushButton *btnSave = new QPushButton("Mettre à jour");
    QPushButton *btnCancel = new QPushButton("Annuler");
    mainLayout->addWidget(btnSave);
    mainLayout->addWidget(btnCancel);

    connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, nomEdit, typeCombo,
                                             etatCombo, capaciteEdit, frequenceEdit,
                                             chargeSpinBox, this]() {
        if (nomEdit->text().isEmpty() || capaciteEdit->text().isEmpty() || frequenceEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants", "Veuillez remplir tous les champs.");
            return;
        }

        updateMachineInTable(currentRow,
                             nomEdit->text(),
                             typeCombo->currentText(),
                             etatCombo->currentText(),
                             capaciteEdit->text(),
                             frequenceEdit->text(),
                             QString::number(chargeSpinBox->value()));

        QMessageBox::information(modifierDialog, "Succès",
                                 "La machine a été mise à jour avec succès.");
        modifierDialog->accept();
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
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Voulez-vous vraiment supprimer la machine '%1' (ID: %2) ?")
                                      .arg(nom).arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);
        QMessageBox::information(this, "Succès", "Machine supprimée avec succès.");
    }
}

void pagemachine::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNomEdit->clear();
    loadMachines();
    QMessageBox::information(this, "Actualisation", "Liste des machines actualisée.");
}

void pagemachine::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                    "machines_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
                                                    "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }

    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out.setGenerateByteOrderMark(true);

    QStringList headers = {"ID Machine", "Nom Machine", "Type Machine", "État Machine",
                           "Capacité", "Fréquence", "Niveau de Charge"};
    for (int i = 0; i < headers.size(); ++i) {
        if (i > 0) out << ";";
        out << headers[i];
    }
    out << "\n";

    int visibleRows = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (ui->tableWidget->isRowHidden(row)) continue;

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            if (col > 0) out << ";";
            QString cell = ui->tableWidget->item(row, col)->text();
            if (cell.contains(';') || cell.contains('"') || cell.contains('\n')) {
                cell.replace("\"", "\"\"");
                cell = "\"" + cell + "\"";
            }
            out << cell;
        }
        out << "\n";
        visibleRows++;
    }

    file.close();
    QMessageBox::information(this, "Succès",
                             QString("Fichier CSV exporté avec succès.\n%1 ligne(s) exportée(s).")
                                 .arg(visibleRows));
}

void pagemachine::on_pushButton_9_clicked()
{
    int fonctionnelle = 0, maintenance = 0, panne = 0, arretee = 0;
    int chargeFaible = 0, chargeMoyenne = 0, chargeElevee = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString etat = ui->tableWidget->item(row, 3)->text();
            QString chargeStr = ui->tableWidget->item(row, 6)->text();
            chargeStr.remove("%");
            int charge = chargeStr.toInt();

            if (etat == "Fonctionnelle") fonctionnelle++;
            else if (etat == "Maintenance") maintenance++;
            else if (etat == "En panne") panne++;
            else if (etat == "Arrêtée") arretee++;

            if (charge < 30) chargeFaible++;
            else if (charge < 70) chargeMoyenne++;
            else chargeElevee++;
        }
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des machines");
    QVBoxLayout *layout = new QVBoxLayout(statsDialog);

    QLabel *label = new QLabel(
        QString("Fonctionnelle: %1\nMaintenance: %2\nEn panne: %3\nArrêtée: %4\n\nCharge faible: %5\nCharge moyenne: %6\nCharge élevée: %7")
            .arg(fonctionnelle).arg(maintenance).arg(panne).arg(arretee)
            .arg(chargeFaible).arg(chargeMoyenne).arg(chargeElevee));
    layout->addWidget(label);

    QPushButton *closeButton = new QPushButton("Fermer");
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    statsDialog->exec();
    delete statsDialog;
}
