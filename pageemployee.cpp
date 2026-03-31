#include "pageemployee.h"
#include "ui_pageemployee.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
#include <QDateEdit>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "commandes.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>



pageemployee::pageemployee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemployee)
{
    ui->setupUi(this);
    setupEmployeesTable();
    loadEmployees();
    setupSearch();
}

pageemployee::~pageemployee()
{
    delete ui;
}

void pageemployee::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pageemployee::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pageemployee::filterTable);
}

void pageemployee::filterTable()
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

void pageemployee::setupEmployeesTable()
{
    QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Salaire (€)", "Date d'embauche", "Statut"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 120);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 120);
    ui->tableWidget->setColumnWidth(6, 100);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void pageemployee::loadEmployees()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYEE, NOM, PRENOM, POSTE, SALAIRE, TO_CHAR(DATE_DE_NAISSANCE, 'DD/MM/YYYY'), NIVEAU FROM EMPLOYEE");
    
    if (query.exec()) {
        while (query.next()) {
            addEmployeeToTable(
                query.value(0).toString(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                query.value(6).toString()
            );
        }
    } else {
        qDebug() << "Erreur de chargement des employés:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les employés depuis la base de données.");
    }
}

void pageemployee::addEmployeeToTable(const QString &id, const QString &nom,
                                      const QString &prenom, const QString &poste,
                                      const QString &salaire, const QString &dateEmbauche,
                                      const QString &statut)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(prenom));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(poste));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(salaire + " €"));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(dateEmbauche));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(statut));

    updateRowColors(row);
}

void pageemployee::updateRowColors(int row)
{
    QTableWidgetItem *statutItem = ui->tableWidget->item(row, 6);
    if (statutItem) {
        QString statut = statutItem->text();
        if (statut == "Actif") {
            statutItem->setBackground(QColor(200, 255, 200));
            statutItem->setForeground(QColor(0, 100, 0));
        } else if (statut == "Inactif" || statut == "Congé") {
            statutItem->setBackground(QColor(255, 200, 200));
            statutItem->setForeground(QColor(139, 0, 0));
        } else if (statut == "Formation") {
            statutItem->setBackground(QColor(255, 255, 200));
            statutItem->setForeground(QColor(128, 128, 0));
        }
    }
}

void pageemployee::updateEmployeeInTable(int row, const QString &nom,
                                         const QString &prenom, const QString &poste,
                                         const QString &salaire, const QString &dateEmbauche,
                                         const QString &statut)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(nom);
        ui->tableWidget->item(row, 2)->setText(prenom);
        ui->tableWidget->item(row, 3)->setText(poste);
        ui->tableWidget->item(row, 4)->setText(salaire + " €");
        ui->tableWidget->item(row, 5)->setText(dateEmbauche);
        ui->tableWidget->item(row, 6)->setText(statut);

        updateRowColors(row);
    }
}

void pageemployee::on_pushButton_6_clicked()
{
    hide();
    commandes *lg = new commandes(this);
    lg->show();
}

void pageemployee::on_pushButton_5_clicked()
{
    hide();
    login *lg = new login(this);
    lg->show();
}

void pageemployee::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void pageemployee::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void pageemployee::on_pushButton_22_clicked()
{
    hide();
    Matieres *pddd = new Matieres(this);
    pddd->show();
}

void pageemployee::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(this);
    ss->show();
}

void pageemployee::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Ajouter un employé");
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
        "QLineEdit, QComboBox, QDateEdit {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QDateEdit:focus { border: 2px solid #6b3e26; }"
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

    QLabel *header = new QLabel("AJOUTER UN EMPLOYÉ");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID employé (ex: E001)");
    idEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom");
    nomEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *prenomEdit = new QLineEdit();
    prenomEdit->setPlaceholderText("Prénom");
    prenomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur", "Responsable Production", "Opérateur", "Comptable", "Commercial", "Magasinier", "Stagiaire"});

    QLineEdit *salaireEdit = new QLineEdit();
    salaireEdit->setPlaceholderText("Salaire (€)");
    salaireEdit->setAlignment(Qt::AlignCenter);

    QDateEdit *dateEmbaucheEdit = new QDateEdit(QDate::currentDate());
    dateEmbaucheEdit->setDisplayFormat("dd/MM/yyyy");
    dateEmbaucheEdit->setCalendarPopup(true);

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Congé", "Formation", "Inactif"});

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("👤 Nom :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("👤 Prénom :"));
    mainLayout->addWidget(prenomEdit);
    mainLayout->addWidget(new QLabel("💼 Poste :"));
    mainLayout->addWidget(posteCombo);
    mainLayout->addWidget(new QLabel("💰 Salaire :"));
    mainLayout->addWidget(salaireEdit);
    mainLayout->addWidget(new QLabel("📅 Date d'embauche :"));
    mainLayout->addWidget(dateEmbaucheEdit);
    mainLayout->addWidget(new QLabel("⚡ Statut :"));
    mainLayout->addWidget(statutCombo);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, nomEdit, prenomEdit,
                                              posteCombo, salaireEdit, dateEmbaucheEdit,
                                              statutCombo, this]() {
        if (idEdit->text().isEmpty() || nomEdit->text().isEmpty() ||
            prenomEdit->text().isEmpty() || salaireEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                               "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        bool ok;
        double salaire = salaireEdit->text().toDouble(&ok);
        if (!ok || salaire <= 0) {
            QMessageBox::warning(ajoutDialog, "Salaire invalide",
                               "Veuillez entrer un salaire valide (nombre positif).");
            return;
        }

        QString formattedSalaire = QString::number(salaire, 'f', 2);
        QString dateFormatted = dateEmbaucheEdit->date().toString("dd/MM/yyyy");

        QSqlQuery query;
        query.prepare("INSERT INTO EMPLOYEE (ID_EMPLOYEE, NOM, PRENOM, POSTE, SALAIRE, DATE_DE_NAISSANCE, NIVEAU) "
                      "VALUES (:id, :nom, :prenom, :poste, :salaire, TO_DATE(:dateEmbauche, 'DD/MM/YYYY'), :statut)");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":prenom", prenomEdit->text());
        query.bindValue(":poste", posteCombo->currentText());
        query.bindValue(":salaire", formattedSalaire);
        query.bindValue(":dateEmbauche", dateFormatted);
        query.bindValue(":statut", statutCombo->currentText());

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ L'employé a été ajouté avec succès!");
            loadEmployees();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void pageemployee::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un employé à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString nom = ui->tableWidget->item(currentRow, 1)->text();
    QString prenom = ui->tableWidget->item(currentRow, 2)->text();
    QString poste = ui->tableWidget->item(currentRow, 3)->text();
    QString salaire = ui->tableWidget->item(currentRow, 4)->text().replace(" €", "");
    QString dateEmbauche = ui->tableWidget->item(currentRow, 5)->text();
    QString statut = ui->tableWidget->item(currentRow, 6)->text();

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier employé");
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
        "QLineEdit, QComboBox, QDateEdit {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QDateEdit:focus { border: 2px solid #6b3e26; }"
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

    QLabel *header = new QLabel("MODIFIER EMPLOYÉ");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(id);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setText(nom);
    nomEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *prenomEdit = new QLineEdit();
    prenomEdit->setText(prenom);
    prenomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur", "Responsable Production", "Opérateur", "Comptable", "Commercial", "Magasinier", "Stagiaire"});
    posteCombo->setCurrentText(poste);

    QLineEdit *salaireEdit = new QLineEdit();
    salaireEdit->setText(salaire);
    salaireEdit->setAlignment(Qt::AlignCenter);

    QDateEdit *dateEmbaucheEdit = new QDateEdit(QDate::fromString(dateEmbauche, "dd/MM/yyyy"));
    dateEmbaucheEdit->setDisplayFormat("dd/MM/yyyy");
    dateEmbaucheEdit->setCalendarPopup(true);

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Congé", "Formation", "Inactif"});
    statutCombo->setCurrentText(statut);

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("👤 Nom :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("👤 Prénom :"));
    mainLayout->addWidget(prenomEdit);
    mainLayout->addWidget(new QLabel("💼 Poste :"));
    mainLayout->addWidget(posteCombo);
    mainLayout->addWidget(new QLabel("💰 Salaire :"));
    mainLayout->addWidget(salaireEdit);
    mainLayout->addWidget(new QLabel("📅 Date d'embauche :"));
    mainLayout->addWidget(dateEmbaucheEdit);
    mainLayout->addWidget(new QLabel("⚡ Statut :"));
    mainLayout->addWidget(statutCombo);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, id, nomEdit, prenomEdit,
                                              posteCombo, salaireEdit, dateEmbaucheEdit,
                                              statutCombo, this]() {
        if (nomEdit->text().isEmpty() || prenomEdit->text().isEmpty() || salaireEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants",
                               "Veuillez remplir tous les champs.");
            return;
        }

        bool ok;
        double salaire = salaireEdit->text().toDouble(&ok);
        if (!ok || salaire <= 0) {
            QMessageBox::warning(modifierDialog, "Salaire invalide",
                               "Veuillez entrer un salaire valide (nombre positif).");
            return;
        }

        QString formattedSalaire = QString::number(salaire, 'f', 2);
        QString dateFormatted = dateEmbaucheEdit->date().toString("dd/MM/yyyy");

        QSqlQuery query;
        query.prepare("UPDATE EMPLOYEE SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                      "SALAIRE = :salaire, DATE_DE_NAISSANCE = TO_DATE(:dateEmbauche, 'DD/MM/YYYY'), NIVEAU = :statut "
                      "WHERE ID_EMPLOYEE = :id");
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":prenom", prenomEdit->text());
        query.bindValue(":poste", posteCombo->currentText());
        query.bindValue(":salaire", formattedSalaire);
        query.bindValue(":dateEmbauche", dateFormatted);
        query.bindValue(":statut", statutCombo->currentText());
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ L'employé a été mis à jour avec succès!");
            loadEmployees();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
    modifierDialog->deleteLater();
}

void pageemployee::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString nom = ui->tableWidget->item(currentRow, 1)->text();
    QString prenom = ui->tableWidget->item(currentRow, 2)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer l'employé '%1 %2' (ID: %3) ?")
                                 .arg(prenom).arg(nom).arg(id),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM EMPLOYEE WHERE ID_EMPLOYEE = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Employé supprimé avec succès!");
            loadEmployees();
            qDebug() << "Employé supprimé:" << id << "-" << prenom << nom;
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        }
    }
}

void pageemployee::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNomEdit->clear();
    loadEmployees();
    QMessageBox::information(this, "Actualisation", "✅ Liste des employés actualisée !");
}

void pageemployee::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                    "employes_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
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

    QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Salaire (€)",
                           "Date d'embauche", "Statut"};
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
void pageemployee::on_pushButton_9_clicked()
{
    int actif = 0, conge = 0, formation = 0, inactif = 0;
    double totalSalaires = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString statut = ui->tableWidget->item(row, 6)->text();
            QString salaireStr = ui->tableWidget->item(row, 4)->text().replace(" €", "");
            double salaire = salaireStr.toDouble();

            if (statut == "Actif") actif++;
            else if (statut == "Congé") conge++;
            else if (statut == "Formation") formation++;
            else if (statut == "Inactif") inactif++;

            totalSalaires += salaire;
        }
    }

    int total = actif + conge + formation + inactif;
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun employé à afficher.");
        return;
    }

    double salaireMoyen = totalSalaires / total;

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des employés");
    statsDialog->resize(850, 650);
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

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES EMPLOYÉS");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *totalLabel = new QLabel(QString("Total: %1").arg(total));
    totalLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #fffaf5;"
        "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
        "border: 1px solid #c49a6c;"
    );
    headerLayout->addWidget(totalLabel);

    mainLayout->addWidget(headerFrame);

    QFrame *chartFrame = new QFrame();
    chartFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #fffaf5;"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 20px;"
        "   padding: 20px;"
        "}"
    );
    QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
    chartLayout->setSpacing(20);
    chartLayout->setAlignment(Qt::AlignBottom);

    int maxCount = qMax(qMax(qMax(actif, conge), formation), inactif);
    maxCount = qMax(maxCount, 1);
    const int maxBarHeight = 200;

    struct StatusData { QString name; int count; QColor color; QString icon; };
    QList<StatusData> statuses = {
        {"Actif", actif, QColor(100, 200, 100), "👔"},
        {"Congé", conge, QColor(255, 200, 200), "🏖️"},
        {"Formation", formation, QColor(255, 255, 100), "📚"},
        {"Inactif", inactif, QColor(200, 200, 200), "⛔"}
    };

    for (const StatusData &sd : statuses) {
        if (sd.count == 0) continue;
        double percent = 100.0 * sd.count / total;
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
        barLayout->addWidget(nameLabel);

        barLayout->addStretch();
        chartLayout->addWidget(container);
    }

    mainLayout->addWidget(chartFrame);

    QFrame *infoFrame = new QFrame();
    infoFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #fffaf5;"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   padding: 15px;"
        "}"
    );
    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);

    QLabel *infoTitle = new QLabel("💰 INFORMATIONS SALARIALES");
    infoTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #6b3e26; margin-bottom: 8px;");
    infoLayout->addWidget(infoTitle);

    QLabel *totalSalLabel = new QLabel(QString("Masse salariale totale: %1 €").arg(totalSalaires, 0, 'f', 2));
    totalSalLabel->setStyleSheet("font-size: 14px; color: #3a2a20;");
    infoLayout->addWidget(totalSalLabel);

    QLabel *moyenLabel = new QLabel(QString("Salaire moyen: %1 €").arg(salaireMoyen, 0, 'f', 2));
    moyenLabel->setStyleSheet("font-size: 14px; color: #3a2a20;");
    infoLayout->addWidget(moyenLabel);

    mainLayout->addWidget(infoFrame);

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

void pageemployee::on_PushButton_9_clicked()
{

}
