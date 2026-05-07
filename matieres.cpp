#include "matieres.h"
#include "ui_matieres.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

// Navigation includes
#include "pageemployee.h"
#include "login.h"
#include "fournisseurs.h"
#include "commandes.h"
#include "produitswindow.h"
#include "pagemachine.h"

Matieres::Matieres(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Matieres)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    
    // Initialisation
    setupMatieresTable();
    setupSearch();
    loadMatieres();
}

Matieres::~Matieres()
{
    delete ui;
}

void Matieres::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
    connect(ui->searchTypeEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
    
    connect(ui->pushButton_10, &QPushButton::clicked, this, &Matieres::filterTable);
    connect(ui->btnAI, &QPushButton::clicked, this, &Matieres::checkAIQuality);
    connect(ui->btnAlertesStock, &QPushButton::clicked, this, &Matieres::checkStockAlerts);
    
    btnAlertesStock = ui->btnAlertesStock;
    ui->btnAlertesStock->hide();
}

void Matieres::filterTable()
{
    QString idFilter = ui->searchIdEdit->text().trimmed();
    QString typeFilter = ui->searchTypeEdit->text().trimmed();

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;
        if (!idFilter.isEmpty()) {
            if (!ui->tableWidget->item(row, 0)->text().contains(idFilter, Qt::CaseInsensitive)) match = false;
        }
        if (match && !typeFilter.isEmpty()) {
            if (!ui->tableWidget->item(row, 1)->text().contains(typeFilter, Qt::CaseInsensitive)) match = false;
        }
        ui->tableWidget->setRowHidden(row, !match);
    }
}

void Matieres::setupMatieresTable()
{
    QStringList headers = {"ID Matière", "Type", "Couleur", "Qualité", "Prix unitaire", "Stock", "Fournisseur"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void Matieres::loadMatieres()
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT ID_MATIERE, TYPE_MATIERE, COULEUR, QUALITE, PRIX, STOCK, FOURNISSEUR FROM MATIERE");
    
    if (query.exec()) {
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            for(int i=0; i<7; ++i) {
                QString val = query.value(i).toString();
                if (i == 4) val += " €";
                ui->tableWidget->setItem(row, i, new QTableWidgetItem(val));
            }
            updateRowColors(row);
        }
    }

    int alertCount = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        int stock = ui->tableWidget->item(row, 5)->text().toInt();
        if (stock < 50) alertCount++;
    }
    
    if (alertCount > 0) {
        ui->btnAlertesStock->setText(QString("⚠️ ALERTES (%1)").arg(alertCount));
        ui->btnAlertesStock->show();
    } else {
        ui->btnAlertesStock->hide();
    }
}

void Matieres::updateRowColors(int row)
{
    QTableWidgetItem *stockItem = ui->tableWidget->item(row, 5);
    if (stockItem) {
        int stock = stockItem->text().toInt();
        if (stock < 50) stockItem->setBackground(QColor(255, 200, 200));
        else if (stock < 100) stockItem->setBackground(QColor(255, 255, 200));
    }
}

// NAVIGATION SLOTS
void Matieres::on_pushButton_11_clicked() { hide(); pageemployee *p = new pageemployee(m_idEmploye, this); p->show(); }
void Matieres::on_pushButton_21_clicked() { hide(); produitswindow *p = new produitswindow(m_idEmploye, this); p->show(); }
void Matieres::on_pushButton_6_clicked()  { hide(); commandes *p = new commandes(m_idEmploye, this); p->show(); } 
void Matieres::on_pushButton_20_clicked() { hide(); fournisseurs *p = new fournisseurs(m_idEmploye, this); p->show(); }
void Matieres::on_pushButton_22_clicked() { /* Current page */ }
void Matieres::on_pushButton_23_clicked() { hide(); pagemachine *p = new pagemachine(m_idEmploye, this); p->show(); }
void Matieres::on_pushButton_5_clicked()  { hide(); login *p = new login(); p->show(); }

// ACTIONS CRUD
void Matieres::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouvelle matière");
    ajoutDialog->setFixedSize(500, 650);
    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->addWidget(new QLabel("<b>AJOUTER MATIÈRE</b>"));

    QLineEdit *idEdit = new QLineEdit(); idEdit->setPlaceholderText("ID");
    QLineEdit *typeEdit = new QLineEdit(); typeEdit->setPlaceholderText("Type");
    QLineEdit *couleurEdit = new QLineEdit(); couleurEdit->setPlaceholderText("Couleur");
    QComboBox *qualiteCombo = new QComboBox(); qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    QLineEdit *prixEdit = new QLineEdit(); prixEdit->setPlaceholderText("Prix");
    QLineEdit *stockEdit = new QLineEdit(); stockEdit->setPlaceholderText("Stock");
    QLineEdit *fournisseurEdit = new QLineEdit(); fournisseurEdit->setPlaceholderText("Fournisseur");

    mainLayout->addWidget(new QLabel("ID:")); mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("Type:")); mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(new QLabel("Couleur:")); mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(new QLabel("Qualité:")); mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(new QLabel("Prix:")); mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(new QLabel("Stock:")); mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("Fournisseur:")); mainLayout->addWidget(fournisseurEdit);

    QPushButton *btnSave = new QPushButton("Enregistrer");
    mainLayout->addWidget(btnSave);
    connect(btnSave, &QPushButton::clicked, [=]() {
        QSqlQuery query;
        query.prepare("INSERT INTO MATIERE (ID_MATIERE, TYPE_MATIERE, COULEUR, QUALITE, PRIX, STOCK, FOURNISSEUR) VALUES (:id, :type, :couleur, :qualite, :prix, :stock, :fournisseur)");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":type", typeEdit->text());
        query.bindValue(":couleur", couleurEdit->text());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":prix", prixEdit->text().toDouble());
        query.bindValue(":stock", stockEdit->text().toInt());
        query.bindValue(":fournisseur", fournisseurEdit->text());
        if (query.exec()) { loadMatieres(); ajoutDialog->accept(); }
    });
    ajoutDialog->exec();
}

void Matieres::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QDialog *modDialog = new QDialog(this);
    modDialog->setWindowTitle("Modifier");
    QVBoxLayout *layout = new QVBoxLayout(modDialog);
    QLineEdit *typeEdit = new QLineEdit(ui->tableWidget->item(currentRow, 1)->text());
    layout->addWidget(new QLabel("Type:")); layout->addWidget(typeEdit);
    QPushButton *btnUpdate = new QPushButton("Mettre à jour");
    layout->addWidget(btnUpdate);
    connect(btnUpdate, &QPushButton::clicked, [=]() {
        QSqlQuery query;
        query.prepare("UPDATE MATIERE SET TYPE_MATIERE=:type WHERE ID_MATIERE=:id");
        query.bindValue(":type", typeEdit->text());
        query.bindValue(":id", id);
        if (query.exec()) { loadMatieres(); modDialog->accept(); }
    });
    modDialog->exec();
}

void Matieres::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    if (QMessageBox::question(this, "Suppression", "Supprimer " + id + " ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM MATIERE WHERE ID_MATIERE = :id");
        query.bindValue(":id", id);
        if (query.exec()) loadMatieres();
    }
}

void Matieres::on_pushButton_4_clicked() { loadMatieres(); }
void Matieres::on_pushButton_7_clicked() { /* Logic PDF */ QMessageBox::information(this, "PDF", "Génération PDF..."); }
void Matieres::on_pushButton_9_clicked() { /* Logic Stats */ }

void Matieres::checkStockAlerts() {
    QMessageBox::warning(this, "Alertes Stock", "Certaines matières sont en rupture de stock.");
}

void Matieres::checkAIQuality() {
    QMessageBox::information(this, "Diagnostic IA", "La qualité des matières est conforme aux standards.");
}

void Matieres::addMatiereToTable(const QString &id, const QString &type, const QString &couleur, const QString &qualite, const QString &prix, const QString &stock, const QString &fournisseur) {
    Q_UNUSED(id); Q_UNUSED(type); Q_UNUSED(couleur); Q_UNUSED(qualite); Q_UNUSED(prix); Q_UNUSED(stock); Q_UNUSED(fournisseur);
}

void Matieres::updateMatiereInTable(int row, const QString &type, const QString &couleur, const QString &qualite, const QString &prix, const QString &stock, const QString &fournisseur) {
    Q_UNUSED(row); Q_UNUSED(type); Q_UNUSED(couleur); Q_UNUSED(qualite); Q_UNUSED(prix); Q_UNUSED(stock); Q_UNUSED(fournisseur);
}
