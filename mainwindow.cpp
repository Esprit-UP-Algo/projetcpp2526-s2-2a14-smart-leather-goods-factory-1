#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ajoutfournisseur.h"
#include "modifierfournisseur.h"

#include <QDate>
#include <QDateTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpression>

namespace {
QString detailsErreurSql(const QSqlError &err)
{
    return QString("Driver: %1\nBase: %2\nCode natif: %3\nTexte: %4")
        .arg(err.driverText(), err.databaseText(), err.nativeErrorCode(), err.text());
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Theme coherent with the former commandes screen.
    this->setStyleSheet(
        "QMainWindow, QWidget {"
        "    background-color: #f1e7dc;"
        "    font-family: 'Segoe UI';"
        "    font-size: 13px;"
        "    color: #3a2a20;"
        "}"
        "QLabel { color: #3a2a20; }"
        "QGroupBox {"
        "    background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #fff6ec,stop:1 #ead7c6);"
        "    border: 2px dashed #b08a6b;"
        "    border-radius: 14px;"
        "    margin-top: 22px;"
        "}"
        "QLineEdit, QDateEdit, QComboBox {"
        "    background-color: rgba(255,250,245,0.6);"
        "    border: 1px solid #c6a88d;"
        "    border-radius: 6px;"
        "    padding: 6px;"
        "    color: #3a2a20;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus {"
        "    border: 2px solid #6b3e26;"
        "    background-color: #fffaf5;"
        "}"
        "QTableWidget {"
        "    background-color: #fffaf5;"
        "    border: 2px solid #b08a6b;"
        "    border-radius: 14px;"
        "    gridline-color: #e0d2c5;"
        "}"
        "QHeaderView::section {"
        "    background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #6b3e26,stop:1 #4a2717);"
        "    color: #fffaf5;"
        "    padding: 8px;"
        "    border: none;"
        "    font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #d8b59c;"
        "    color: #2a1a12;"
        "}"
        "QPushButton {"
        "    background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #7a4a2e,stop:1 #5b2f1d);"
        "    border: 2px solid #3a1f14;"
        "    border-radius: 10px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "    color: #fffaf5;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #8b5a3a,stop:1 #6b3e26);"
        "}"
        "QPushButton#pushButton_3 { background-color: #6f8f3d; border-color: #4c6328; }"
        "QPushButton#pushButton_4 { background-color: #d07a2d; border-color: #6b3e26; }"
        "QPushButton#pushButton_5 { background-color: #a23b2a; border-color: #6e2418; }"
        "QPushButton#pushButton_6 { background-color: #a47148; border-color: #6b3e26; }"
        );

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({
        "ID fournisseur",
        "Nom",
        "Type matiere",
        "Telephone",
        "Adresse",
        "Delai livraison",
        "Qualite",
        "Statut"
    });

    ui->label_9->setText("ID fournisseur :");
    ui->label_10->setText("Nom fournisseur :");

    if (resoudreStructureFournisseurs()) {
        chargerFournisseurs();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked() {
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return;
    }

    AjoutFournisseur dialog(this);
    const QString idAuto = genererIdFournisseur();
    if (idAuto.isEmpty()) {
        QMessageBox::warning(this, "ID auto", "Impossible de generer automatiquement IDFOURNISSEUR.");
        return;
    }
    dialog.setIdFournisseur(idAuto);

    if (dialog.exec() == QDialog::Accepted) {
        const QString idFournisseur = dialog.getIdFournisseur().trimmed();
        const QString nom = dialog.getNomFournisseur().trimmed();
        const QString type = dialog.getTypeMatiere().trimmed();
        const QString telephone = dialog.getTelephone().trimmed();
        const QString adresse = dialog.getAdresse().trimmed();
        const QString delai = dialog.getDelaiLivraison().toString("dd/MM/yyyy");
        const QString qualite = dialog.getQualiteMatiere().trimmed();
        const QString statut = dialog.getStatut().trimmed();

        if (idFournisseur.isEmpty() || nom.isEmpty() || type.isEmpty()) {
            QMessageBox::warning(this, "Validation", "IDFOURNISSEUR, NOM et TYPE_MATIERE sont obligatoires.");
            return;
        }

        if (!validerTelephone(telephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE doit etre numerique.");
            return;
        }

        QSqlQuery query;
        query.prepare(
            QString("INSERT INTO %1 "
                    "(IDFOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT) "
                    "VALUES (:id, :nom, :type, TO_NUMBER(NULLIF(:telephone, '')), NULLIF(:adresse, ''), TO_DATE(:delai, 'DD/MM/YYYY'), NULLIF(:qualite, ''), NULLIF(:statut, ''))")
                .arg(m_tableFournisseurs)
            );

        query.bindValue(":id", idFournisseur);
        query.bindValue(":nom", nom);
        query.bindValue(":type", type);
        query.bindValue(":telephone", telephone);
        query.bindValue(":adresse", adresse);
        query.bindValue(":delai", delai);
        query.bindValue(":qualite", qualite);
        query.bindValue(":statut", statut);

        if (!query.exec()) {
            afficherErreurSql("Ajout fournisseur", detailsErreurSql(query.lastError()));
            return;
        }

        QMessageBox::information(this, "Succes", "Fournisseur ajoute avec succes.");
        chargerFournisseurs();
    }
}

void MainWindow::on_pushButton_4_clicked() {
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return;
    }

    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection", "Veuillez selectionner un fournisseur a modifier.");
        return;
    }

    const QString idFournisseur = ui->tableWidget->item(currentRow, 0)->text();
    const QString nom = ui->tableWidget->item(currentRow, 1)->text();
    const QString type = ui->tableWidget->item(currentRow, 2)->text();
    const QString telephone = ui->tableWidget->item(currentRow, 3)->text();
    const QString adresse = ui->tableWidget->item(currentRow, 4)->text();
    const QString delaiStr = ui->tableWidget->item(currentRow, 5)->text();
    const QString qualite = ui->tableWidget->item(currentRow, 6)->text();
    const QString statut = ui->tableWidget->item(currentRow, 7)->text();

    QDate delai = QDate::fromString(delaiStr, "dd/MM/yyyy");
    if (!delai.isValid()) {
        delai = QDate::currentDate().addDays(30);
    }

    ModifierFournisseur dialog(this);
    dialog.setInitialData(nom, type, telephone, adresse, delai, qualite, statut);

    if (dialog.exec() == QDialog::Accepted) {
        const QString newNom = dialog.getNomFournisseur().trimmed();
        const QString newType = dialog.getTypeMatiere().trimmed();
        const QString newTelephone = dialog.getTelephone().trimmed();
        const QString newAdresse = dialog.getAdresse().trimmed();
        const QString newDelai = dialog.getDelaiLivraison().toString("dd/MM/yyyy");
        const QString newQualite = dialog.getQualiteMatiere().trimmed();
        const QString newStatut = dialog.getStatut().trimmed();

        if (newNom.isEmpty() || newType.isEmpty()) {
            QMessageBox::warning(this, "Validation", "NOM et TYPE_MATIERE sont obligatoires.");
            return;
        }

        if (!validerTelephone(newTelephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE doit etre numerique.");
            return;
        }

        QSqlQuery query;
        query.prepare(
            QString("UPDATE %1 SET "
                    "NOM = :nom, "
                    "TYPE_MATIERE = :type, "
                    "TELEPHONE = TO_NUMBER(NULLIF(:telephone, '')), "
                    "ADRESSE = NULLIF(:adresse, ''), "
                    "DELAI_LIVRAISON = TO_DATE(:delai, 'DD/MM/YYYY'), "
                    "QUALITE = NULLIF(:qualite, ''), "
                    "STATUT = NULLIF(:statut, '') "
                    "WHERE IDFOURNISSEUR = :id")
                .arg(m_tableFournisseurs)
            );

        query.bindValue(":nom", newNom);
        query.bindValue(":type", newType);
        query.bindValue(":telephone", newTelephone);
        query.bindValue(":adresse", newAdresse);
        query.bindValue(":delai", newDelai);
        query.bindValue(":qualite", newQualite);
        query.bindValue(":statut", newStatut);
        query.bindValue(":id", idFournisseur);

        if (!query.exec()) {
            afficherErreurSql("Modification fournisseur", detailsErreurSql(query.lastError()));
            return;
        }

        QMessageBox::information(this, "Succes", "Fournisseur mis a jour.");
        chargerFournisseurs();
    }
}

void MainWindow::on_pushButton_5_clicked() {
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return;
    }

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Selection", "Veuillez selectionner un fournisseur a supprimer.");
        return;
    }

    const QString idFournisseur = ui->tableWidget->item(currentRow, 0)->text();

    const auto reply = QMessageBox::question(this,
                                             "Confirmation",
                                             QString("Supprimer le fournisseur %1 ?").arg(idFournisseur),
                                             QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE IDFOURNISSEUR = :id").arg(m_tableFournisseurs));
    query.bindValue(":id", idFournisseur);

    if (!query.exec()) {
        afficherErreurSql("Suppression fournisseur", query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes", "Fournisseur supprime.");
    chargerFournisseurs();
}

void MainWindow::on_pushButton_6_clicked() {
    chargerFournisseurs();
}

void MainWindow::on_pushButton_7_clicked() {
    const QString idFiltre = ui->lineEdit->text().trimmed();
    const QString nomFiltre = ui->lineEdit_2->text().trimmed();
    chargerFournisseurs(idFiltre, nomFiltre);
}

bool MainWindow::resoudreStructureFournisseurs()
{
    if (!QSqlDatabase::database().isOpen()) {
        afficherErreurSql("Structure fournisseurs", "La connexion base de donnees est fermee.");
        return false;
    }

    if (!m_tableFournisseurs.isEmpty()) {
        return true;
    }

    QSqlQuery query;
    const QStringList candidates = {"TABLE_FOURNISSEURS", "FOURNISSEUR", "FOURNISSEURS"};
    for (const QString &tableName : candidates) {
        query.prepare("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME = :table_name");
        query.bindValue(":table_name", tableName);
        if (query.exec() && query.next()) {
            m_tableFournisseurs = query.value(0).toString();
            return true;
        }
    }

    QSqlQuery synQuery;
    for (const QString &synName : candidates) {
        synQuery.prepare("SELECT TABLE_OWNER, TABLE_NAME FROM USER_SYNONYMS WHERE SYNONYM_NAME = :syn_name");
        synQuery.bindValue(":syn_name", synName);
        if (synQuery.exec() && synQuery.next()) {
            const QString owner = synQuery.value(0).toString();
            const QString tableName = synQuery.value(1).toString();
            m_tableFournisseurs = owner + "." + tableName;
            return true;
        }
    }

    afficherErreurSql(
        "Structure fournisseurs",
        "Aucune table TABLE_FOURNISSEURS/FOURNISSEUR/FOURNISSEURS trouvee dans le schema courant."
        );
    return false;
}

bool MainWindow::chargerFournisseurs(const QString &idFiltre, const QString &nomFiltre)
{
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return false;
    }

    QString sql =
        "SELECT IDFOURNISSEUR, NOM, TYPE_MATIERE, TO_CHAR(TELEPHONE), ADRESSE, "
        "TO_CHAR(DELAI_LIVRAISON, 'DD/MM/YYYY') AS DELAI_LIVRAISON, QUALITE, STATUT "
        "FROM %1 WHERE 1=1";
    sql = sql.arg(m_tableFournisseurs);

    if (!idFiltre.isEmpty()) {
        sql += " AND IDFOURNISSEUR = :id";
    }
    if (!nomFiltre.isEmpty()) {
        sql += " AND UPPER(NOM) LIKE :nom";
    }
    sql += " ORDER BY IDFOURNISSEUR";

    QSqlQuery query;
    query.prepare(sql);
    if (!idFiltre.isEmpty()) {
        query.bindValue(":id", idFiltre);
    }
    if (!nomFiltre.isEmpty()) {
        query.bindValue(":nom", "%" + nomFiltre.toUpper() + "%");
    }

    if (!query.exec()) {
        afficherErreurSql("Chargement fournisseurs", detailsErreurSql(query.lastError()));
        return false;
    }

    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }

    return true;
}

QString MainWindow::genererIdFournisseur()
{
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return QString();
    }

    QSqlQuery query;
    query.prepare(
        QString("SELECT IDFOURNISSEUR FROM (SELECT IDFOURNISSEUR FROM %1 ORDER BY IDFOURNISSEUR DESC) WHERE ROWNUM = 1")
            .arg(m_tableFournisseurs)
        );

    if (query.exec() && query.next()) {
        const QString lastId = query.value(0).toString();
        const QRegularExpression re("(\\d+)$");
        const QRegularExpressionMatch match = re.match(lastId);
        if (match.hasMatch()) {
            bool ok = false;
            const int numero = match.captured(1).toInt(&ok);
            if (ok) {
                return QString("F%1").arg(numero + 1, 4, 10, QChar('0'));
            }
        }
    }

    // Fallback if REGEXP/ODBC conversion is unsupported.
    return "F" + QDateTime::currentDateTime().toString("yyMMddhhmmss");
}

bool MainWindow::validerTelephone(const QString &telephone) const
{
    if (telephone.isEmpty()) {
        return true;
    }

    bool ok = false;
    telephone.toLongLong(&ok);
    return ok;
}

void MainWindow::afficherErreurSql(const QString &operation, const QString &detail)
{
    QMessageBox::critical(this, operation, QString("%1\n\n%2").arg(operation, detail));
}
