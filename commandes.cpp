#include "commandes.h"
#include "ui_commandes.h"
#include "ajout.h"
#include "modifier.h"
#include "login.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>



commandes::commandes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::commandes)
{
    ui->setupUi(this);
    setupTable();
    loadSampleData();
}

commandes::~commandes()
{
    delete ui;
}

void commandes::setupTable()
{
    ui->tableWidget->setColumnCount(9);
    QStringList headers = {"ID Commande", "Client", "Adresse", "Date Commande",
                          "Date Livraison", "État", "Montant", "Mode Paiement", "Employé"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSortingEnabled(true);

    // Set column widths
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 200);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(4, 120);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 100);
    ui->tableWidget->setColumnWidth(7, 120);
    ui->tableWidget->setColumnWidth(8, 150);
}

void commandes::loadSampleData()
{
    // Clear existing rows
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT C.ID_COMMANDE, C.NOMCLIENT, C.ADRESSE_LIVRAISON, TO_CHAR(C.DATE_COMMANDE, 'DD/MM/YYYY'), TO_CHAR(C.DATE_LIVRAISON, 'DD/MM/YYYY'), C.ETAT_COMMANDE, C.MONTANT_TOTALE, C.MODE_PAIMENT, E.NOM || ' ' || E.PRENOM AS EMP_NAME, C.ID_EMPLOYEE FROM COMMANDE C LEFT JOIN EMPLOYEE E ON C.ID_EMPLOYEE = E.ID_EMPLOYEE");
    
    if (query.exec()) {
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString() + " €"));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
            
            QTableWidgetItem *empItem = new QTableWidgetItem(query.value(8).toString());
            empItem->setData(Qt::UserRole, query.value(9)); // Store ID_EMPLOYEE secretly
            ui->tableWidget->setItem(row, 8, empItem);
        }
    } else {
        qDebug() << "Erreur de chargement des commandes:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les commandes depuis la base de données.");
    }

    updateTableColors();
}

void commandes::updateTableColors()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        QTableWidgetItem *etatItem = ui->tableWidget->item(row, 5);
        if (!etatItem) continue;

        QString etat = etatItem->text();

        if (etat == "Livrée") {
            etatItem->setBackground(QColor(200, 255, 200));
            etatItem->setForeground(QColor(0, 100, 0));
        } else if (etat == "Annulée") {
            etatItem->setBackground(QColor(255, 200, 200));
            etatItem->setForeground(QColor(139, 0, 0));
        } else if (etat == "En attente") {
            etatItem->setBackground(QColor(255, 255, 200));
            etatItem->setForeground(QColor(128, 128, 0));
        } else if (etat == "Confirmée") {
            etatItem->setBackground(QColor(200, 230, 255));
            etatItem->setForeground(QColor(0, 0, 139));
        } else if (etat == "En préparation") {
            etatItem->setBackground(QColor(255, 200, 230));
            etatItem->setForeground(QColor(139, 0, 139));
        }
    }
}

bool commandes::validateData(const QString &id, const QString &client,
                            const QString &address, const QString &montant)
{
    if (id.isEmpty() || client.isEmpty() || address.isEmpty() || montant.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return false;
    }

    bool ok;
    double montantValue = montant.toDouble(&ok);
    if (!ok || montantValue <= 0) {
        QMessageBox::warning(this, "Montant invalide", "Veuillez entrer un montant valide (nombre positif).");
        return false;
    }

    return true;
}

// ADD BUTTON
void commandes::on_pushButton_clicked()
{
    Ajout dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString id = dialog.getId();
        QString client = dialog.getClient();
        QString address = dialog.getAddress();
        QString dateCommande = dialog.getDateCommande();
        QString dateLivraison = dialog.getDateLivraison();
        QString etat = dialog.getEtat();
        QString montant = dialog.getMontant();
        QString modePaiement = dialog.getModePaiement();
        QString employeId = dialog.getEmploye();

        if (!validateData(id, client, address, montant)) return;

        QSqlQuery query;
        query.prepare("INSERT INTO COMMANDE (ID_COMMANDE, NOMCLIENT, ADRESSE_LIVRAISON, DATE_COMMANDE, DATE_LIVRAISON, ETAT_COMMANDE, MONTANT_TOTALE, MODE_PAIMENT, ID_EMPLOYEE) "
                      "VALUES (:id, :client, :adresse, TO_DATE(:datec, 'DD/MM/YYYY'), TO_DATE(:datel, 'DD/MM/YYYY'), :etat, :montant, :modep, :empid)");
        query.bindValue(":id", id);
        query.bindValue(":client", client);
        query.bindValue(":adresse", address);
        query.bindValue(":datec", dateCommande);
        query.bindValue(":datel", dateLivraison);
        query.bindValue(":etat", etat);
        query.bindValue(":montant", montant);
        query.bindValue(":modep", modePaiement);
        query.bindValue(":empid", employeId);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Commande ajoutée avec succès !");
            loadSampleData(); // refresh from db
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
        }
    }
}

// EDIT BUTTON
void commandes::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString client = ui->tableWidget->item(currentRow, 1)->text();
    QString address = ui->tableWidget->item(currentRow, 2)->text();
    QString dateCommande = ui->tableWidget->item(currentRow, 3)->text();
    QString dateLivraison = ui->tableWidget->item(currentRow, 4)->text();
    QString etat = ui->tableWidget->item(currentRow, 5)->text();
    QString montant = ui->tableWidget->item(currentRow, 6)->text().replace(" €", "");
    QString modePaiement = ui->tableWidget->item(currentRow, 7)->text();
    QString employeId = ui->tableWidget->item(currentRow, 8)->data(Qt::UserRole).toString();

    Modifier dialog(this);
    dialog.setInitialData(id, client, address, dateCommande, dateLivraison, etat, montant, modePaiement, employeId);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE COMMANDE SET NOMCLIENT = :client, ADRESSE_LIVRAISON = :adresse, DATE_COMMANDE = TO_DATE(:datec, 'DD/MM/YYYY'), "
                      "DATE_LIVRAISON = TO_DATE(:datel, 'DD/MM/YYYY'), ETAT_COMMANDE = :etat, MONTANT_TOTALE = :montant, MODE_PAIMENT = :modep, ID_EMPLOYEE = :empid "
                      "WHERE ID_COMMANDE = :id");
        query.bindValue(":client", dialog.getClient());
        query.bindValue(":adresse", dialog.getAddress());
        query.bindValue(":datec", dialog.getDateCommande());
        query.bindValue(":datel", dialog.getDateLivraison());
        query.bindValue(":etat", dialog.getEtat());
        query.bindValue(":montant", dialog.getMontant());
        query.bindValue(":modep", dialog.getModePaiement());
        query.bindValue(":empid", dialog.getEmploye());
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Commande mise à jour avec succès !");
            loadSampleData();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
    }
}

// DELETE BUTTON - FIXED
void commandes::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString client = ui->tableWidget->item(currentRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer la commande %1 de %2 ?")
                                 .arg(id).arg(client),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Commande supprimée avec succès !");
            loadSampleData();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        }
    }
}

// REFRESH BUTTON
void commandes::on_pushButton_4_clicked()
{
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    loadSampleData();
    QMessageBox::information(this, "Actualisation", "✅ Liste des commandes actualisée !");
}

// SEARCH FUNCTIONALITY
void commandes::filterTable(const QString &idFilter, const QString &dateFilter)
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;

        if (!idFilter.isEmpty()) {
            QString id = ui->tableWidget->item(row, 0)->text();
            if (!id.contains(idFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        if (match && !dateFilter.isEmpty()) {
            QString date = ui->tableWidget->item(row, 3)->text();
            if (!date.contains(dateFilter)) {
                match = false;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

void commandes::on_pushButton_10_clicked()
{
    QString idFilter = ui->lineEdit_5->text();
    QString dateFilter = ui->lineEdit_6->text();
    filterTable(idFilter, dateFilter);
}

void commandes::on_lineEdit_5_textChanged(const QString &text)
{
    QString dateFilter = ui->lineEdit_6->text();
    filterTable(text, dateFilter);
}

void commandes::on_lineEdit_6_textChanged(const QString &text)
{
    QString idFilter = ui->lineEdit_5->text();
    filterTable(idFilter, text);
}
#include <QFile>
#include <QTextStream>

void commandes::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                    "commandes_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
                                                    "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }

    QTextStream out(&file);

    // --- UTF‑8 with BOM (compatible with Excel on Windows) ---
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out.setGenerateByteOrderMark(true);   // this writes the BOM

    // Headers (8 columns)
    QStringList headers = {"ID Commande", "Client", "Adresse", "Date Commande",
                           "Date Livraison", "État", "Montant", "Mode Paiement"};
    for (int i = 0; i < headers.size(); ++i) {
        if (i > 0) out << ";";
        out << headers[i];
    }
    out << "\n";

    // Data rows
    int visibleRows = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (ui->tableWidget->isRowHidden(row)) continue;

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            if (col > 0) out << ";";
            QString cell = ui->tableWidget->item(row, col)->text();
            // Escape quotes and wrap in quotes if needed
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

void commandes::exportToPDF()
{}


void commandes::on_pushButton_9_clicked()
{
    showStatistics();
}

#include "qcustomplot.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QMessageBox>
void commandes::showStatistics()
{
    // 1. Count commands by status
    int enAttente = 0, confirmee = 0, enPreparation = 0, livree = 0, annulee = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString etat = ui->tableWidget->item(row, 5)->text();
            if (etat == "En attente") enAttente++;
            else if (etat == "Confirmée") confirmee++;
            else if (etat == "En préparation") enPreparation++;
            else if (etat == "Livrée") livree++;
            else if (etat == "Annulée") annulee++;
        }
    }

    int total = enAttente + confirmee + enPreparation + livree + annulee;
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune commande à afficher.");
        return;
    }

    int maxCount = qMax(qMax(qMax(qMax(enAttente, confirmee), enPreparation), livree), annulee);
    maxCount = qMax(maxCount, 1);
    const int maxBarHeight = 200;

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des commandes");
    statsDialog->resize(850, 600);
    statsDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f1e7dc;"           // main background
        "   border: 2px dashed #b08a6b;"          // stitched effect
        "   border-radius: 20px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(25, 25, 25, 25);
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

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES COMMANDES");
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
        "   background-color: #fffaf5;"            // light leather
        "   border: 2px solid #b08a6b;"
        "   border-radius: 20px;"
        "   padding: 20px;"
        "}"
    );
    QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
    chartLayout->setSpacing(20);
    chartLayout->setAlignment(Qt::AlignBottom);

    struct StatusData {
        QString name;
        int count;
        QColor color;
        QString icon;
    };

    QList<StatusData> statuses = {
        {"En attente", enAttente, QColor(212, 172, 130), "⏳"},   // light brown
        {"Confirmée",  confirmee, QColor(107, 62, 38), "✅"},     // dark brown
        {"En préparation", enPreparation, QColor(160, 110, 70), "⚙️"}, // medium brown
        {"Livrée",     livree,    QColor(74, 39, 23), "📦"},       // very dark brown
        {"Annulée",    annulee,   QColor(180, 90, 50), "❌"}       // rust
    };

    for (const StatusData &sd : statuses) {
        if (sd.count == 0) continue;

        double percent = 100.0 * sd.count / total;
        int barHeight = (sd.count * maxBarHeight) / maxCount;
        if (barHeight < 10 && sd.count > 0) barHeight = 10;

        QWidget *barContainer = new QWidget();
        QVBoxLayout *barLayout = new QVBoxLayout(barContainer);
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
        chartLayout->addWidget(barContainer);
    }

    mainLayout->addWidget(chartFrame, 1);

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
// NAVIGATION METHODS
void commandes::on_pushButton_5_clicked()
{
    hide();
    login *lg = new login(this);
    lg->show();
}

void commandes::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
    pl->show();
}

void commandes::on_pushButton_12_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void commandes::on_pushButton_19_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
    pl->show();
}

void commandes::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void commandes::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void commandes::on_pushButton_22_clicked()
{
    hide();
    Matieres *mm = new Matieres(this);
    mm->show();
}

void commandes::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(this);
    ss->show();
}

void commandes::on_pushButton_17_clicked()
{
    hide();
    login *lg = new login(this);
    lg->show();
}
