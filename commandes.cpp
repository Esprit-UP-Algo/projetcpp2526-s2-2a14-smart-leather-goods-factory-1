#include "commandes.h"
#include "ui_commandes.h"
#include "ajout.h"
#include "modifier.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

commandes::commandes(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::commandes)
{
    ui->setupUi(this);

    // 1. Setup UI defaults
    ui->dateEdit_commande->setDate(QDate::currentDate());

    // 2. Load the data
    loadAllCommandes();

    // 3. Connect signals AFTER loading to avoid flickering
    connect(ui->lineEdit_5, &QLineEdit::textChanged, this, &commandes::applyFilters);
    connect(ui->dateEdit_commande, &QDateEdit::dateChanged, this, &commandes::applyFilters);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->sortItems(4, Qt::AscendingOrder);
}

commandes::~commandes()
{
    delete ui;
}

void commandes::on_pushButton_clicked() {
    Ajout dialog(this);

    while (dialog.exec() == QDialog::Accepted) {
        QString reference = dialog.getReference().trimmed();
        
        if (reference.isEmpty()) {
            QMessageBox::warning(this, "Validation", "La référence ne peut pas être vide.");
            continue;
        }

        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM COMMANDE WHERE REFERENCE = :ref");
        checkQuery.bindValue(":ref", reference);
        
        if (checkQuery.exec() && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QMessageBox::critical(this, "Doublon", "Erreur : Cette référence existe déjà.");
                continue;
            }
        }

        QString client = dialog.getClient();
        QString address = dialog.getAddress();
        QDateTime dateCmd = dialog.getDateOrder();
        QDateTime dateLiv = dialog.getDateDelivery(); // This is the delivery date
        QString etat = dialog.getStatus();
        QString montant = dialog.getAmount();
        QString modePaiement = ""; 

        QSqlQuery q;
        q.prepare("INSERT INTO COMMANDE "
                  "(ID_COMMANDE, REFERENCE, NOM_CLIENT, ADRESSE_LIVRAISON, "
                  " DATE_COMMANDE, DATE_LIVRAISON_PREVUE, ETAT_COMMANDE, MONTANT_TOTAL, MODE_PAIEMENT) "
                  "VALUES (SEQ_COMMANDE.NEXTVAL, :ref, :nom, :addr, :datecmd, :dateliv, :etat, :montant, :mode)");
        q.bindValue(":ref", reference);
        q.bindValue(":nom", client);
        q.bindValue(":addr", address);
        q.bindValue(":datecmd", dateCmd);
        q.bindValue(":dateliv", dateLiv);
        q.bindValue(":etat", etat);
        q.bindValue(":montant", montant);
        q.bindValue(":mode", modePaiement);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur d'insertion", q.lastError().text());
            continue; 
        }

        QString id;
        QSqlQuery qid("SELECT SEQ_COMMANDE.CURRVAL FROM DUAL");
        if (qid.next()) id = qid.value(0).toString();

        // --- BUG FIX: Disable sorting while adding items ---
        ui->tableWidget->setSortingEnabled(false);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(reference));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(client));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(address));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(dateCmd.date().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(dateLiv.date().toString("yyyy-MM-dd"))); // date_prev
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(etat));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(montant));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(modePaiement));

        ui->tableWidget->setSortingEnabled(true);
        // --- END BUG FIX ---

        applyFilters(); // Re-apply filters so the new row is visible if it matches
        QMessageBox::information(this, "Succès", "Commande ajoutée avec succès !");
        break; 
    }
}
void commandes::on_pushButton_2_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString client = ui->tableWidget->item(currentRow, 2)->text();
    QString address = ui->tableWidget->item(currentRow, 3)->text();
    QString dateCmdStr = ui->tableWidget->item(currentRow, 4)->text();
    QString dateLivStr = ui->tableWidget->item(currentRow, 5)->text();
    QString amount = ui->tableWidget->item(currentRow, 7)->text();

    Modifier dialog(this);
    dialog.setInitialData(id, client, address, amount);
    
    QDate dateCmd = QDate::fromString(dateCmdStr, "yyyy-MM-dd");
    QDate dateLiv = QDate::fromString(dateLivStr, "yyyy-MM-dd");
    dialog.setDates(QDateTime(dateCmd, QTime(0,0)), QDateTime(dateLiv, QTime(0,0)));

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery q;
        q.prepare("UPDATE COMMANDE SET "
                  "NOM_CLIENT = :nom, ADRESSE_LIVRAISON = :addr, "
                  "DATE_LIVRAISON_PREVUE = :dateliv, ETAT_COMMANDE = :etat, "
                  "MONTANT_TOTAL = :montant "
                  "WHERE ID_COMMANDE = :id");
        q.bindValue(":nom", dialog.getClient());
        q.bindValue(":addr", dialog.getAddress());
        q.bindValue(":dateliv", dialog.getDateDelivery());
        q.bindValue(":etat", dialog.getStatus());
        q.bindValue(":montant", dialog.getAmount());
        q.bindValue(":id", id);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur", q.lastError().text());
            return;
        }

        // --- BUG FIX: Disable sorting while updating items ---
        ui->tableWidget->setSortingEnabled(false);

        ui->tableWidget->item(currentRow, 2)->setText(dialog.getClient());
        ui->tableWidget->item(currentRow, 3)->setText(dialog.getAddress());
        ui->tableWidget->item(currentRow, 5)->setText(dialog.getDateDelivery().date().toString("yyyy-MM-dd"));
        ui->tableWidget->item(currentRow, 6)->setText(dialog.getStatus());
        ui->tableWidget->item(currentRow, 7)->setText(dialog.getAmount());

        ui->tableWidget->setSortingEnabled(true);
        // --- END BUG FIX ---

        applyFilters(); 
        QMessageBox::information(this, "Succès", "Commande mise à jour !");
    }
}

void commandes::loadAllCommandes() {
    // 1. Completely block sorting and signals during load
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->blockSignals(true); 
    ui->tableWidget->setRowCount(0);

    QSqlQuery q;
    // Removed ORDER BY because the QTableWidget sorting handles the view
    if (!q.exec("SELECT ID_COMMANDE, REFERENCE, NOM_CLIENT, ADRESSE_LIVRAISON, "
                "DATE_COMMANDE, DATE_LIVRAISON_PREVUE, ETAT_COMMANDE, MONTANT_TOTAL, MODE_PAIEMENT "
                "FROM COMMANDE")) {
        QMessageBox::critical(this, "Erreur SQL", q.lastError().text());
        ui->tableWidget->blockSignals(false);
        ui->tableWidget->setSortingEnabled(true);
        return;
    }

    int rowCount = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(rowCount);

        auto fmtDate = [](const QVariant &v) -> QString {
            if (v.isNull()) return "";
            QDateTime dt = v.toDateTime();
            if (dt.isValid()) return dt.date().toString("yyyy-MM-dd");
            QDate d = v.toDate();
            if (d.isValid()) return d.toString("yyyy-MM-dd");
            return v.toString();
        };

        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(q.value(3).toString()));
        ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(fmtDate(q.value(4))));
        ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(fmtDate(q.value(5))));
        ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(q.value(6).toString()));
        ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(q.value(7).toString()));
        ui->tableWidget->setItem(rowCount, 8, new QTableWidgetItem(q.value(8).toString()));
        
        rowCount++;
    }

    ui->tableWidget->blockSignals(false);
    ui->tableWidget->setSortingEnabled(true);

    // IMPORTANT: Only apply filters if there is text in the search box or the user changed the date
    // If you want to see EVERYTHING on start, comment out the next line:
    applyFilters(); 
}

void commandes::on_pushButton_7_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner une commande.");
        return;
    }

    // Extract fields with safe checks
    auto getText = [&](int col) -> QString {
        QTableWidgetItem *it = ui->tableWidget->item(currentRow, col);
        return it ? it->text() : QString();
    };

    QString id = getText(0);
    QString reference = getText(1);
    QString client = getText(2);
    QString address = getText(3);
    QString dateCommande = getText(4);
    QString dateLivraison = getText(5);
    QString etat = getText(6);
    QString montant = getText(7);
    QString modePaiement = getText(8);

    QString html;
    html += "<html><head><meta charset='utf-8'/>";
    html += "<style>";
    html += "body{font-family:Segoe UI, Arial, sans-serif;color:#2a1a12;}";
    html += ".invoice{max-width:800px;margin:0 auto;padding:24px;border:2px solid #b08a6b;border-radius:16px;background:#fffaf5;}";
    html += ".header{display:flex;justify-content:space-between;align-items:center;margin-bottom:12px;}";
    html += ".brand{font-size:22px;font-weight:700;color:#5b2f1d;}";
    html += ".meta{font-size:12px;color:#6b3e26;text-align:right;}";
    html += "h1{font-size:20px;color:#5b2f1d;margin:8px 0 16px 0;text-align:center;}";
    html += "table{width:100%;border-collapse:collapse;margin-top:8px;}";
    html += "th,td{padding:10px;border-bottom:1px solid #e0d2c5;text-align:left;}";
    html += "th{background:#6b3e26;color:#fffaf5;}";
    html += ".section-title{margin-top:16px;color:#6b3e26;font-weight:700;border-bottom:2px solid #b08a6b;display:inline-block;padding-bottom:4px;}";
    html += ".totals{margin-top:16px;float:right;}";
    html += ".totals td{padding:6px 10px;}";
    html += ".totals tr:last-child td{font-weight:700;border-top:2px solid #b08a6b;}";
    html += ".footer{clear:both;text-align:center;margin-top:28px;color:#6b3e26;}";
    html += "</style></head><body>";
    html += "<div class='invoice'>";
    html += "  <div class='header'>";
    html += "    <div class='brand'>SmartLeather - Facture</div>";
    html += "    <div class='meta'>Émise le: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + "<br/>";
    html += "    N°: " + (reference.isEmpty() ? id : reference) + "</div>";
    html += "  </div>";
    html += "  <h1>Détails de la commande</h1>";
    html += "  <table>";
    html += "    <tr><th>Champ</th><th>Valeur</th></tr>";
    html += "    <tr><td>ID Commande</td><td>" + id + "</td></tr>";
    html += "    <tr><td>Référence</td><td>" + reference + "</td></tr>";
    html += "    <tr><td>Client</td><td>" + client + "</td></tr>";
    html += "    <tr><td>Adresse</td><td>" + address + "</td></tr>";
    html += "    <tr><td>Date Commande</td><td>" + dateCommande + "</td></tr>";
    html += "    <tr><td>Date Livraison Prévue</td><td>" + dateLivraison + "</td></tr>";
    html += "    <tr><td>État</td><td>" + etat + "</td></tr>";
    html += "    <tr><td>Mode Paiement</td><td>" + modePaiement + "</td></tr>";
    html += "  </table>";
    html += "  <table class='totals'>";
    html += "    <tr><td>Sous-total</td><td>" + montant + "</td></tr>";
    html += "    <tr><td>Total TTC</td><td>" + montant + "</td></tr>";
    html += "  </table>";
    html += "  <div class='footer'>Merci pour votre confiance.</div>";
    html += "</div></body></html>";

    QString defaultName = "facture_" + (reference.isEmpty() ? id : reference) + ".pdf";
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer la facture", defaultName, "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::PrinterMode::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(12, 12, 12, 12));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Export PDF", "Facture exportée: " + fileName);
}

// make sure u have this in database
// CREATE SEQUENCE SEQ_COMMANDE START WITH 1 INCREMENT BY 1 NOCACHE;

void commandes::filterByReference(const QString &query) {
    for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        QTableWidgetItem *refItem = ui->tableWidget->item(r, 1);
        bool match = refItem && refItem->text().contains(query, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(r, !query.isEmpty() ? !match : false);
    }
}

void commandes::applyFilters() {
    QString refQuery = ui->lineEdit_5->text().trimmed();
    QDate dateQuery = ui->dateEdit_commande->date();

    for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        // Reference Match
        QTableWidgetItem *refItem = ui->tableWidget->item(r, 1);
        bool refMatch = refItem && refItem->text().contains(refQuery, Qt::CaseInsensitive);

        // Date Match
        QTableWidgetItem *dateItem = ui->tableWidget->item(r, 4);
        QDate rowDate = QDate::fromString(dateItem ? dateItem->text() : "", "yyyy-MM-dd");
        bool dateMatch = (rowDate == dateQuery);

        // If you want the search to only filter by date IF the user interacts with it, 
        // you would add more logic here. 
        // Currently: Row is visible ONLY if it matches the reference AND matches the date box.
        bool visible = (refQuery.isEmpty() || refMatch) && dateMatch;
        
        ui->tableWidget->setRowHidden(r, !visible);
    }
}

// Update the old function to call the new logic
void commandes::on_pushButton_10_clicked() {
    applyFilters();
}

void commandes::on_pushButton_3_clicked() {
    // 1. Check if a row is selected
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande à supprimer.");
        return;
    }

    // 2. Get the ID of the command (Column 0) and Reference (Column 1) for the message
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();

    // 3. Ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", 
                                  "Êtes-vous sûr de vouloir supprimer la commande Ref: " + ref + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 4. Delete from Database
        QSqlQuery q;
        q.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE = :id");
        q.bindValue(":id", id);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur de suppression", 
                                  "Impossible de supprimer la commande.\n" + q.lastError().text());
            return;
        }

        // 5. Delete from UI Table
        // We temporarily disable sorting to ensure the correct index is removed
        ui->tableWidget->setSortingEnabled(false);
        ui->tableWidget->removeRow(currentRow);
        ui->tableWidget->setSortingEnabled(true);

        QMessageBox::information(this, "Succès", "La commande a été supprimée avec succès.");
    }
}

void commandes::on_pushButton_4_clicked() {
    loadAllCommandes();
    QMessageBox::information(this, "Actualisation", "Liste des commandes mise à jour.");
}

// Update the lineEdit connection in constructor to use applyFilters()
// connect(ui->lineEdit_5, &QLineEdit::textChanged, this, &commandes::applyFilters);
