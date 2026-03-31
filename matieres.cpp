#include "matieres.h"
#include "ui_matieres.h"
#include <QDateEdit>
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
#include "pageemployee.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "pagemachine.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>



Matieres::Matieres(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Matieres)
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
    Matieres *mat = new Matieres(this);
    mat->show();
}

void Matieres::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
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
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void Matieres::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void Matieres::on_pushButton_22_clicked()
{
    hide();
    Matieres *pddd = new Matieres(this);
    pddd->show();
}

void Matieres::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(this);
    ss->show();
}

void Matieres::setupMatieresTable()
{
    QStringList headers = {"ID", "Type", "Qualité", "Couleur", "Épaisseur", "Prix (€)", "Stock", "Seuil", "Date Réc."};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 90);
    ui->tableWidget->setColumnWidth(3, 90);
    ui->tableWidget->setColumnWidth(4, 90);
    ui->tableWidget->setColumnWidth(5, 80);
    ui->tableWidget->setColumnWidth(6, 80);
    ui->tableWidget->setColumnWidth(7, 80);
    ui->tableWidget->setColumnWidth(8, 100);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void Matieres::loadMatieres()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID_MATIERE, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, TO_CHAR(DATE_RECEPTION, 'DD/MM/YYYY') FROM MATIEREPREMIER");
    
    if (query.exec()) {
        while (query.next()) {
            QDateTime recDate = QDateTime::fromString(query.value(8).toString(), "dd/MM/yyyy");
            
            addMatiereToTable(
                query.value(0).toString(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString(),
                query.value(7).toString(),
                recDate
            );
        }
    } else {
        qDebug() << "Erreur de chargement des matières:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les matières:\n" + query.lastError().text());
    }
}

void Matieres::addMatiereToTable(const QString &id, const QString &type,
                                 const QString &qualite, const QString &couleur,
                                 const QString &epaisseur, const QString &prix,
                                 const QString &stock, const QString &seuil,
                                 const QDateTime &dateReception)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(qualite));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(couleur));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(epaisseur));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(prix + " €"));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(stock));
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(seuil));
    ui->tableWidget->setItem(row, 8, new QTableWidgetItem(dateReception.toString("dd/MM/yyyy")));

    updateRowColors(row);
}

void Matieres::updateRowColors(int row)
{
    QTableWidgetItem *qualiteItem = ui->tableWidget->item(row, 2);
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

    QTableWidgetItem *stockItem = ui->tableWidget->item(row, 6);
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
                                    const QString &qualite, const QString &couleur,
                                    const QString &epaisseur, const QString &prix,
                                    const QString &stock, const QString &seuil,
                                    const QDateTime &dateReception)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(type);
        ui->tableWidget->item(row, 2)->setText(qualite);
        ui->tableWidget->item(row, 3)->setText(couleur);
        ui->tableWidget->item(row, 4)->setText(epaisseur);
        ui->tableWidget->item(row, 5)->setText(prix + " €");
        ui->tableWidget->item(row, 6)->setText(stock);
        ui->tableWidget->item(row, 7)->setText(seuil);
        ui->tableWidget->item(row, 8)->setText(dateReception.toString("dd/MM/yyyy"));

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
        "QLineEdit, QComboBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }"
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
    stockEdit->setPlaceholderText("Stock (Quantité)");
    stockEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *epaisseurEdit = new QLineEdit();
    epaisseurEdit->setPlaceholderText("Épaisseur");
    epaisseurEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *seuilEdit = new QLineEdit();
    seuilEdit->setPlaceholderText("Seuil minimal");
    seuilEdit->setAlignment(Qt::AlignCenter);

    QDateEdit *dateReceptionEdit = new QDateEdit(QDate::currentDate());
    dateReceptionEdit->setDisplayFormat("dd/MM/yyyy");
    dateReceptionEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📦 Type Cuir :"));
    mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(new QLabel("⭐ Qualité :"));
    mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(new QLabel("🎨 Couleur :"));
    mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(new QLabel("📏 Épaisseur :"));
    mainLayout->addWidget(epaisseurEdit);
    mainLayout->addWidget(new QLabel("💰 Prix unitaire :"));
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(new QLabel("📊 Stock :"));
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("📉 Seuil minimum :"));
    mainLayout->addWidget(seuilEdit);
    mainLayout->addWidget(new QLabel("📅 Date Reception :"));
    mainLayout->addWidget(dateReceptionEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, typeEdit, qualiteCombo,
                                              couleurEdit, epaisseurEdit, prixEdit, stockEdit,
                                              seuilEdit, dateReceptionEdit, this]() {
        if (idEdit->text().isEmpty() || typeEdit->text().isEmpty() ||
            couleurEdit->text().isEmpty() || epaisseurEdit->text().isEmpty() ||
            prixEdit->text().isEmpty() || stockEdit->text().isEmpty() || seuilEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                               "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        bool ok;
        double prix = prixEdit->text().toDouble(&ok);
        if (!ok || prix <= 0) {
            QMessageBox::warning(ajoutDialog, "Prix invalide",
                               "Veuillez entrer un prix valide (nombre positif).");
            return;
        }

        int stock = stockEdit->text().toInt(&ok);
        if (!ok || stock < 0) {
            QMessageBox::warning(ajoutDialog, "Stock invalide",
                               "Veuillez entrer un stock valide (nombre positif).");
            return;
        }

        int seuil = seuilEdit->text().toInt(&ok);
        if (!ok || seuil < 0) {
            QMessageBox::warning(ajoutDialog, "Seuil invalide",
                               "Veuillez entrer un seuil valide (nombre positif).");
            return;
        }

        QString formattedPrix = QString::number(prix, 'f', 2);

        QSqlQuery query;
        query.prepare("INSERT INTO MATIEREPREMIER (ID_MATIERE, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, DATE_RECEPTION) "
                      "VALUES (:id, :type, :qualite, :couleur, :epaisseur, :prix, :stock, :seuil, TO_TIMESTAMP(:date_reception, 'DD/MM/YYYY'))");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":type", typeEdit->text());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":couleur", couleurEdit->text());
        query.bindValue(":epaisseur", epaisseurEdit->text());
        query.bindValue(":prix", formattedPrix);
        query.bindValue(":stock", stock);
        query.bindValue(":seuil", seuil);
        query.bindValue(":date_reception", dateReceptionEdit->date().toString("dd/MM/yyyy"));

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ La matière a été ajoutée avec succès!");
            loadMatieres();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
        }
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
    QString qualite = ui->tableWidget->item(currentRow, 2)->text();
    QString couleur = ui->tableWidget->item(currentRow, 3)->text();
    QString epaisseur = ui->tableWidget->item(currentRow, 4)->text();
    QString prix = ui->tableWidget->item(currentRow, 5)->text().replace(" €", "");
    QString stock = ui->tableWidget->item(currentRow, 6)->text();
    QString seuil = ui->tableWidget->item(currentRow, 7)->text();
    QDate dateReception = QDate::fromString(ui->tableWidget->item(currentRow, 8)->text(), "dd/MM/yyyy");

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier matière");
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
        "QLineEdit, QComboBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }"
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

    QLabel *header = new QLabel("MODIFIER MATIÈRE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(id);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

    QLineEdit *typeEdit = new QLineEdit();
    typeEdit->setText(type);
    typeEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *couleurEdit = new QLineEdit();
    couleurEdit->setText(couleur);
    couleurEdit->setAlignment(Qt::AlignCenter);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    qualiteCombo->setCurrentText(qualite);

    QLineEdit *prixEdit = new QLineEdit();
    prixEdit->setText(prix);
    prixEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *stockEdit = new QLineEdit();
    stockEdit->setText(stock);
    stockEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *epaisseurEdit = new QLineEdit();
    epaisseurEdit->setText(epaisseur);
    epaisseurEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *seuilEdit = new QLineEdit();
    seuilEdit->setText(seuil);
    seuilEdit->setAlignment(Qt::AlignCenter);

    QDateEdit *dateReceptionEdit = new QDateEdit(dateReception);
    dateReceptionEdit->setDisplayFormat("dd/MM/yyyy");
    dateReceptionEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📦 Type Cuir :"));
    mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(new QLabel("⭐ Qualité :"));
    mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(new QLabel("🎨 Couleur :"));
    mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(new QLabel("📏 Épaisseur :"));
    mainLayout->addWidget(epaisseurEdit);
    mainLayout->addWidget(new QLabel("💰 Prix unitaire :"));
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(new QLabel("📊 Stock :"));
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("📉 Seuil minimum :"));
    mainLayout->addWidget(seuilEdit);
    mainLayout->addWidget(new QLabel("📅 Date Reception :"));
    mainLayout->addWidget(dateReceptionEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, id, typeEdit, qualiteCombo,
                                              couleurEdit, epaisseurEdit, prixEdit, stockEdit,
                                              seuilEdit, dateReceptionEdit, this]() {
        if (typeEdit->text().isEmpty() || couleurEdit->text().isEmpty() ||
            epaisseurEdit->text().isEmpty() || prixEdit->text().isEmpty() ||
            stockEdit->text().isEmpty() || seuilEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants",
                               "Veuillez remplir tous les champs.");
            return;
        }

        bool ok;
        double prix = prixEdit->text().toDouble(&ok);
        if (!ok || prix <= 0) {
            QMessageBox::warning(modifierDialog, "Prix invalide",
                               "Veuillez entrer un prix valide (nombre positif).");
            return;
        }

        int stock = stockEdit->text().toInt(&ok);
        if (!ok || stock < 0) {
            QMessageBox::warning(modifierDialog, "Stock invalide",
                               "Veuillez entrer un stock valide (nombre positif).");
            return;
        }

        int seuil = seuilEdit->text().toInt(&ok);
        if (!ok || seuil < 0) {
            QMessageBox::warning(modifierDialog, "Seuil invalide",
                               "Veuillez entrer un seuil valide (nombre positif).");
            return;
        }

        QString formattedPrix = QString::number(prix, 'f', 2);

        QSqlQuery query;
        query.prepare("UPDATE MATIEREPREMIER SET TYPE_CUIR = :type, QUALITE = :qualite, COULEUR = :couleur, "
                      "EPAISSEUR = :epaisseur, PRIX_UNITAIRE = :prix, QUANTITE_STOCK = :stock, "
                      "SEUIL_MIN = :seuil, DATE_RECEPTION = TO_TIMESTAMP(:date_reception, 'DD/MM/YYYY') "
                      "WHERE ID_MATIERE = :id");
        query.bindValue(":type", typeEdit->text());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":couleur", couleurEdit->text());
        query.bindValue(":epaisseur", epaisseurEdit->text());
        query.bindValue(":prix", formattedPrix);
        query.bindValue(":stock", stock);
        query.bindValue(":seuil", seuil);
        query.bindValue(":date_reception", dateReceptionEdit->date().toString("dd/MM/yyyy"));
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ La matière a été mise à jour avec succès!");
            loadMatieres();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
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
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer la matière '%1' - %2 ?")
                                 .arg(id).arg(type),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM MATIEREPREMIER WHERE ID_MATIERE = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Matière supprimée avec succès!");
            loadMatieres();
            qDebug() << "Matière supprimée:" << id << "- Type:" << type;
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        }
    }
}

void Matieres::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchTypeEdit->clear();
    loadMatieres();
    QMessageBox::information(this, "Actualisation", "✅ Liste des matières actualisée !");
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

    QStringList headers = {"ID Matière", "Type", "Couleur", "Qualité",
                           "Prix unitaire", "Stock", "Fournisseur"};
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
                             QString("✅ Fichier CSV exporté avec succès !\n%1 ligne(s) exportée(s).")
                             .arg(visibleRows));
}



void Matieres::on_pushButton_9_clicked()
{

    int qualiteA = 0, qualiteB = 0, qualiteC = 0;
    int stockFaible = 0, stockMoyen = 0, stockEleve = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString qualite = ui->tableWidget->item(row, 3)->text(); // column 3 = Qualité
            int stock = ui->tableWidget->item(row, 5)->text().toInt(); // column 5 = Stock

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

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES MATIÈRES");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *totalLabel = new QLabel(QString("Total matières: %1").arg(totalQualite));
    totalLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #fffaf5;"
        "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
        "border: 1px solid #c49a6c;"
    );
    headerLayout->addWidget(totalLabel);

    mainLayout->addWidget(headerFrame);


    QTabWidget *tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(
        "QTabWidget::pane {"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   background-color: #fffaf5;"
        "   padding: 10px;"
        "}"
        "QTabBar::tab {"
        "   background-color: #e9dccf;"
        "   border: 1px solid #b08a6b;"
        "   border-radius: 10px;"
        "   padding: 8px 16px;"
        "   margin-right: 5px;"
        "   font-weight: bold;"
        "   color: #3a2a20;"
        "}"
        "QTabBar::tab:selected {"
        "   background-color: #6b3e26;"
        "   color: #fffaf5;"
        "}"
    );

    if (totalQualite > 0) {
        QWidget *qualiteTab = new QWidget();
        QVBoxLayout *qualiteLayout = new QVBoxLayout(qualiteTab);

        QFrame *chartFrame = new QFrame();
        chartFrame->setStyleSheet(
            "QFrame {"
            "   background-color: #fffaf5;"
            "   border: none;"
            "   padding: 10px;"
            "}"
        );
        QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
        chartLayout->setSpacing(20);
        chartLayout->setAlignment(Qt::AlignBottom);

        int maxCount = qMax(qMax(qualiteA, qualiteB), qualiteC);
        maxCount = qMax(maxCount, 1);
        const int maxBarHeight = 200;

        struct QualData { QString name; int count; QColor color; QString icon; };
        QList<QualData> qualites = {
            {"Qualité A", qualiteA, QColor(100, 200, 100), "⭐"},
            {"Qualité B", qualiteB, QColor(255, 255, 100), "⭐"},
            {"Qualité C", qualiteC, QColor(200, 100, 100), "⭐"}
        };

        for (const QualData &qd : qualites) {
            if (qd.count == 0) continue;
            double percent = 100.0 * qd.count / totalQualite;
            int barHeight = (qd.count * maxBarHeight) / maxCount;
            if (barHeight < 10) barHeight = 10;

            QWidget *container = new QWidget();
            QVBoxLayout *barLayout = new QVBoxLayout(container);
            barLayout->setSpacing(8);
            barLayout->setAlignment(Qt::AlignHCenter);

            QLabel *countLabel = new QLabel(QString::number(qd.count));
            countLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3a2a20;");
            countLabel->setAlignment(Qt::AlignCenter);
            barLayout->addWidget(countLabel);

            QLabel *percentLabel = new QLabel(QString("%1%").arg(percent, 0, 'f', 1));
            percentLabel->setStyleSheet("font-size: 14px; color: #5b2f1d; font-weight: bold;");
            percentLabel->setAlignment(Qt::AlignCenter);
            barLayout->addWidget(percentLabel);

            QFrame *bar = new QFrame();
            bar->setFixedWidth(80);
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
            )").arg(qd.color.lighter(110).name()).arg(qd.color.name()));
            barLayout->addWidget(bar, 0, Qt::AlignCenter);

            QLabel *nameLabel = new QLabel(qd.icon + " " + qd.name);
            nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3a2a20;");
            nameLabel->setAlignment(Qt::AlignCenter);
            barLayout->addWidget(nameLabel);

            barLayout->addStretch();
            chartLayout->addWidget(container);
        }

        qualiteLayout->addWidget(chartFrame);
        tabWidget->addTab(qualiteTab, "Par qualité");
    }


    if (totalStock > 0) {
        QWidget *stockTab = new QWidget();
        QVBoxLayout *stockLayout = new QVBoxLayout(stockTab);

        QFrame *chartFrame = new QFrame();
        chartFrame->setStyleSheet(
            "QFrame {"
            "   background-color: #fffaf5;"
            "   border: none;"
            "   padding: 10px;"
            "}"
        );
        QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
        chartLayout->setSpacing(20);
        chartLayout->setAlignment(Qt::AlignBottom);

        int maxCount = qMax(qMax(stockFaible, stockMoyen), stockEleve);
        maxCount = qMax(maxCount, 1);
        const int maxBarHeight = 200;

        struct StockData { QString name; int count; QColor color; QString icon; };
        QList<StockData> stocks = {
            {"Stock faible (<50)", stockFaible, QColor(200, 100, 100), "⚠️"},
            {"Stock moyen (50-150)", stockMoyen, QColor(255, 255, 100), "⚖️"},
            {"Stock élevé (>150)", stockEleve, QColor(100, 200, 100), "📦"}
        };

        for (const StockData &sd : stocks) {
            if (sd.count == 0) continue;
            double percent = 100.0 * sd.count / totalStock;
            int barHeight = (sd.count * maxBarHeight) / maxCount;
            if (barHeight < 10) barHeight = 10;

            QWidget *container = new QWidget();
            QVBoxLayout *barLayout = new QVBoxLayout(container);
            barLayout->setSpacing(8);
            barLayout->setAlignment(Qt::AlignHCenter);

            QLabel *countLabel = new QLabel(QString::number(sd.count));
            countLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3a2a20;");
            countLabel->setAlignment(Qt::AlignCenter);
            barLayout->addWidget(countLabel);

            QLabel *percentLabel = new QLabel(QString("%1%").arg(percent, 0, 'f', 1));
            percentLabel->setStyleSheet("font-size: 14px; color: #5b2f1d; font-weight: bold;");
            percentLabel->setAlignment(Qt::AlignCenter);
            barLayout->addWidget(percentLabel);

            QFrame *bar = new QFrame();
            bar->setFixedWidth(80);
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
            )").arg(sd.color.lighter(110).name()).arg(sd.color.name()));
            barLayout->addWidget(bar, 0, Qt::AlignCenter);

            QLabel *nameLabel = new QLabel(sd.icon + " " + sd.name);
            nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3a2a20;");
            nameLabel->setAlignment(Qt::AlignCenter);
            nameLabel->setWordWrap(true);
            barLayout->addWidget(nameLabel);

            barLayout->addStretch();
            chartLayout->addWidget(container);
        }

        stockLayout->addWidget(chartFrame);
        tabWidget->addTab(stockTab, "Par stock");
    }

    mainLayout->addWidget(tabWidget);


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
