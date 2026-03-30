#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ajoutfournisseur.h"
#include "modifierfournisseur.h"

#include <QDate>
#include <QDateTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPainter>
#include <QPdfWriter>
#include <QRegularExpression>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

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
    ui->label_10->setText("Type matiere :");
    ui->comboTypeRecherche->clear();
    ui->comboTypeRecherche->addItems({"Tous", "Bois", "Métal", "Plastique", "Verre"});

    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList logoPaths = {
        appDir + "/logo.png",
        appDir + "/assets/logo.png",
        appDir + "/images/logo.png"
    };

    QPixmap logo;
    for (const QString &path : logoPaths) {
        if (logo.load(path)) {
            break;
        }
    }
    if (!logo.isNull()) {
        ui->labelLogo->setPixmap(logo.scaled(ui->labelLogo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

        if (idFournisseur.isEmpty() || idFournisseur.size() < 3 || nom.isEmpty() || nom.size() < 3 || type.isEmpty() || adresse.isEmpty() || adresse.size() < 5) {
            QMessageBox::warning(this, "Validation", "ID (min 3), NOM (min 3), TYPE et ADRESSE (min 5) sont obligatoires.");
            return;
        }

        if (!validerTelephone(telephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE est obligatoire et doit contenir entre 8 et 15 chiffres.");
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

        if (newNom.isEmpty() || newNom.size() < 3 || newType.isEmpty() || newAdresse.isEmpty() || newAdresse.size() < 5) {
            QMessageBox::warning(this, "Validation", "NOM (min 3), TYPE et ADRESSE (min 5) sont obligatoires.");
            return;
        }

        if (!validerTelephone(newTelephone)) {
            QMessageBox::warning(this, "Validation", "TELEPHONE est obligatoire et doit contenir entre 8 et 15 chiffres.");
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
               "WHEN UPPER(QUALITE) LIKE 'A%' THEN 1 "
               "WHEN UPPER(QUALITE) LIKE 'B%' THEN 2 "
               "WHEN UPPER(QUALITE) LIKE 'C%' THEN 3 "
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
                "WHEN UPPER(QUALITE) LIKE 'A%%' THEN 'A' "
                "WHEN UPPER(QUALITE) LIKE 'B%%' THEN 'B' "
                "WHEN UPPER(QUALITE) LIKE 'C%%' THEN 'C' "
                "ELSE 'AUTRE' END AS QUALITE_GROUPE, "
                "COUNT(*) AS TOTAL "
                "FROM %1 "
                "GROUP BY CASE "
                "WHEN UPPER(QUALITE) LIKE 'A%%' THEN 'A' "
                "WHEN UPPER(QUALITE) LIKE 'B%%' THEN 'B' "
                "WHEN UPPER(QUALITE) LIKE 'C%%' THEN 'C' "
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
    dominante = "A (bonne)";
    if (countB > maxCount) {
        maxCount = countB;
        dominante = "B (moyenne)";
    }
    if (countC > maxCount) {
        maxCount = countC;
        dominante = "C (mediocre)";
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

    addSlice("A (bonne)", countA, QColor("#6ba539"));
    addSlice("B (moyenne)", countB, QColor("#e5a93d"));
    addSlice("C (mediocre)", countC, QColor("#d45a3a"));
    addSlice("Autre", countAutre, QColor("#8f8f8f"));

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Repartition des fournisseurs par qualite de matiere");
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
    categories << "A" << "B" << "C" << "Autre";
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
        QString("Total fournisseurs: %1 | A: %2  B: %3  C: %4  Autre: %5")
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

