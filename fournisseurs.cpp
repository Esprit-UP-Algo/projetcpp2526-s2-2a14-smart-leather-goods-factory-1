#include "fournisseurs.h"
#include "ui_fournisseurs.h"
#include "ajoutfournisseur.h"
#include "modifierfournisseur.h"
#include "login.h"
#include "pageemployee.h"
#include "commandes.h"
#include "produitswindow.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QDebug>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>
#include <QTextEdit>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QSslError>
#include <QPainter>
#include <QPixmap>
#include <QPolygon>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopServices>
#include <QUrl>
#include <QtMath>
#include <cmath>
#include <algorithm>
#include <QFrame>

fournisseurs::fournisseurs(int idEmploye, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::fournisseurs)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    
    // Initialize comboTypeRecherche
    ui->comboTypeRecherche->addItems({"Tous", "Bois", "Métal", "Cuir bovin", "Cuir ovin", "Cuir synthetique"});

    setupFournisseursTable();
    loadFournisseurs();
    setupSearch();
}

fournisseurs::~fournisseurs() { delete ui; }

void fournisseurs::setupFournisseursTable()
{
    ui->tableWidget->setColumnCount(12);
    QStringList headers = {"ID", "Réf", "Nom", "Matière", "Téléphone", "Adresse", "Délai", "Qualité", "Statut", "Prix (TND)", "Capacité", "Fiabilité (%)"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void fournisseurs::loadFournisseurs()
{
    ui->tableWidget->setRowCount(0);
    
    QString baseSql = "SELECT ID_FOURNISSEUR, REF, NOM, TYPE_MATIERE, TEL, ADRESSE, TO_CHAR(DELAI_LIVRAISON, 'DD/MM/YYYY'), "
                      "QUALITE, STATUT, PRIX_UNITAIRE, CAPACITE, FIABILITE "
                      "FROM SMARTLEATHER.FOURNISSEUR WHERE 1=1 ";

    // Search filters
    QString searchId = ui->lineEdit->text().trimmed();
    QString searchType = ui->comboTypeRecherche->currentText();
    
    if (!searchId.isEmpty()) {
        baseSql += " AND (ID_FOURNISSEUR LIKE '%" + searchId + "%' OR REF LIKE '%" + searchId + "%')";
    }
    if (searchType != "Tous") {
        baseSql += " AND TYPE_MATIERE = '" + searchType + "'";
    }

    // Sorting
    QString sortSql;
    int sortIdx = ui->comboBox->currentIndex();
    if (sortIdx == 1) sortSql = " ORDER BY DELAI_LIVRAISON ASC";
    else if (sortIdx == 2) sortSql = " ORDER BY DELAI_LIVRAISON DESC";
    else sortSql = " ORDER BY ID_FOURNISSEUR DESC";

    // Compatible Oracle Pagination (ROWNUM pattern for 11g/12c/19c/XE)
    int offset = (m_currentPage - 1) * m_pageSize;
    int maxRow = offset + m_pageSize;
    
    QString sql = QString(
        "SELECT * FROM ("
        "  SELECT a.*, ROWNUM rnum FROM ("
        "    %1 %2"
        "  ) a WHERE ROWNUM <= %3"
        ") WHERE rnum > %4"
    ).arg(baseSql).arg(sortSql).arg(maxRow).arg(offset);

    QSqlQuery query;
    if (!query.exec(sql)) {
        QMessageBox::critical(this, "Erreur de chargement", 
            "Impossible de charger les fournisseurs.\n" + query.lastError().text());
        return;
    }

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 12; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        updateRowColors(row);
    }
    
    ui->pushButton_11->setText(QString::number(m_currentPage));
    ui->pushButton_12->setText(QString::number(m_currentPage + 1));
    ui->pushButton_13->setText(QString::number(m_currentPage + 2));
}

void fournisseurs::updateRowColors(int row)
{
    QTableWidgetItem *statutItem = ui->tableWidget->item(row, 8);
    if (!statutItem) return;
    if (statutItem->text() == "Actif") statutItem->setForeground(QColor(0, 150, 0));
    else if (statutItem->text() == "Inactif") statutItem->setForeground(QColor(200, 0, 0));
}

void fournisseurs::setupSearch()
{
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &fournisseurs::filterTable);
    connect(ui->comboTypeRecherche, SIGNAL(currentIndexChanged(int)), this, SLOT(filterTable()));
}

void fournisseurs::filterTable() { loadFournisseurs(); }

// Ajouter
void fournisseurs::on_pushButton_3_clicked()
{
    AjoutFournisseur dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO SMARTLEATHER.FOURNISSEUR (ID_FOURNISSEUR, REF, NOM, TYPE_MATIERE, TEL, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT, PRIX_UNITAIRE, CAPACITE, FIABILITE) "
                      "VALUES (SMARTLEATHER.SEQ_FOURNISSEUR.NEXTVAL, :ref, :nom, :type, :tel, :addr, :delai, :qualite, :statut, :prix, :cap, :fiab)");
        query.bindValue(":ref", dialog.getIdFournisseur());
        query.bindValue(":nom", dialog.getNomFournisseur());
        query.bindValue(":type", dialog.getTypeMatiere());
        query.bindValue(":tel", dialog.getTelephone());
        query.bindValue(":addr", dialog.getAdresse());
        query.bindValue(":delai", dialog.getDelaiLivraison());
        query.bindValue(":qualite", dialog.getQualiteMatiere());
        query.bindValue(":statut", dialog.getStatut());
        query.bindValue(":prix", dialog.getPrixUnitaireEstime());
        query.bindValue(":cap", dialog.getCapaciteMax());
        query.bindValue(":fiab", dialog.getTauxFiabilite());

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Fournisseur ajouté !");
            loadFournisseurs();
        } else {
            QMessageBox::critical(this, "Erreur", query.lastError().text());
        }
    }
}

// Modifier
void fournisseurs::on_pushButton_4_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez un fournisseur."); return; }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString tel = ui->tableWidget->item(currentRow, 4)->text();
    QString addr = ui->tableWidget->item(currentRow, 5)->text();
    QString delaiStr = ui->tableWidget->item(currentRow, 6)->text();
    QString qual = ui->tableWidget->item(currentRow, 7)->text();
    QString statut = ui->tableWidget->item(currentRow, 8)->text();
    double prix = ui->tableWidget->item(currentRow, 9)->text().toDouble();
    int cap = ui->tableWidget->item(currentRow, 10)->text().toInt();
    double fiab = ui->tableWidget->item(currentRow, 11)->text().toDouble();

    ModifierFournisseur dialog(this);
    dialog.setInitialData(nom, type, tel, addr, QDate::fromString(delaiStr, "dd/MM/yyyy"), qual, statut, prix, cap, fiab);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE SMARTLEATHER.FOURNISSEUR SET NOM=:nom, TYPE_MATIERE=:type, TEL=:tel, "
                      "ADRESSE=:addr, DELAI_LIVRAISON=:delai, QUALITE=:qualite, STATUT=:statut, "
                      "PRIX_UNITAIRE=:prix, CAPACITE=:cap, FIABILITE=:fiab WHERE ID_FOURNISSEUR=:id");
        query.bindValue(":nom", dialog.getNomFournisseur());
        query.bindValue(":type", dialog.getTypeMatiere());
        query.bindValue(":tel", dialog.getTelephone());
        query.bindValue(":addr", dialog.getAdresse());
        query.bindValue(":delai", dialog.getDelaiLivraison());
        query.bindValue(":qualite", dialog.getQualiteMatiere());
        query.bindValue(":statut", dialog.getStatut());
        query.bindValue(":prix", dialog.getPrixUnitaireEstime());
        query.bindValue(":cap", dialog.getCapaciteMax());
        query.bindValue(":fiab", dialog.getTauxFiabilite());
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Mise à jour réussie !");
            loadFournisseurs();
        } else {
            QMessageBox::critical(this, "Erreur", query.lastError().text());
        }
    }
}

// Supprimer
void fournisseurs::on_pushButton_5_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    if (QMessageBox::question(this, "Suppression", "Confirmer la suppression ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTLEATHER.FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
        query.bindValue(":id", id);
        if (query.exec()) loadFournisseurs();
    }
}

// Actualiser
void fournisseurs::on_pushButton_6_clicked() { loadFournisseurs(); }

// Recherche
void fournisseurs::on_pushButton_7_clicked() { loadFournisseurs(); }

// Export PDF (Placeholder for user logic)
void fournisseurs::on_pushButton_8_clicked() { QMessageBox::information(this, "PDF", "Exportation PDF en cours..."); }

// Statistiques
void fournisseurs::on_pushButton_9_clicked() {
    int actif = 0, inactif = 0;
    QSqlQuery query("SELECT STATUT, COUNT(*) FROM SMARTLEATHER.FOURNISSEUR GROUP BY STATUT");
    while(query.next()) {
        if(query.value(0).toString() == "Actif") actif = query.value(1).toInt();
        else inactif = query.value(1).toInt();
    }
    QPieSeries *series = new QPieSeries();
    series->append("Actif", actif);
    series->append("Inactif", inactif);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des fournisseurs");
    QChartView *chartView = new QChartView(chart);
    QDialog d(this); d.resize(400,300); QVBoxLayout *l = new QVBoxLayout(&d); l->addWidget(chartView); d.exec();
}

// Pagination
void fournisseurs::on_pushButton_10_clicked() { if(m_currentPage > 1) { m_currentPage--; loadFournisseurs(); } }
void fournisseurs::on_pushButton_14_clicked() { m_currentPage++; loadFournisseurs(); }
void fournisseurs::on_pushButton_11_clicked() { }
void fournisseurs::on_pushButton_12_clicked() { m_currentPage++; loadFournisseurs(); }
void fournisseurs::on_pushButton_13_clicked() { m_currentPage += 2; loadFournisseurs(); }

void fournisseurs::on_comboBox_currentIndexChanged(int index) { Q_UNUSED(index); loadFournisseurs(); }

// Maps
void fournisseurs::on_pushButton_maps_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un fournisseur.");
        return;
    }

    QString nom     = ui->tableWidget->item(currentRow, 2)->text();
    QString adresse = ui->tableWidget->item(currentRow, 5)->text();

    QString cleanAddr = adresse;
    cleanAddr.replace(" - ", ", ");
    QString encodedAddr = QUrl::toPercentEncoding(cleanAddr);

    // Build dialog first
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📍 Localisation - " + nom);
    dialog->setFixedSize(660, 580);
    dialog->setStyleSheet("QDialog { background-color: #f4efe9; }");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    QLabel *titleLabel = new QLabel("<b>📍 " + nom + "</b> — " + cleanAddr, dialog);
    titleLabel->setWordWrap(true);
    titleLabel->setStyleSheet("font-size: 13px; color: #4a2517; padding: 6px;"
                              " background: rgba(255,255,255,0.7); border-radius: 6px;");
    layout->addWidget(titleLabel);

    QLabel *mapLabel = new QLabel("🔄 Chargement de la carte...", dialog);
    mapLabel->setFixedSize(640, 480);
    mapLabel->setAlignment(Qt::AlignCenter);
    mapLabel->setStyleSheet("background-color: #e8e0d8; border: 2px solid #6b3f2b; border-radius: 8px;");
    layout->addWidget(mapLabel, 0, Qt::AlignCenter);

    QPushButton *btnClose = new QPushButton("Fermer", dialog);
    btnClose->setStyleSheet("QPushButton { background-color: #6b3f2b; color: white; border-radius: 8px;"
                            " padding: 10px 24px; font-weight: bold; } QPushButton:hover { background-color: #8a5a44; }");
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);

    dialog->show();
    QCoreApplication::processEvents();

    QNetworkAccessManager nam;
    nam.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    // ── Step 1: Geocode the address with Nominatim ──────────────────────────
    QUrl geoUrl("https://nominatim.openstreetmap.org/search?q=" + encodedAddr + "&format=json&limit=1");
    QNetworkRequest geoReq(geoUrl);
    geoReq.setHeader(QNetworkRequest::UserAgentHeader, "SmartLeatherApp/1.0");
    QNetworkReply *geoReply = nam.get(geoReq);

    QEventLoop loop;
    connect(geoReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QTimer::singleShot(8000, &loop, &QEventLoop::quit);
    loop.exec();

    if (!geoReply->isFinished() || geoReply->error() != QNetworkReply::NoError) {
        mapLabel->setText("❌ Erreur de connexion.\nVérifiez votre accès internet.");
        geoReply->deleteLater();
        dialog->exec();
        delete dialog;
        return;
    }

    QByteArray geoData = geoReply->readAll();
    geoReply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(geoData);
    QJsonArray arr = doc.array();
    if (arr.isEmpty()) {
        mapLabel->setText("❌ Adresse introuvable sur OpenStreetMap.\n\nAdresse : " + cleanAddr);
        dialog->exec();
        delete dialog;
        return;
    }

    double lat = arr[0].toObject()["lat"].toString().toDouble();
    double lon = arr[0].toObject()["lon"].toString().toDouble();

    // ── Step 2: Calculate OSM tile coordinates ──────────────────────────────
    int zoom = 15;
    double latRad = qDegreesToRadians(lat);
    int tileX = int((lon + 180.0) / 360.0 * (1 << zoom));
    int tileY = int((1.0 - std::log(std::tan(latRad) + 1.0 / std::cos(latRad)) / M_PI) / 2.0 * (1 << zoom));

    // ── Step 3: Download a 3x3 grid of tiles (each 256x256) ─────────────────
    const int TILE_SIZE = 256;
    const int GRID = 3; // 3x3 tiles = 768x768, we'll crop to 640x480
    QPixmap mapPixmap(TILE_SIZE * GRID, TILE_SIZE * GRID);
    mapPixmap.fill(Qt::lightGray);
    QPainter painter(&mapPixmap);

    bool anyTileLoaded = false;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int tx = tileX + dx;
            int ty = tileY + dy;
            QUrl tileUrl(QString("https://tile.openstreetmap.org/%1/%2/%3.png").arg(zoom).arg(tx).arg(ty));
            QNetworkRequest tileReq(tileUrl);
            tileReq.setHeader(QNetworkRequest::UserAgentHeader, "SmartLeatherApp/1.0");
            QNetworkReply *tileReply = nam.get(tileReq);

            QEventLoop tileLoop;
            connect(tileReply, &QNetworkReply::finished, &tileLoop, &QEventLoop::quit);
            QTimer::singleShot(5000, &tileLoop, &QEventLoop::quit);
            tileLoop.exec();

            if (tileReply->error() == QNetworkReply::NoError) {
                QPixmap tile;
                if (tile.loadFromData(tileReply->readAll())) {
                    painter.drawPixmap((dx + 1) * TILE_SIZE, (dy + 1) * TILE_SIZE, tile);
                    anyTileLoaded = true;
                }
            }
            tileReply->deleteLater();
        }
    }

    // ── Step 4: Draw red marker at center ────────────────────────────────────
    if (anyTileLoaded) {
        int cx = TILE_SIZE + TILE_SIZE / 2;
        int cy = TILE_SIZE + TILE_SIZE / 2;
        painter.setRenderHint(QPainter::Antialiasing);
        // Shadow
        painter.setBrush(QColor(0, 0, 0, 80));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(cx - 10, cy + 20, 20, 8);
        // Pin body
        painter.setBrush(QColor(220, 50, 50));
        painter.setPen(QPen(Qt::white, 2));
        painter.drawEllipse(cx - 12, cy - 28, 24, 24);
        // Pin tip
        painter.setBrush(QColor(220, 50, 50));
        painter.setPen(Qt::NoPen);
        QPolygon pin;
        pin << QPoint(cx - 5, cy - 6) << QPoint(cx + 5, cy - 6) << QPoint(cx, cy + 4);
        painter.drawPolygon(pin);
        // Inner dot
        painter.setBrush(Qt::white);
        painter.drawEllipse(cx - 5, cy - 21, 10, 10);
    }
    painter.end();

    // Crop center 640x480
    QPixmap cropped = mapPixmap.copy((TILE_SIZE * GRID - 640) / 2, (TILE_SIZE * GRID - 480) / 2, 640, 480);
    if (anyTileLoaded) {
        mapLabel->setPixmap(cropped);
    } else {
        mapLabel->setText("❌ Impossible de charger les tuiles de la carte.\nVérifiez votre connexion internet.");
    }

    dialog->exec();
    delete dialog;
}

// Recommandations
void fournisseurs::on_pushButton_recommandation_clicked()
{
    // Fetch all active suppliers
    QSqlQuery query("SELECT NOM, PRIX_UNITAIRE, FIABILITE, TYPE_MATIERE, STATUT "
                    "FROM SMARTLEATHER.FOURNISSEUR WHERE UPPER(TRIM(STATUT)) = 'ACTIF'");

    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la requête :\n" + query.lastError().text());
        return;
    }

    struct FournisseurInfo {
        QString nom, type;
        double prix, fiab, score;
    };
    QList<FournisseurInfo> liste;

    while (query.next()) {
        FournisseurInfo f;
        f.nom  = query.value(0).toString();
        f.prix = query.value(1).toDouble();
        f.fiab = query.value(2).toDouble();
        f.type = query.value(3).toString();
        // Score: fiabilite / (prix+1) so that prix=0 doesn't exclude
        f.score = f.fiab / (f.prix + 1.0);
        liste.append(f);
    }

    if (liste.isEmpty()) {
        QMessageBox::information(this, "Recommandation",
            "Aucun fournisseur avec statut 'Actif' trouvé.\n\n"
            "Vérifiez que le champ STATUT contient bien 'Actif'.");
        return;
    }

    // Sort by score descending
    std::sort(liste.begin(), liste.end(), [](const FournisseurInfo &a, const FournisseurInfo &b) {
        return a.score > b.score;
    });

    const FournisseurInfo &best = liste.first();

    // Build a styled result dialog
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Recommandation IA");
    dialog->setFixedSize(420, 300);
    dialog->setStyleSheet(
        "QDialog { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #faf6f1,stop:1 #e8ddd0); }"
        "QLabel { color: #3b2a1a; }"
        "QPushButton { background:#6b3f2b; color:white; border-radius:8px; padding:10px 28px; font-weight:bold; }"
        "QPushButton:hover { background:#8a5a44; }"
    );

    QVBoxLayout *lay = new QVBoxLayout(dialog);
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(10);

    QLabel *star = new QLabel("⭐ Meilleur Fournisseur Recommandé", dialog);
    star->setStyleSheet("font-size:15px; font-weight:bold; color:#4a2517;");
    star->setAlignment(Qt::AlignCenter);
    lay->addWidget(star);

    QLabel *nom = new QLabel("<b>" + best.nom + "</b>", dialog);
    nom->setStyleSheet("font-size:22px; color:#6b3f2b;");
    nom->setAlignment(Qt::AlignCenter);
    lay->addWidget(nom);

    QFrame *sep = new QFrame(dialog);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #c9b8a5;");
    lay->addWidget(sep);

    auto addInfo = [&](QString label, QString val) {
        QLabel *l = new QLabel(QString("<b>%1</b> %2").arg(label, val), dialog);
        l->setStyleSheet("font-size:13px;");
        lay->addWidget(l);
    };

    addInfo("🧵 Matière :", best.type);
    addInfo("💰 Prix unitaire :", best.prix > 0 ? QString::number(best.prix, 'f', 2) + " TND" : "Non défini");
    addInfo("✅ Fiabilité :", QString::number(best.fiab, 'f', 1) + " %");
    addInfo("📊 Score :", QString::number(best.score, 'f', 4));
    addInfo("📦 Fournisseurs actifs :", QString::number(liste.size()));

    lay->addStretch();

    QPushButton *btn = new QPushButton("OK", dialog);
    connect(btn, &QPushButton::clicked, dialog, &QDialog::accept);
    lay->addWidget(btn, 0, Qt::AlignCenter);

    dialog->exec();
    delete dialog;
}

// Sidebar navigation
void fournisseurs::on_pushButton_15_clicked() { hide(); pageemployee *p = new pageemployee(m_idEmploye, this); p->show(); }
void fournisseurs::on_pushButton_20_clicked() { hide(); commandes *p = new commandes(m_idEmploye, this); p->show(); }
void fournisseurs::on_pushButton_21_clicked() { hide(); produitswindow *p = new produitswindow(m_idEmploye, this); p->show(); }
void fournisseurs::on_pushButton_22_clicked() { hide(); Matieres *p = new Matieres(m_idEmploye, this); p->show(); }
void fournisseurs::on_pushButton_23_clicked() { hide(); pagemachine *p = new pagemachine(m_idEmploye, this); p->show(); }
void fournisseurs::on_pushButton_16_clicked() { hide(); login *l = new login(); l->show(); }
