#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ajoutfournisseur.h"
#include "modifierfournisseur.h"

#include <QDate>
#include <QDateTime>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QPainter>
#include <QPdfWriter>
#include <QRegularExpression>
#include <QTableWidget>
#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>

#include <algorithm>
#include <cmath>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

namespace {
QString jsEscape(const QString &value)
{
    QString escaped = value;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    escaped.replace("\n", " ");
    escaped.replace("\r", " ");
    return escaped;
}

bool ouvrirCarteHtmlGoogle(const QString &apiKey, const QString &nom, const QString &recherche)
{
    if (apiKey.trimmed().isEmpty()) {
        return false;
    }

    const QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (tempDir.isEmpty()) {
        return false;
    }

    const QString filePath = tempDir + "/fournisseur_google_map.html";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }

    const QString nomJs = jsEscape(nom);
    const QString rechercheJs = jsEscape(recherche);
    const QString keyJs = jsEscape(apiKey.trimmed());

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "<!doctype html>\n"
           "<html>\n"
           "  <head>\n"
           "    <meta charset=\"utf-8\" />\n"
           "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n"
           "    <title>Fournisseur Map</title>\n"
           "    <style>html, body, #map { height: 100%; margin: 0; padding: 0; }</style>\n"
           "    <script>\n"
           "      function initMap() {\n"
           "        const geocoder = new google.maps.Geocoder();\n"
           "        const query = \"" << rechercheJs << "\";\n"
           "        const supplierName = \"" << nomJs << "\";\n"
           "        geocoder.geocode({ address: query }, function(results, status) {\n"
           "          if (status !== 'OK' || !results || !results.length) {\n"
           "            alert('Impossible de localiser cette adresse: ' + query);\n"
           "            return;\n"
           "          }\n"
           "          const map = new google.maps.Map(document.getElementById('map'), {\n"
           "            zoom: 18,\n"
           "            center: results[0].geometry.location,\n"
           "            mapTypeControl: false\n"
           "          });\n"
           "          const marker = new google.maps.Marker({\n"
           "            position: results[0].geometry.location,\n"
           "            map: map,\n"
           "            title: supplierName\n"
           "          });\n"
           "          const info = new google.maps.InfoWindow({\n"
           "            content: '<div style=\"font-family:Segoe UI;font-size:14px\"><b>' + supplierName + '</b><br>' + query + '</div>'\n"
           "          });\n"
           "          info.open({ map, anchor: marker });\n"
           "        });\n"
           "      }\n"
           "    </script>\n"
           "  </head>\n"
           "  <body>\n"
           "    <div id=\"map\"></div>\n"
           "    <script src=\"https://maps.googleapis.com/maps/api/js?key=" << keyJs << "&callback=initMap\" defer></script>\n"
           "  </body>\n"
           "</html>\n";

    file.close();
    return QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

QString chargerGoogleMapsApiKey()
{
    const QString envKey = qEnvironmentVariable("GOOGLE_MAPS_API_KEY").trimmed();
    if (!envKey.isEmpty()) {
        return envKey;
    }

    const QStringList candidates = {
        QCoreApplication::applicationDirPath() + "/google_maps_api_key.txt",
        QCoreApplication::applicationDirPath() + "/../google_maps_api_key.txt",
        QDir::currentPath() + "/google_maps_api_key.txt"
    };

    for (const QString &path : candidates) {
        QFile f(path);
        if (!f.exists()) {
            continue;
        }
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }
        QTextStream in(&f);
        in.setEncoding(QStringConverter::Utf8);
        const QString fileKey = in.readAll().trimmed();
        f.close();
        if (!fileKey.isEmpty()) {
            return fileKey;
        }
    }

    return QString();
}

QString nettoyerPourMaps(const QString &texte)
{
    return texte.simplified().trimmed();
}

QString construireRechercheMaps(const QString &nom, const QString &adresse)
{
    QStringList parties;

    if (!nom.trimmed().isEmpty()) {
        parties << nettoyerPourMaps(nom);
    }

    const QStringList segments = adresse.split('-', Qt::SkipEmptyParts);
    for (const QString &segment : segments) {
        const QString propre = nettoyerPourMaps(segment);
        if (!propre.isEmpty()) {
            parties << propre;
        }
    }

    if (parties.isEmpty()) {
        return QString();
    }

    return parties.join(", ");
}

QString detailsErreurSql(const QSqlError &err)
{
    return QString("Driver: %1\nBase: %2\nCode natif: %3\nTexte: %4")
    .arg(err.driverText(), err.databaseText(), err.nativeErrorCode(), err.text());
}

bool nomFournisseurValide(const QString &nom)
{
    return QRegularExpression("^[A-ZÀ-Ý][A-Za-zÀ-ÿ\\s'-]{0,19}$").match(nom).hasMatch();
}

bool adresseComplete(const QString &adresse)
{
    const QStringList parts = adresse.split("-", Qt::SkipEmptyParts);
    if (parts.size() < 3) {
        return false;
    }
    const bool baseOk = !parts.at(0).trimmed().isEmpty()
                        && !parts.at(1).trimmed().isEmpty()
                        && !parts.at(2).trimmed().isEmpty();
    if (!baseOk) {
        return false;
    }
    if (parts.size() >= 5) {
        return !parts.at(3).trimmed().isEmpty() && !parts.at(4).trimmed().isEmpty();
    }
    return true;
}

bool idFournisseurValide(const QString &id)
{
    return QRegularExpression("^[A-Z][A-Z0-9]{2,19}$").match(id).hasMatch();
}

bool tableHasColumn(const QString &qualifiedTable, const QString &columnName)
{
    QString owner;
    QString table = qualifiedTable;
    if (qualifiedTable.contains('.')) {
        const QStringList parts = qualifiedTable.split('.');
        if (parts.size() == 2) {
            owner = parts.at(0).trimmed().toUpper();
            table = parts.at(1).trimmed();
        }
    }

    table = table.trimmed().toUpper();
    const QString col = columnName.trimmed().toUpper();

    QSqlQuery query;
    if (!owner.isEmpty()) {
        query.prepare(
            "SELECT COUNT(*) FROM ALL_TAB_COLUMNS "
            "WHERE OWNER = :owner AND TABLE_NAME = :table AND COLUMN_NAME = :col"
            );
        query.bindValue(":owner", owner);
    } else {
        query.prepare(
            "SELECT COUNT(*) FROM USER_TAB_COLUMNS "
            "WHERE TABLE_NAME = :table AND COLUMN_NAME = :col"
            );
    }

    query.bindValue(":table", table);
    query.bindValue(":col", col);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toInt() > 0;
}

int scoreQualite(const QString &qualite)
{
    const QString q = qualite.trimmed().toUpper();
    if (q.startsWith("1")) {
        return 100;
    }
    if (q.startsWith("2")) {
        return 75;
    }
    if (q.startsWith("3")) {
        return 55;
    }
    return 45;
}

double coutUnitaireEstime(const QString &type, const QString &qualite)
{
    const QString t = type.trimmed().toUpper();
    double base = 50.0;
    if (t.contains("BOIS")) {
        base = 35.0;
    } else if (t.contains("METAL") || t.contains("M\u00c9TAL")) {
        base = 60.0;
    } else if (t.contains("BOVIN")) {
        base = 82.0;
    } else if (t.contains("OVIN")) {
        base = 72.0;
    } else if (t.contains("SYNTH")) {
        base = 52.0;
    }

    const QString q = qualite.trimmed().toUpper();
    double facteurQualite = 1.0;
    if (q.startsWith("1")) {
        facteurQualite = 1.20;
    } else if (q.startsWith("2")) {
        facteurQualite = 1.00;
    } else if (q.startsWith("3")) {
        facteurQualite = 0.85;
    }

    return base * facteurQualite;
}

int scoreBudget(double coutTotalEstime, double budgetMax)
{
    if (budgetMax <= 0.0) {
        return 0;
    }
    if (coutTotalEstime <= budgetMax) {
        return 100;
    }
    const double depassementRatio = (coutTotalEstime - budgetMax) / budgetMax;
    const int penalty = static_cast<int>(std::round(depassementRatio * 120.0));
    return std::max(0, 100 - penalty);
}

int scoreDelai(const QDate &dateLivraison, const QDate &dateLimite)
{
    if (!dateLivraison.isValid()) {
        return 40;
    }
    if (dateLivraison <= dateLimite) {
        return 100;
    }
    const int retardJours = dateLimite.daysTo(dateLivraison);
    return std::max(0, 100 - retardJours * 4);
}

struct RecommendationItem {
    QString id;
    QString nom;
    QString qualite;
    QDate delai;
    double coutUnitaire = 0.0;
    double coutTotal = 0.0;
    int score = 0;
};
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
    ui->label_10->setText("Type matiere :");
    ui->comboTypeRecherche->clear();
    ui->comboTypeRecherche->addItems({"Tous", "Bois", "Métal", "Cuir bovin", "Cuir ovin", "Cuir synthetique"});

    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList logoPaths = {
        appDir + "/logo.png",
        appDir + "/assets/logo.png",
        appDir + "/images/logo.png"
    };

    QPixmap logo;
    for (const QString &path : logoPaths) {
        if (logo.load(path)) { break; }
    }
    if (!logo.isNull()) {
        ui->labelLogo->setPixmap(logo.scaled(ui->labelLogo->size(), 
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

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

    if (dialog.exec() == QDialog::Accepted) {
        const QString idFournisseur = dialog.getIdFournisseur().trimmed();
        const QString nom = dialog.getNomFournisseur().trimmed();
        const QString type = dialog.getTypeMatiere().trimmed();
        const QString telephone = dialog.getTelephone().trimmed();
        const QString adresse = dialog.getAdresse().trimmed();
        const QString delai = dialog.getDelaiLivraison().toString("dd/MM/yyyy");
        const QString qualite = dialog.getQualiteMatiere().trimmed();
        const QString statut = dialog.getStatut().trimmed();
        const double prixUnitaire = dialog.getPrixUnitaireEstime();
        const int capaciteMax = dialog.getCapaciteMax();
        const double tauxFiabilite = dialog.getTauxFiabilite();

        if (!idFournisseurValide(idFournisseur) || type.isEmpty()) {
            QMessageBox::warning(this, "Validation", "ID invalide (majuscule initiale, 3 a 20 caracteres A-Z/0-9) et TYPE obligatoire.");
            return;
        }

        if (!nomFournisseurValide(nom)) {
            QMessageBox::warning(this, "Validation", "Le NOM doit commencer par une majuscule et contenir au maximum 20 caracteres.");
            return;
        }

        if (!adresseComplete(adresse)) {
            QMessageBox::warning(this, "Validation", "L'ADRESSE doit etre complete: Pays - Region - Ville.");
            return;
        }

        if (!validerTelephone(telephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE est obligatoire et doit contenir entre 8 et 15 chiffres.");
            return;
        }

        QSqlQuery query;
        if (m_hasAdvancedRecommendationFields) {
            query.prepare(
                QString("INSERT INTO %1 "
                        "(IDFOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT, "
                        "PRIX_UNITAIRE_ESTIME, CAPACITE_MAX, TAUX_FIABILITE) "
                        "VALUES (:id, :nom, :type, TO_NUMBER(NULLIF(:telephone, '')), NULLIF(:adresse, ''), "
                        "TO_DATE(:delai, 'DD/MM/YYYY'), NULLIF(:qualite, ''), NULLIF(:statut, ''), :prix, :capacite, :fiabilite)")
                    .arg(m_tableFournisseurs)
                );
        } else {
            query.prepare(
                QString("INSERT INTO %1 "
                        "(IDFOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT) "
                        "VALUES (:id, :nom, :type, TO_NUMBER(NULLIF(:telephone, '')), NULLIF(:adresse, ''), "
                        "TO_DATE(:delai, 'DD/MM/YYYY'), NULLIF(:qualite, ''), NULLIF(:statut, ''))")
                    .arg(m_tableFournisseurs)
                );
        }

        query.bindValue(":id", idFournisseur);
        query.bindValue(":nom", nom);
        query.bindValue(":type", type);
        query.bindValue(":telephone", telephone);
        query.bindValue(":adresse", adresse);
        query.bindValue(":delai", delai);
        query.bindValue(":qualite", qualite);
        query.bindValue(":statut", statut);
        if (m_hasAdvancedRecommendationFields) {
            query.bindValue(":prix", prixUnitaire);
            query.bindValue(":capacite", capaciteMax);
            query.bindValue(":fiabilite", tauxFiabilite);
        }

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
    double prixUnitaire = 0.0;
    int capaciteMax = 0;
    double tauxFiabilite = 50.0;

    if (m_hasAdvancedRecommendationFields) {
        QSqlQuery detailsQuery;
        detailsQuery.prepare(
            QString("SELECT NVL(PRIX_UNITAIRE_ESTIME, 0), NVL(CAPACITE_MAX, 0), NVL(TAUX_FIABILITE, 50) "
                    "FROM %1 WHERE IDFOURNISSEUR = :id")
                .arg(m_tableFournisseurs)
            );
        detailsQuery.bindValue(":id", idFournisseur);
        if (detailsQuery.exec() && detailsQuery.next()) {
            prixUnitaire = detailsQuery.value(0).toDouble();
            capaciteMax = detailsQuery.value(1).toInt();
            tauxFiabilite = detailsQuery.value(2).toDouble();
        }
    }

    QDate delai = QDate::fromString(delaiStr, "dd/MM/yyyy");
    if (!delai.isValid()) {
        delai = QDate::currentDate().addDays(30);
    }

    ModifierFournisseur dialog(this);
    dialog.setInitialData(nom, type, telephone, adresse, delai, qualite, statut,
                          prixUnitaire, capaciteMax, tauxFiabilite);

    if (dialog.exec() == QDialog::Accepted) {
        const QString newNom = dialog.getNomFournisseur().trimmed();
        const QString newType = dialog.getTypeMatiere().trimmed();
        const QString newTelephone = dialog.getTelephone().trimmed();
        const QString newAdresse = dialog.getAdresse().trimmed();
        const QString newDelai = dialog.getDelaiLivraison().toString("dd/MM/yyyy");
        const QString newQualite = dialog.getQualiteMatiere().trimmed();
        const QString newStatut = dialog.getStatut().trimmed();
        const double newPrixUnitaire = dialog.getPrixUnitaireEstime();
        const int newCapaciteMax = dialog.getCapaciteMax();
        const double newTauxFiabilite = dialog.getTauxFiabilite();

        if (newType.isEmpty()) {
            QMessageBox::warning(this, "Validation", "Le TYPE est obligatoire.");
            return;
        }

        if (!nomFournisseurValide(newNom)) {
            QMessageBox::warning(this, "Validation", "Le NOM doit commencer par une majuscule et contenir au maximum 20 caracteres.");
            return;
        }

        if (!adresseComplete(newAdresse)) {
            QMessageBox::warning(this, "Validation", "L'ADRESSE doit etre complete: Pays - Region - Ville.");
            return;
        }

        if (!validerTelephone(newTelephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE est obligatoire et doit contenir entre 8 et 15 chiffres.");
            return;
        }

        QSqlQuery query;
        if (m_hasAdvancedRecommendationFields) {
            query.prepare(
                QString("UPDATE %1 SET "
                        "NOM = :nom, "
                        "TYPE_MATIERE = :type, "
                        "TELEPHONE = TO_NUMBER(NULLIF(:telephone, '')), "
                        "ADRESSE = NULLIF(:adresse, ''), "
                        "DELAI_LIVRAISON = TO_DATE(:delai, 'DD/MM/YYYY'), "
                        "QUALITE = NULLIF(:qualite, ''), "
                        "STATUT = NULLIF(:statut, ''), "
                        "PRIX_UNITAIRE_ESTIME = :prix, "
                        "CAPACITE_MAX = :capacite, "
                        "TAUX_FIABILITE = :fiabilite "
                        "WHERE IDFOURNISSEUR = :id")
                    .arg(m_tableFournisseurs)
                );
        } else {
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
        }

        query.bindValue(":nom", newNom);
        query.bindValue(":type", newType);
        query.bindValue(":telephone", newTelephone);
        query.bindValue(":adresse", newAdresse);
        query.bindValue(":delai", newDelai);
        query.bindValue(":qualite", newQualite);
        query.bindValue(":statut", newStatut);
        if (m_hasAdvancedRecommendationFields) {
            query.bindValue(":prix", newPrixUnitaire);
            query.bindValue(":capacite", newCapaciteMax);
            query.bindValue(":fiabilite", newTauxFiabilite);
        }
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
    QString typeFiltre = ui->comboTypeRecherche->currentText().trimmed();
    if (typeFiltre == "Tous") {
        typeFiltre.clear();
    }
    chargerFournisseurs(idFiltre, typeFiltre);
}

void MainWindow::on_pushButton_8_clicked()
{
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "Exporter PDF", "Le tableau fournisseurs est vide.");
        return;
    }

    QString chemin = QFileDialog::getSaveFileName(
        this,
        "Exporter le tableau fournisseurs en PDF",
        "fournisseurs.pdf",
        "PDF (*.pdf)");

    if (chemin.isEmpty()) {
        return;
    }
    if (!chemin.endsWith(".pdf", Qt::CaseInsensitive)) {
        chemin += ".pdf";
    }

    QPdfWriter writer(chemin);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setResolution(120);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "Exporter PDF", "Impossible de creer le fichier PDF.");
        return;
    }

    const int margin = 40;
    const int pageWidth = writer.width();
    const int pageHeight = writer.height();
    const int cols = ui->tableWidget->columnCount();
    const int colWidth = (pageWidth - 2 * margin) / qMax(1, cols);
    const int headerHeight = 34;
    const int rowHeight = 28;

    int y = margin;

    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(margin, y, "Liste des fournisseurs");

    QFont infoFont = painter.font();
    infoFont.setPointSize(9);
    infoFont.setBold(false);
    painter.setFont(infoFont);
    y += 24;
    painter.drawText(margin, y, "Date export: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    y += 20;

    auto drawHeader = [&](int yHeader) {
        QFont headerFont = painter.font();
        headerFont.setBold(true);
        painter.setFont(headerFont);
        for (int c = 0; c < cols; ++c) {
            const int x = margin + c * colWidth;
            painter.drawRect(x, yHeader, colWidth, headerHeight);
            const QString titre = ui->tableWidget->horizontalHeaderItem(c)
                                      ? ui->tableWidget->horizontalHeaderItem(c)->text()
                                      : QString("Col %1").arg(c + 1);
            painter.drawText(x + 6, yHeader + 22, titre);
        }
        QFont bodyFont = painter.font();
        bodyFont.setBold(false);
        painter.setFont(bodyFont);
    };

    drawHeader(y);
    y += headerHeight;

    for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        if (y + rowHeight > pageHeight - margin) {
            writer.newPage();
            y = margin;
            drawHeader(y);
            y += headerHeight;
        }

        for (int c = 0; c < cols; ++c) {
            const int x = margin + c * colWidth;
            painter.drawRect(x, y, colWidth, rowHeight);
            const QString texte = ui->tableWidget->item(r, c) ? ui->tableWidget->item(r, c)->text() : "";
            painter.drawText(x + 6, y + 19, texte.left(32));
        }
        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Exporter PDF", "Export termine: " + chemin);
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
            m_hasAdvancedRecommendationFields =
                tableHasColumn(m_tableFournisseurs, "PRIX_UNITAIRE_ESTIME") &&
                tableHasColumn(m_tableFournisseurs, "CAPACITE_MAX") &&
                tableHasColumn(m_tableFournisseurs, "TAUX_FIABILITE");
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
            m_hasAdvancedRecommendationFields =
                tableHasColumn(m_tableFournisseurs, "PRIX_UNITAIRE_ESTIME") &&
                tableHasColumn(m_tableFournisseurs, "CAPACITE_MAX") &&
                tableHasColumn(m_tableFournisseurs, "TAUX_FIABILITE");
            return true;
        }
    }

    afficherErreurSql(
        "Structure fournisseurs",
        "Aucune table TABLE_FOURNISSEURS/FOURNISSEUR/FOURNISSEURS trouvee dans le schema courant."
        );
    return false;
}

bool MainWindow::chargerFournisseurs(const QString &idFiltre, const QString &typeFiltre)
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
    if (!typeFiltre.isEmpty()) {
        sql += " AND UPPER(TYPE_MATIERE) LIKE :type";
    }

    const int triIndex = ui->comboBox->currentIndex();
    if (triIndex == 1) {
        sql += " ORDER BY DELAI_LIVRAISON ASC NULLS LAST, IDFOURNISSEUR";
    } else if (triIndex == 2) {
        sql += " ORDER BY DELAI_LIVRAISON DESC NULLS LAST, IDFOURNISSEUR";
    } else if (triIndex == 3) {
        sql += " ORDER BY CASE "
               "WHEN UPPER(QUALITE) LIKE '1%' THEN 1 "
               "WHEN UPPER(QUALITE) LIKE '2%' THEN 2 "
               "WHEN UPPER(QUALITE) LIKE '3%' THEN 3 "
               "ELSE 4 END, IDFOURNISSEUR";
    } else {
        sql += " ORDER BY IDFOURNISSEUR";
    }

    QSqlQuery query;
    query.prepare(sql);
    if (!idFiltre.isEmpty()) {
        query.bindValue(":id", idFiltre);
    }
    if (!typeFiltre.isEmpty()) {
        query.bindValue(":type", "%" + typeFiltre.toUpper() + "%");
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
    return QRegularExpression("^\\d{8,15}$").match(telephone).hasMatch();
}

void MainWindow::afficherErreurSql(const QString &operation, const QString &detail)
{
    QMessageBox::critical(this, operation, QString("%1\n\n%2").arg(operation, detail));
}

void MainWindow::on_comboBox_currentIndexChanged(int)
{
    const QString idFiltre = ui->lineEdit->text().trimmed();
    QString typeFiltre = ui->comboTypeRecherche->currentText().trimmed();
    if (typeFiltre == "Tous") {
        typeFiltre.clear();
    }
    chargerFournisseurs(idFiltre, typeFiltre);
}

void MainWindow::on_pushButton_9_clicked()
{
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return;
    }

    QSqlQuery query;
    query.prepare(
        QString("SELECT CASE "
                "WHEN UPPER(QUALITE) LIKE '1%%' THEN 'A' "
                "WHEN UPPER(QUALITE) LIKE '2%%' THEN 'B' "
                "WHEN UPPER(QUALITE) LIKE '3%%' THEN 'C' "
                "ELSE 'AUTRE' END AS QUALITE_GROUPE, "
                "COUNT(*) AS TOTAL "
                "FROM %1 "
                "GROUP BY CASE "
                "WHEN UPPER(QUALITE) LIKE '1%%' THEN 'A' "
                "WHEN UPPER(QUALITE) LIKE '2%%' THEN 'B' "
                "WHEN UPPER(QUALITE) LIKE '3%%' THEN 'C' "
                "ELSE 'AUTRE' END")
            .arg(m_tableFournisseurs)
        );

    if (!query.exec()) {
        afficherErreurSql("Statistiques fournisseurs", detailsErreurSql(query.lastError()));
        return;
    }

    int countA = 0;
    int countB = 0;
    int countC = 0;
    int countAutre = 0;

    while (query.next()) {
        const QString groupe = query.value(0).toString();
        const int total = query.value(1).toInt();
        if (groupe == "A") {
            countA = total;
        } else if (groupe == "B") {
            countB = total;
        } else if (groupe == "C") {
            countC = total;
        } else {
            countAutre = total;
        }
    }

    if (countA + countB + countC + countAutre == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune donnee fournisseur pour afficher les statistiques.");
        return;
    }

    const int total = countA + countB + countC + countAutre;
    auto pourcentage = [total](int v) -> double {
        return total > 0 ? (100.0 * static_cast<double>(v) / static_cast<double>(total)) : 0.0;
    };

    QString dominante = "Aucune";
    int maxCount = countA;
    dominante = "1er choix";
    if (countB > maxCount) {
        maxCount = countB;
        dominante = "2eme choix";
    }
    if (countC > maxCount) {
        maxCount = countC;
        dominante = "3eme choix";
    }
    if (countAutre > maxCount) {
        dominante = "Autre";
    }

    auto *series = new QPieSeries();
    series->setHoleSize(0.42);

    auto addSlice = [&](const QString &labelBase, int value, const QColor &color) {
        if (value <= 0) {
            return;
        }
        const QString label = QString("%1  %2 (%3%)")
                                  .arg(labelBase)
                                  .arg(value)
                                  .arg(QString::number(pourcentage(value), 'f', 1));
        QPieSlice *slice = series->append(label, value);
        slice->setLabelVisible(true);
        slice->setBrush(color);
        slice->setBorderColor(Qt::white);
        slice->setBorderWidth(2);
        if (value == maxCount && maxCount > 0) {
            slice->setExploded(true);
            slice->setExplodeDistanceFactor(0.06);
        }
    };

    addSlice("1er choix", countA, QColor("#6ba539"));
    addSlice("2eme choix", countB, QColor("#e5a93d"));
    addSlice("3eme choix", countC, QColor("#d45a3a"));
    addSlice("Autre", countAutre, QColor("#8f8f8f"));

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Repartition des fournisseurs par qualite");
    chart->setTitleBrush(QBrush(QColor("#4a2717")));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor("#fffaf5")));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent;");

    auto *barSet = new QBarSet("Nombre de fournisseurs");
    *barSet << countA << countB << countC << countAutre;
    barSet->setColor(QColor("#8b5a3a"));

    auto *barSeries = new QBarSeries();
    barSeries->append(barSet);

    auto *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Comparaison par qualite (Histogramme)");
    barChart->setTitleBrush(QBrush(QColor("#4a2717")));
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setBackgroundBrush(QBrush(QColor("#fffaf5")));

    QStringList categories;
    categories << "1er choix" << "2eme choix" << "3eme choix" << "Autre";
    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    int maxBar = countA;
    if (countB > maxBar) {
        maxBar = countB;
    }
    if (countC > maxBar) {
        maxBar = countC;
    }
    if (countAutre > maxBar) {
        maxBar = countAutre;
    }
    auto *axisY = new QValueAxis();
    axisY->setRange(0, maxBar + 1);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Nombre");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);
    barChart->legend()->setVisible(false);

    auto *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setStyleSheet("background: transparent;");

    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques - Qualite des matieres");
    dialog->resize(860, 620);
    dialog->setStyleSheet(
        "QDialog { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #fff9f2, stop:1 #f3e7d9); }"
        "QLabel#title { color: #4a2717; font-size: 20px; font-weight: 800; }"
        "QLabel#subtitle { color: #6b3e26; font-size: 12px; font-weight: 600; }"
        "QLabel#badge { background-color: #fff2e1; border: 2px solid #d9b18d; border-radius: 10px; padding: 10px 14px;"
        "               color: #4a2717; font-size: 12px; font-weight: 700; }"
    );

    auto *layout = new QVBoxLayout(dialog);
    auto *title = new QLabel("Qualite des matieres: vue analytique", dialog);
    title->setObjectName("title");
    auto *subtitle = new QLabel(
        QString("Total fournisseurs: %1 | 1er choix: %2  2eme choix: %3  3eme choix: %4  Autre: %5")
            .arg(total)
            .arg(countA)
            .arg(countB)
            .arg(countC)
            .arg(countAutre),
        dialog);
    subtitle->setObjectName("subtitle");
    auto *badge = new QLabel(
        QString("Qualite dominante: %1 (%2 fournisseurs)").arg(dominante).arg(maxCount),
        dialog);
    badge->setObjectName("badge");

    auto *tabs = new QTabWidget(dialog);
    tabs->addTab(chartView, "Camembert");
    tabs->addTab(barChartView, "Histogramme");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(badge);
    layout->addWidget(tabs, 1);
    dialog->setLayout(layout);
    dialog->exec();
}

void MainWindow::on_pushButton_maps_clicked()
{
    const int row = ui->tableWidget->currentRow();

    QString nomSelectionne;
    QString adresseSelectionnee;

    if (row >= 0) {
        if (ui->tableWidget->item(row, 1)) {
            nomSelectionne = ui->tableWidget->item(row, 1)->text().trimmed();
        }
        if (ui->tableWidget->item(row, 4)) {
            adresseSelectionnee = ui->tableWidget->item(row, 4)->text().trimmed();
        }
    }

    if (nomSelectionne.isEmpty()) {
        QStringList candidates;
        if (!m_tableFournisseurs.isEmpty()) {
            candidates << m_tableFournisseurs;
        }
        candidates << "TABLE_FOURNISSEURS" << "FOURNISSEUR" << "FOURNISSEURS";
        candidates.removeDuplicates();

        for (const QString &tableName : candidates) {
            QSqlQuery query;
            query.prepare(
                QString("SELECT NOM, ADRESSE "
                        "FROM %1 "
                        "WHERE ADRESSE IS NOT NULL AND ROWNUM = 1")
                    .arg(tableName)
                );

            if (!query.exec() || !query.next()) {
                continue;
            }

            nomSelectionne = query.value(0).toString().trimmed();
            adresseSelectionnee = query.value(1).toString().trimmed();
            if (!nomSelectionne.isEmpty()) {
                m_tableFournisseurs = tableName;
                break;
            }
        }
    }

    if (nomSelectionne.isEmpty()) {
        QMessageBox::information(this, "Maps", "Selectionne un fournisseur pour l'afficher dans Google Maps.");
        return;
    }

    const QString recherche = construireRechercheMaps(nomSelectionne, adresseSelectionnee);
    if (recherche.isEmpty()) {
        QMessageBox::information(this, "Maps", "Selectionne un fournisseur pour l'afficher dans Google Maps.");
        return;
    }

    const QString apiKey = chargerGoogleMapsApiKey();
    if (ouvrirCarteHtmlGoogle(apiKey, nomSelectionne, recherche)) {
        QMessageBox::information(this, "Maps", QString("Ouverture de la carte HTML Google Maps pour: %1").arg(nomSelectionne));
        return;
    }

    QUrl mapsUrl("https://www.google.com/maps/search/");
    QUrlQuery query;
    query.addQueryItem("api", "1");
    query.addQueryItem("query", recherche);
    query.addQueryItem("zoom", "18");
    mapsUrl.setQuery(query);

    if (!QDesktopServices::openUrl(mapsUrl)) {
        QMessageBox::warning(this, "Maps", "Impossible d'ouvrir Google Maps dans le navigateur.");
        return;
    }

    QMessageBox::information(
        this,
        "Maps",
        QString("Ouverture de Google Maps (mode navigateur) pour: %1\n"
                "Astuce: ajoute GOOGLE_MAPS_API_KEY ou un fichier google_maps_api_key.txt pour activer la carte HTML avec épingle nommée.")
            .arg(nomSelectionne)
        );
}

void MainWindow::on_pushButton_recommandation_clicked()
{
    if (m_tableFournisseurs.isEmpty() && !resoudreStructureFournisseurs()) {
        return;
    }

    QDialog besoinsDialog(this);
    besoinsDialog.setWindowTitle("Recommandation intelligente");
    besoinsDialog.resize(420, 250);

    auto *form = new QFormLayout(&besoinsDialog);
    auto *typeInput = new QComboBox(&besoinsDialog);
    typeInput->addItems({"Bois", "Métal", "Cuir bovin", "Cuir ovin", "Cuir synthetique"});

    auto *quantiteInput = new QSpinBox(&besoinsDialog);
    quantiteInput->setRange(1, 100000);
    quantiteInput->setValue(500);

    auto *budgetInput = new QDoubleSpinBox(&besoinsDialog);
    budgetInput->setRange(1.0, 100000000.0);
    budgetInput->setDecimals(2);
    budgetInput->setValue(50000.0);
    budgetInput->setSuffix(" TND");

    auto *delaiInput = new QDateEdit(QDate::currentDate().addDays(30), &besoinsDialog);
    delaiInput->setCalendarPopup(true);
    delaiInput->setDisplayFormat("dd/MM/yyyy");

    form->addRow("Type matière", typeInput);
    form->addRow("Quantité demandée", quantiteInput);
    form->addRow("Budget max", budgetInput);
    form->addRow("Délai max", delaiInput);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &besoinsDialog);
    form->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &besoinsDialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &besoinsDialog, &QDialog::reject);

    if (besoinsDialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString typeCible = typeInput->currentText().trimmed();
    const int quantite = quantiteInput->value();
    const double budget = budgetInput->value();
    const QDate dateLimite = delaiInput->date();

    QSqlQuery query;
    if (m_hasAdvancedRecommendationFields) {
        query.prepare(
            QString("SELECT IDFOURNISSEUR, NOM, TYPE_MATIERE, QUALITE, DELAI_LIVRAISON, STATUT, "
                    "NVL(PRIX_UNITAIRE_ESTIME, 0), NVL(CAPACITE_MAX, 0), NVL(TAUX_FIABILITE, 50) "
                    "FROM %1 "
                    "WHERE UPPER(TYPE_MATIERE) LIKE :type")
                .arg(m_tableFournisseurs)
            );
    } else {
        query.prepare(
            QString("SELECT IDFOURNISSEUR, NOM, TYPE_MATIERE, QUALITE, DELAI_LIVRAISON, STATUT "
                    "FROM %1 "
                    "WHERE UPPER(TYPE_MATIERE) LIKE :type")
                .arg(m_tableFournisseurs)
            );
    }
    query.bindValue(":type", "%" + typeCible.toUpper() + "%");

    if (!query.exec()) {
        afficherErreurSql("Recommandation fournisseurs", detailsErreurSql(query.lastError()));
        return;
    }

    if (!m_hasAdvancedRecommendationFields) {
        QMessageBox::information(
            this,
            "Recommandation",
            "Les colonnes PRIX_UNITAIRE_ESTIME, CAPACITE_MAX et TAUX_FIABILITE sont absentes.\n"
            "Le classement utilise une estimation provisoire."
            );
    }

    QVector<RecommendationItem> candidats;
    while (query.next()) {
        const QString statut = query.value(5).toString().trimmed().toUpper();
        if (!statut.isEmpty() && !statut.startsWith("ACTIF")) {
            continue;
        }

        RecommendationItem item;
        item.id = query.value(0).toString();
        item.nom = query.value(1).toString();
        item.qualite = query.value(3).toString();
        item.delai = query.value(4).toDate();
        const int qScore = scoreQualite(item.qualite);

        double fiabiliteScore = 60.0;
        int capaciteMax = quantite;
        if (m_hasAdvancedRecommendationFields) {
            item.coutUnitaire = query.value(6).toDouble();
            capaciteMax = query.value(7).toInt();
            fiabiliteScore = query.value(8).toDouble();
            if (capaciteMax > 0 && quantite > capaciteMax) {
                continue;
            }
            if (item.coutUnitaire <= 0.0) {
                item.coutUnitaire = coutUnitaireEstime(typeCible, item.qualite);
            }
        } else {
            item.coutUnitaire = coutUnitaireEstime(typeCible, item.qualite);
        }

        item.coutTotal = item.coutUnitaire * static_cast<double>(quantite);

        const int bScore = scoreBudget(item.coutTotal, budget);
        const int dScore = scoreDelai(item.delai, dateLimite);
        const double fScore = std::max(0.0, std::min(100.0, fiabiliteScore));
        item.score = static_cast<int>(std::round(0.40 * qScore + 0.30 * bScore + 0.20 * dScore + 0.10 * fScore));

        candidats.append(item);
    }

    if (candidats.isEmpty()) {
        QMessageBox::information(this, "Recommandation", "Aucun fournisseur actif ne correspond au type saisi.");
        return;
    }

    std::sort(candidats.begin(), candidats.end(), [](const RecommendationItem &a, const RecommendationItem &b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        return a.coutTotal < b.coutTotal;
    });

    const int topCount = std::min(3, static_cast<int>(candidats.size()));

    QDialog resultatDialog(this);
    resultatDialog.setWindowTitle("Top 3 fournisseurs recommandes");
    resultatDialog.resize(780, 340);

    auto *layout = new QVBoxLayout(&resultatDialog);
    auto *subtitle = new QLabel(
        QString("Besoin: %1 | Quantite: %2 | Budget: %3 TND | Delai: %4")
            .arg(typeCible)
            .arg(quantite)
            .arg(QString::number(budget, 'f', 2))
            .arg(dateLimite.toString("dd/MM/yyyy")),
        &resultatDialog);

    auto *table = new QTableWidget(topCount, 6, &resultatDialog);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Score", "Qualite", "Delai", "Cout estime"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    for (int i = 0; i < topCount; ++i) {
        const RecommendationItem &r = candidats.at(i);
        table->setItem(i, 0, new QTableWidgetItem(r.id));
        table->setItem(i, 1, new QTableWidgetItem(r.nom));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(r.score)));
        table->setItem(i, 3, new QTableWidgetItem(r.qualite));
        table->setItem(i, 4, new QTableWidgetItem(r.delai.isValid() ? r.delai.toString("dd/MM/yyyy") : "N/A"));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(r.coutTotal, 'f', 2) + " TND"));
    }

    layout->addWidget(subtitle);
    layout->addWidget(table);

    auto *closeBtn = new QPushButton("Fermer", &resultatDialog);
    connect(closeBtn, &QPushButton::clicked, &resultatDialog, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);

    resultatDialog.exec();
}

