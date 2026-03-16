#include "matieres.h"
#include "ui_matieres.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDateTime>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFrame>
#include <QTabWidget>

#include "pageemployee.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "pagemachine.h"

Matieres::Matieres(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Matieres)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    setupMatieresTable();
    loadMatieres();
    setupSearch();
}

Matieres::~Matieres()
{
    delete ui;
}

void Matieres::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
    connect(ui->searchTypeEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
}

void Matieres::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString typeFilter = ui->searchTypeEdit->text();

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;

        if (!idFilter.isEmpty()) {
            QString id = ui->tableWidget->item(row, 0)->text();
            if (!id.contains(idFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        if (match && !typeFilter.isEmpty()) {
            QString type = ui->tableWidget->item(row, 1)->text();
            if (!type.contains(typeFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

void Matieres::on_pushButton_6_clicked()
{
    hide();
    Matieres *mat = new Matieres(m_idEmploye, this);
    mat->show();
}

void Matieres::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(m_idEmploye, this);
    pl->show();
}

void Matieres::on_pushButton_5_clicked()
{
    hide();
    login *lg = new login(this);
    lg->show();
}

void Matieres::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(m_idEmploye, this);
    pf->show();
}

void Matieres::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(m_idEmploye, this);
    pd->show();
}

void Matieres::on_pushButton_22_clicked()
{
    hide();
    Matieres *pddd = new Matieres(m_idEmploye, this);
    pddd->show();
}

void Matieres::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(m_idEmploye, this);
    ss->show();
}

void Matieres::setupMatieresTable()
{
    QStringList headers = {"ID Matière", "Type", "Couleur", "Qualité", "Prix unitaire", "Stock", "Fournisseur"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 80);
    ui->tableWidget->setColumnWidth(6, 120);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void Matieres::loadMatieres()
{
    ui->tableWidget->setRowCount(0);

    QList<QStringList> sampleData = {
        {"M001", "Cuir", "Marron", "Qualité A", "45.50", "100", "Cuirs Premium"},
        {"M002", "Tissu", "Noir", "Qualité B", "12.75", "250", "Tissus Lyon"},
        {"M003", "Métal", "Argent", "Qualité A", "78.00", "50", "Métaux Industries"},
        {"M004", "Plastique", "Blanc", "Qualité C", "8.50", "500", "Plastiques Modernes"},
        {"M005", "Bois", "Chêne", "Qualité B", "32.00", "150", "Bois & Cie"}
    };

    for (const QStringList &rowData : sampleData) {
        addMatiereToTable(rowData[0], rowData[1], rowData[2], rowData[3],
                          rowData[4], rowData[5], rowData[6]);
    }
}

void Matieres::addMatiereToTable(const QString &id, const QString &type,
                                 const QString &couleur, const QString &qualite,
                                 const QString &prix, const QString &stock,
                                 const QString &fournisseur)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(couleur));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(qualite));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(prix + " €"));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(stock));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(fournisseur));

    updateRowColors(row);
}

void Matieres::updateRowColors(int row)
{
    QTableWidgetItem *qualiteItem = ui->tableWidget->item(row, 3);
    if (qualiteItem) {
        QString qualite = qualiteItem->text();
        if (qualite.contains("A")) {
            qualiteItem->setBackground(QColor(200, 255, 200));
            qualiteItem->setForeground(QColor(0, 100, 0));
        } else if (qualite.contains("B")) {
            qualiteItem->setBackground(QColor(255, 255, 200));
            qualiteItem->setForeground(QColor(128, 128, 0));
        } else if (qualite.contains("C")) {
            qualiteItem->setBackground(QColor(255, 200, 200));
            qualiteItem->setForeground(QColor(139, 0, 0));
        }
    }

    QTableWidgetItem *stockItem = ui->tableWidget->item(row, 5);
    if (stockItem) {
        int stock = stockItem->text().toInt();
        if (stock < 50) {
            stockItem->setBackground(QColor(255, 200, 200));
        } else if (stock < 100) {
            stockItem->setBackground(QColor(255, 255, 200));
        }
    }
}

void Matieres::updateMatiereInTable(int row, const QString &type,
                                    const QString &couleur, const QString &qualite,
                                    const QString &prix, const QString &stock,
                                    const QString &fournisseur)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(type);
        ui->tableWidget->item(row, 2)->setText(couleur);
        ui->tableWidget->item(row, 3)->setText(qualite);
        ui->tableWidget->item(row, 4)->setText(prix + " €");
        ui->tableWidget->item(row, 5)->setText(stock);
        ui->tableWidget->item(row, 6)->setText(fournisseur);

        updateRowColors(row);
    }
}

void Matieres::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouvelle matière");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setModal(true);

    ajoutDialog->setStyleSheet(
        "QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }"
        "QLabel#headerLabel { color: #6b3e26; font-size: 22px; font-weight: bold; margin-bottom: 15px; }"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QLineEdit, QComboBox { background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px; padding: 12px; color: #3a2a20; font-size: 14px; min-height: 20px; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton { border-radius: 10px; padding: 15px; font-weight: bold; color: white; border-bottom: 3px solid rgba(0,0,0,0.2); font-size: 15px; min-width: 150px; }"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("AJOUTER MATIÈRE PREMIÈRE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID matière (ex: M001)");
    idEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *typeEdit = new QLineEdit();
    typeEdit->setPlaceholderText("Type (Cuir, Tissu, etc.)");
    typeEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *couleurEdit = new QLineEdit();
    couleurEdit->setPlaceholderText("Couleur");
    couleurEdit->setAlignment(Qt::AlignCenter);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});

    QLineEdit *prixEdit = new QLineEdit();
    prixEdit->setPlaceholderText("Prix unitaire (€)");
    prixEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *stockEdit = new QLineEdit();
    stockEdit->setPlaceholderText("Stock");
    stockEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *fournisseurEdit = new QLineEdit();
    fournisseurEdit->setPlaceholderText("Fournisseur");
    fournisseurEdit->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel("Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("Type :"));
    mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(new QLabel("Couleur :"));
    mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(new QLabel("Qualité :"));
    mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(new QLabel("Prix unitaire :"));
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(new QLabel("Stock :"));
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("Fournisseur :"));
    mainLayout->addWidget(fournisseurEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("Enregistrer");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, typeEdit, couleurEdit,
                                             qualiteCombo, prixEdit, stockEdit, fournisseurEdit, this]() {
        if (idEdit->text().isEmpty() || typeEdit->text().isEmpty() ||
            couleurEdit->text().isEmpty() || prixEdit->text().isEmpty() ||
            stockEdit->text().isEmpty() || fournisseurEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                                 "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        bool ok;
        double prix = prixEdit->text().toDouble(&ok);
        if (!ok || prix <= 0) {
            QMessageBox::warning(ajoutDialog, "Prix invalide",
                                 "Veuillez entrer un prix valide.");
            return;
        }

        int stock = stockEdit->text().toInt(&ok);
        if (!ok || stock < 0) {
            QMessageBox::warning(ajoutDialog, "Stock invalide",
                                 "Veuillez entrer un stock valide.");
            return;
        }

        QString formattedPrix = QString::number(prix, 'f', 2);

        addMatiereToTable(idEdit->text(),
                          typeEdit->text(),
                          couleurEdit->text(),
                          qualiteCombo->currentText(),
                          formattedPrix,
                          QString::number(stock),
                          fournisseurEdit->text());

        QMessageBox::information(ajoutDialog, "Succès",
                                 "La matière a été ajoutée avec succès.");
        ajoutDialog->accept();
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void Matieres::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                             "Veuillez sélectionner une matière à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString type = ui->tableWidget->item(currentRow, 1)->text();
    QString couleur = ui->tableWidget->item(currentRow, 2)->text();
    QString qualite = ui->tableWidget->item(currentRow, 3)->text();
    QString prix = ui->tableWidget->item(currentRow, 4)->text().replace(" €", "");
    QString stock = ui->tableWidget->item(currentRow, 5)->text();
    QString fournisseur = ui->tableWidget->item(currentRow, 6)->text();

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier matière");
    modifierDialog->setFixedSize(650, 800);
    modifierDialog->setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(modifierDialog);
    QLabel *header = new QLabel("MODIFIER MATIÈRE");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit(id);
    idEdit->setReadOnly(true);

    QLineEdit *typeEdit = new QLineEdit(type);
    QLineEdit *couleurEdit = new QLineEdit(couleur);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    qualiteCombo->setCurrentText(qualite);

    QLineEdit *prixEdit = new QLineEdit(prix);
    QLineEdit *stockEdit = new QLineEdit(stock);
    QLineEdit *fournisseurEdit = new QLineEdit(fournisseur);

    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(fournisseurEdit);

    QPushButton *btnSave = new QPushButton("Mettre à jour");
    QPushButton *btnCancel = new QPushButton("Annuler");
    mainLayout->addWidget(btnSave);
    mainLayout->addWidget(btnCancel);

    connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, typeEdit, couleurEdit,
                                             qualiteCombo, prixEdit, stockEdit, fournisseurEdit, this]() {
        if (typeEdit->text().isEmpty() || couleurEdit->text().isEmpty() ||
            prixEdit->text().isEmpty() || stockEdit->text().isEmpty() ||
            fournisseurEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants",
                                 "Veuillez remplir tous les champs.");
            return;
        }

        bool ok;
        double prix = prixEdit->text().toDouble(&ok);
        if (!ok || prix <= 0) {
            QMessageBox::warning(modifierDialog, "Prix invalide", "Prix invalide.");
            return;
        }

        int stock = stockEdit->text().toInt(&ok);
        if (!ok || stock < 0) {
            QMessageBox::warning(modifierDialog, "Stock invalide", "Stock invalide.");
            return;
        }

        QString formattedPrix = QString::number(prix, 'f', 2);

        updateMatiereInTable(currentRow,
                             typeEdit->text(),
                             couleurEdit->text(),
                             qualiteCombo->currentText(),
                             formattedPrix,
                             QString::number(stock),
                             fournisseurEdit->text());

        QMessageBox::information(modifierDialog, "Succès",
                                 "La matière a été mise à jour avec succès.");
        modifierDialog->accept();
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
    modifierDialog->deleteLater();
}

void Matieres::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                             "Veuillez sélectionner une matière à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString type = ui->tableWidget->item(currentRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Voulez-vous vraiment supprimer la matière '%1' - %2 ?")
                                      .arg(id).arg(type),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);
        QMessageBox::information(this, "Succès", "Matière supprimée avec succès.");
    }
}

void Matieres::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchTypeEdit->clear();
    loadMatieres();
    QMessageBox::information(this, "Actualisation", "Liste des matières actualisée.");
}

void Matieres::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                    "matieres_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
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

    QStringList headers = {"ID Matière", "Type", "Couleur", "Qualité", "Prix unitaire", "Stock", "Fournisseur"};
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

void Matieres::on_pushButton_9_clicked()
{
    int qualiteA = 0, qualiteB = 0, qualiteC = 0;
    int stockFaible = 0, stockMoyen = 0, stockEleve = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString qualite = ui->tableWidget->item(row, 3)->text();
            int stock = ui->tableWidget->item(row, 5)->text().toInt();

            if (qualite == "Qualité A") qualiteA++;
            else if (qualite == "Qualité B") qualiteB++;
            else if (qualite == "Qualité C") qualiteC++;

            if (stock < 50) stockFaible++;
            else if (stock < 150) stockMoyen++;
            else stockEleve++;
        }
    }

    int totalQualite = qualiteA + qualiteB + qualiteC;
    int totalStock = stockFaible + stockMoyen + stockEleve;

    if (totalQualite == 0 && totalStock == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune matière à afficher.");
        return;
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des matières");
    statsDialog->resize(500, 300);

    QVBoxLayout *layout = new QVBoxLayout(statsDialog);
    QLabel *label = new QLabel(
        QString("Qualité A: %1\nQualité B: %2\nQualité C: %3\n\nStock faible: %4\nStock moyen: %5\nStock élevé: %6")
            .arg(qualiteA).arg(qualiteB).arg(qualiteC)
            .arg(stockFaible).arg(stockMoyen).arg(stockEleve));
    layout->addWidget(label);

    QPushButton *closeButton = new QPushButton("Fermer");
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    layout->addWidget(closeButton);

    statsDialog->exec();
    delete statsDialog;
}
