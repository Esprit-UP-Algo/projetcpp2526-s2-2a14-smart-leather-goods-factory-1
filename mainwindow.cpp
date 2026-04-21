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
#include <QDir>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QImage>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPolygonF>
#include <QPdfWriter>
#include <QRegularExpression>
#include <QTableWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QScrollArea>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QShortcut>

#if __has_include(<QtWebEngineWidgets/QWebEngineView>)
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineCore/QWebEngineSettings>
#define FOURNISSEURS_HAS_WEBENGINE 1
#elif __has_include(<QWebEngineView>)
#include <QWebEngineView>
#include <QWebEngineSettings>
#define FOURNISSEURS_HAS_WEBENGINE 1
#else
#define FOURNISSEURS_HAS_WEBENGINE 0
#endif

#include <algorithm>
#include <cmath>
#include <functional>
#include <QHash>
#include <QSet>
#include <memory>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

namespace {

constexpr double kPi = 3.14159265358979323846;

class SignaturePadWidget : public QWidget
{
public:
    explicit SignaturePadWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setMinimumHeight(280);
        setCursor(Qt::CrossCursor);
        setMouseTracking(true);
        setAutoFillBackground(true);
    }

    void clear()
    {
        m_strokes.clear();
        m_activeStroke.clear();
        m_drawing = false;
        update();
    }

    bool hasSignature() const
    {
        return !m_strokes.isEmpty() || !m_activeStroke.isEmpty();
    }

    QImage exportImage(const QSize &targetSize = QSize(1200, 420)) const
    {
        const QSize exportSize = targetSize.isValid() ? targetSize : size();
        QImage image(exportSize, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::white);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if (width() > 0 && height() > 0) {
            painter.scale(static_cast<qreal>(exportSize.width()) / width(), static_cast<qreal>(exportSize.height()) / height());
        }
        renderCanvas(painter);
        return image;
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        renderCanvas(painter);
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton) {
            QWidget::mousePressEvent(event);
            return;
        }

        m_drawing = true;
        m_activeStroke = QPolygonF();
        m_activeStroke << event->position();
        update();
        event->accept();
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (!m_drawing || !(event->buttons() & Qt::LeftButton)) {
            QWidget::mouseMoveEvent(event);
            return;
        }

        m_activeStroke << event->position();
        update();
        event->accept();
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton || !m_drawing) {
            QWidget::mouseReleaseEvent(event);
            return;
        }

        m_activeStroke << event->position();
        if (!m_activeStroke.isEmpty()) {
            m_strokes << m_activeStroke;
        }
        m_activeStroke.clear();
        m_drawing = false;
        update();
        event->accept();
    }

private:
    QList<QPolygonF> m_strokes;
    QPolygonF m_activeStroke;
    bool m_drawing = false;

    void renderCanvas(QPainter &painter) const
    {
        painter.fillRect(rect(), Qt::white);
        painter.setPen(QPen(QColor("#6b3f2b"), 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawRect(rect().adjusted(0, 0, -1, -1));

        if (!hasSignature()) {
            painter.setPen(QColor("#7d6b5d"));
            painter.drawText(rect(), Qt::AlignCenter, "Signez ici avec la souris ou le doigt");
            return;
        }

        const auto drawStroke = [&painter](const QPolygonF &stroke) {
            if (stroke.size() == 1) {
                painter.drawEllipse(stroke.first(), 2.0, 2.0);
                return;
            }
            painter.drawPolyline(stroke);
        };

        painter.setPen(QPen(QColor("#1f1a17"), 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const QPolygonF &stroke : m_strokes) {
            drawStroke(stroke);
        }
        if (!m_activeStroke.isEmpty()) {
            drawStroke(m_activeStroke);
        }
    }
};

class SignatureDialog : public QDialog
{
public:
    explicit SignatureDialog(QWidget *parent = nullptr)
        : QDialog(parent)
        , m_signataireEdit(new QLineEdit(this))
        , m_pad(new SignaturePadWidget(this))
    {
        setWindowTitle("Signature de validation");
        resize(880, 620);

        auto *layout = new QVBoxLayout(this);

        auto *info = new QLabel(
            "Validez la recommandation par une signature manuscrite. Le fichier sera enregistré localement avec l'horodatage.",
            this);
        info->setWordWrap(true);
        info->setStyleSheet("color: #4a2517; font-weight: 600;");
        layout->addWidget(info);

        auto *form = new QFormLayout();
        m_signataireEdit->setPlaceholderText("Nom du valideur");
        form->addRow("Validé par", m_signataireEdit);
        layout->addLayout(form);

        auto *group = new QGroupBox("Zone de signature", this);
        auto *groupLayout = new QVBoxLayout(group);
        groupLayout->addWidget(m_pad);
        layout->addWidget(group, 1);

        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        auto *clearButton = new QPushButton("Effacer", this);
        buttons->addButton(clearButton, QDialogButtonBox::ResetRole);
        connect(clearButton, &QPushButton::clicked, m_pad, &SignaturePadWidget::clear);
        connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
            if (m_signataireEdit->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, "Signature", "Veuillez saisir le nom du valideur.");
                return;
            }
            if (!m_pad->hasSignature()) {
                QMessageBox::warning(this, "Signature", "Veuillez tracer une signature avant de valider.");
                return;
            }
            accept();
        });
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

    QString signataire() const
    {
        return m_signataireEdit->text().trimmed();
    }

    QImage signatureImage() const
    {
        return m_pad->exportImage();
    }

private:
    QLineEdit *m_signataireEdit = nullptr;
    SignaturePadWidget *m_pad = nullptr;
};

QString normaliserNomFichier(QString texte)
{
    texte = texte.simplified().trimmed();
    texte.replace(QRegularExpression("[^A-Za-z0-9]+"), "_");
    texte.remove(QRegularExpression("^_+|_+$"));
    if (texte.isEmpty()) {
        texte = "valideur";
    }
    return texte.toLower();
}

bool sauvegarderSignatureValidation(const QImage &signature, const QString &signataire, const QString &contexte, QString *cheminEnregistre)
{
    const QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (baseDir.isEmpty()) {
        return false;
    }

    QDir dir(baseDir);
    if (!dir.mkpath("signatures")) {
        return false;
    }

    const QString dossier = dir.filePath("signatures");
    const QString horodatage = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    const QString baseName = QString("validation_%1_%2").arg(horodatage, normaliserNomFichier(signataire));
    const QString imagePath = QDir(dossier).filePath(baseName + ".png");
    const QString metaPath = QDir(dossier).filePath(baseName + ".txt");

    if (!signature.save(imagePath)) {
        return false;
    }

    QFile metaFile(metaPath);
    if (metaFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&metaFile);
        stream << "Date: " << QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss") << "\n";
        stream << "Valideur: " << signataire << "\n";
        stream << contexte << "\n";
    }

    if (cheminEnregistre) {
        *cheminEnregistre = imagePath;
    }

    return true;
}

class InteractiveMapView : public QGraphicsView
{
public:
    explicit InteractiveMapView(QWidget *parent = nullptr)
        : QGraphicsView(parent)
        , m_scene(new QGraphicsScene(this))
        , m_manager(new QNetworkAccessManager(this))
    {
        setScene(m_scene);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::NoAnchor);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);
        setRenderHint(QPainter::Antialiasing, true);
        setRenderHint(QPainter::SmoothPixmapTransform, true);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        QObject::connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, [this]() {
            chargerTuilesVisibles();
        });
        QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [this]() {
            chargerTuilesVisibles();
        });
    }

    void setCenterAndMarker(double latitude, double longitude, int zoom)
    {
        m_centerLat = qBound(-85.0511, latitude, 85.0511);
        m_centerLon = longitude;
        m_zoom = qBound(2, zoom, 19);
        rebuildScene(true);
    }

    void zoomInStep()
    {
        zoomAtViewportCenter(+1);
    }

    void zoomOutStep()
    {
        zoomAtViewportCenter(-1);
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QGraphicsView::resizeEvent(event);
        chargerTuilesVisibles();
    }

    void wheelEvent(QWheelEvent *event) override
    {
        const int delta = event->angleDelta().y();
        if (delta == 0) {
            event->accept();
            return;
        }

        const QPointF sceneBefore = mapToScene(event->position().toPoint());
        const auto geo = worldToGeo(sceneBefore, m_zoom);
        const int newZoom = qBound(2, m_zoom + (delta > 0 ? 1 : -1), 19);

        if (newZoom != m_zoom) {
            m_zoom = newZoom;
            m_centerLat = geo.first;
            m_centerLon = geo.second;
            rebuildScene(true);
        }

        event->accept();
    }

private:
    QGraphicsScene *m_scene = nullptr;
    QNetworkAccessManager *m_manager = nullptr;
    QHash<QString, QGraphicsPixmapItem *> m_tiles;
    QSet<QString> m_pending;
    QGraphicsEllipseItem *m_marker = nullptr;
    QGraphicsSimpleTextItem *m_statusItem = nullptr;
    int m_failedRequests = 0;
    double m_centerLat = 36.8065;
    double m_centerLon = 10.1815;
    int m_zoom = 16;

    void zoomAtViewportCenter(int delta)
    {
        const QPointF sceneCenter = mapToScene(viewport()->rect().center());
        const auto geo = worldToGeo(sceneCenter, m_zoom);
        const int newZoom = qBound(2, m_zoom + delta, 19);
        if (newZoom == m_zoom) {
            return;
        }

        m_zoom = newZoom;
        m_centerLat = geo.first;
        m_centerLon = geo.second;
        rebuildScene(true);
    }

    static QPointF geoToWorld(double latitude, double longitude, int zoom)
    {
        const double latRad = latitude * kPi / 180.0;
        const double n = std::pow(2.0, zoom);
        const double x = (longitude + 180.0) / 360.0 * n * 256.0;
        const double y = (1.0 - std::log(std::tan(latRad) + (1.0 / std::cos(latRad))) / kPi) / 2.0 * n * 256.0;
        return QPointF(x, y);
    }

    static QPair<double, double> worldToGeo(const QPointF &world, int zoom)
    {
        const double n = std::pow(2.0, zoom);
        const double lon = (world.x() / (256.0 * n)) * 360.0 - 180.0;
        const double mercN = kPi * (1.0 - 2.0 * world.y() / (256.0 * n));
        const double lat = 180.0 / kPi * std::atan(std::sinh(mercN));
        return qMakePair(lat, lon);
    }

    static QUrl tileUrl(int zoom, int x, int y)
    {
        return QUrl(QStringLiteral("https://tile.openstreetmap.org/%1/%2/%3.png").arg(zoom).arg(x).arg(y));
    }

    void rebuildScene(bool recenter)
    {
        m_pending.clear();
        m_tiles.clear();
        m_scene->clear();
        m_marker = nullptr;
        m_statusItem = nullptr;
        m_failedRequests = 0;

        const double worldSize = 256.0 * std::pow(2.0, m_zoom);
        m_scene->setSceneRect(0, 0, worldSize, worldSize);

        const QPointF markerPos = geoToWorld(m_centerLat, m_centerLon, m_zoom);
        m_marker = m_scene->addEllipse(-10, -10, 20, 20, QPen(QColor(123, 30, 30), 3), QBrush(QColor(214, 40, 40)));
        m_marker->setPos(markerPos);
        m_marker->setZValue(10);

        auto *dot = m_scene->addEllipse(-3.5, -3.5, 7, 7, QPen(Qt::NoPen), QBrush(Qt::white));
        dot->setPos(markerPos);
        dot->setZValue(11);

        if (recenter) {
            centerOn(markerPos);
        }

        chargerTuilesVisibles();
    }

    void chargerTuilesVisibles()
    {
        if (!m_scene) {
            return;
        }

        const QRectF visible = mapToScene(viewport()->rect()).boundingRect();
        const int maxTile = (1 << m_zoom) - 1;

        const int minX = qMax(0, static_cast<int>(std::floor(visible.left() / 256.0)) - 1);
        const int maxX = qMin(maxTile, static_cast<int>(std::floor(visible.right() / 256.0)) + 1);
        const int minY = qMax(0, static_cast<int>(std::floor(visible.top() / 256.0)) - 1);
        const int maxY = qMin(maxTile, static_cast<int>(std::floor(visible.bottom() / 256.0)) + 1);

        for (int ty = minY; ty <= maxY; ++ty) {
            for (int tx = minX; tx <= maxX; ++tx) {
                const QString key = QStringLiteral("%1/%2/%3").arg(m_zoom).arg(tx).arg(ty);
                if (m_tiles.contains(key) || m_pending.contains(key)) {
                    continue;
                }

                m_pending.insert(key);
                QNetworkRequest req(tileUrl(m_zoom, tx, ty));
                req.setHeader(QNetworkRequest::UserAgentHeader, "FournisseursQt/1.0 (OSM-Tiles)");
                QNetworkReply *reply = m_manager->get(req);

                QObject::connect(reply, &QNetworkReply::finished, this,
                    [this, reply, key, tx, ty]() {
                        m_pending.remove(key);
                        if (reply->error() == QNetworkReply::NoError) {
                            QPixmap tile;
                            if (tile.loadFromData(reply->readAll())) {
                                auto *item = m_scene->addPixmap(tile);
                                item->setPos(tx * 256.0, ty * 256.0);
                                item->setZValue(0);
                                m_tiles.insert(key, item);
                                if (m_statusItem) {
                                    m_scene->removeItem(m_statusItem);
                                    delete m_statusItem;
                                    m_statusItem = nullptr;
                                }
                            }
                        } else {
                            ++m_failedRequests;
                            if (m_tiles.isEmpty() && m_failedRequests >= 3 && !m_statusItem) {
                                m_statusItem = m_scene->addSimpleText("Connexion internet requise pour charger la carte.");
                                m_statusItem->setBrush(QBrush(QColor(212, 80, 80)));
                                m_statusItem->setZValue(20);
                                const QPointF center = mapToScene(viewport()->rect().center());
                                m_statusItem->setPos(center.x() - 180.0, center.y() - 10.0);
                            }
                        }
                        reply->deleteLater();
                    });
            }
        }
    }
};

QString nettoyerPourMaps(const QString &texte)
{
    return texte.simplified().trimmed();
}

QUrl construireUrlNominatim(const QString &recherche)
{
    QUrl url(QStringLiteral("https://nominatim.openstreetmap.org/search"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("format"), QStringLiteral("jsonv2"));
    query.addQueryItem(QStringLiteral("limit"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("addressdetails"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("q"), recherche);
    url.setQuery(query);
    return url;
}

QString normaliserPartieAdresse(QString partie)
{
    partie = partie.simplified().trimmed();
    partie.remove(QRegularExpression("\\bville\\b", QRegularExpression::CaseInsensitiveOption));
    partie = partie.simplified().trimmed();
    return partie;
}

QString ajouterContexteTunisie(QString valeur)
{
    if (valeur.contains("Tunisie", Qt::CaseInsensitive) || valeur.contains("Tunis", Qt::CaseInsensitive)) {
        return valeur;
    }
    return valeur + ", Tunisie";
}

QStringList construireCandidatsGeocodage(const QString &recherche)
{
    QStringList segments = recherche.split(',', Qt::SkipEmptyParts);
    QStringList parties;
    for (const QString &segment : segments) {
        const QString normalise = normaliserPartieAdresse(segment);
        if (!normalise.isEmpty() && !parties.contains(normalise, Qt::CaseInsensitive)) {
            parties << normalise;
        }
    }

    QStringList candidats;
    auto ajouter = [&](QString valeur) {
        valeur = nettoyerPourMaps(valeur);
        valeur = ajouterContexteTunisie(valeur);
        if (!valeur.isEmpty() && !candidats.contains(valeur, Qt::CaseInsensitive)) {
            candidats << valeur;
        }
    };

    if (parties.size() >= 4) {
        const QString pays = parties.value(0);
        const QString region = parties.value(1);
        const QString ville = parties.value(2);
        const QString rue = parties.value(3);
        const QString localisation = parties.value(4);

        ajouter(QStringList{rue, ville, region, pays}.join(", "));
        ajouter(QStringList{rue, ville, pays}.join(", "));
        ajouter(QStringList{rue, region, pays}.join(", "));
        ajouter(QStringList{ville, region, pays}.join(", "));
        ajouter(QStringList{ville, pays}.join(", "));
        if (!localisation.isEmpty()) {
            ajouter(QStringList{localisation, rue, ville, pays}.join(", "));
            ajouter(QStringList{localisation, rue, ville, region, pays}.join(", "));
        }
    }

    if (parties.size() >= 3) {
        const QString pays = parties.value(0);
        const QString region = parties.value(1);
        const QString ville = parties.value(2);
        ajouter(QStringList{ville, region, pays}.join(", "));
        ajouter(QStringList{region, ville, pays}.join(", "));
        ajouter(QStringList{ville, pays}.join(", "));
    }

    if (parties.size() >= 2) {
        const QString pays = parties.value(0);
        const QString ville = parties.value(1);
        ajouter(QStringList{ville, pays}.join(", "));
    }

    ajouter(recherche);
    return candidats;
}

void afficherErreurCarte(QLabel *mapLabel, const QString &message)
{
    mapLabel->setText(message);
    mapLabel->setStyleSheet("color: #d9534f; padding: 20px;");
    mapLabel->show();
}

bool afficherCarteInteractive(QLabel *mapLabel, const QString &nom, const QString &recherche, double latitude, double longitude)
{
#if FOURNISSEURS_HAS_WEBENGINE
        if (!mapLabel || !mapLabel->parentWidget()) {
                return false;
        }

        auto *layout = qobject_cast<QVBoxLayout *>(mapLabel->parentWidget()->layout());
        if (!layout) {
                return false;
        }

        auto *webView = new QWebEngineView(mapLabel->parentWidget());
        webView->setMinimumSize(800, 600);
        webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
        webView->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);

        const QString titre = nom.toHtmlEscaped();
        const QString adresse = recherche.toHtmlEscaped();
        const QString html = QString(R"HTML(
<!doctype html>
<html>
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
    <style>
        html, body { margin: 0; padding: 0; width: 100%%; height: 100%%; background: #efe7dd; }
        #map { width: 100%%; height: calc(100%% - 52px); }
        #meta {
            height: 52px;
            display: flex;
            flex-direction: column;
            justify-content: center;
            padding: 0 12px;
            background: #f6eee3;
            border-bottom: 1px solid #d8c2aa;
            color: #4a2517;
            font: 600 12px sans-serif;
            box-sizing: border-box;
        }
    </style>
</head>
<body>
    <div id="meta"><div>%3</div><div>%4</div></div>
    <div id="map"></div>
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
    <script>
        const map = L.map('map', { zoomControl: true }).setView([%1, %2], 16);
        L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
            maxZoom: 19,
            attribution: '&copy; OpenStreetMap contributors'
        }).addTo(map);
        L.marker([%1, %2]).addTo(map);
    </script>
</body>
</html>
)HTML")
                .arg(latitude, 0, 'f', 6)
                .arg(longitude, 0, 'f', 6)
                .arg(titre)
                .arg(adresse);

        webView->setHtml(html, QUrl(QStringLiteral("https://leafletjs.com/")));

        const int index = layout->indexOf(mapLabel);
        if (index >= 0) {
                layout->insertWidget(index, webView, 1);
        } else {
                layout->addWidget(webView, 1);
        }

        mapLabel->hide();
        mapLabel->deleteLater();
        return true;
#else
        Q_UNUSED(mapLabel);
        Q_UNUSED(nom);
        Q_UNUSED(recherche);
        Q_UNUSED(latitude);
        Q_UNUSED(longitude);
        return false;
#endif
}

} // namespace

void afficherCartePopupOpenStreetMap(QWidget *parent, const QString &nom, const QString &recherche)
{
    if (recherche.trimmed().isEmpty()) {
        QMessageBox::warning(parent, "Localisation",
            "Aucune adresse exploitable n'a été trouvée pour ce fournisseur.");
        return;
    }

    // Créer la popup
    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(QString("Localisation: %1").arg(nom));
    dialog->setModal(true);
    dialog->resize(850, 750);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    
    // Header avec les infos du fournisseur
    QGroupBox *infoGroup = new QGroupBox("Informations", dialog);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    
    QLabel *nomLabel = new QLabel(QString("Fournisseur: <b>%1</b>").arg(nom), dialog);
    QLabel *adresseLabel = new QLabel(QString("Adresse: <b>%1</b>").arg(recherche), dialog);
    nomLabel->setStyleSheet("color: #4a2517; font-size: 12px; padding: 5px;");
    adresseLabel->setStyleSheet("color: #4a2517; font-size: 11px; padding: 5px;");
    infoLayout->addWidget(nomLabel);
    infoLayout->addWidget(adresseLabel);
    
    mainLayout->addWidget(infoGroup);
    
    // Zone d'etat (texte) + vue interactive (zoom/deplacement)
    QLabel *mapLabel = new QLabel(dialog);
    mapLabel->setAlignment(Qt::AlignCenter);
    mapLabel->setText("Chargement de la carte...");
    mapLabel->setStyleSheet("color: #666; padding: 20px;");
    mapLabel->setWordWrap(true);

    auto *mapView = new InteractiveMapView(dialog);
    mapView->setMinimumSize(800, 600);
    mapView->hide();

    mainLayout->addWidget(mapLabel);
    mainLayout->addWidget(mapView, 1);
    
    QNetworkAccessManager *manager = new QNetworkAccessManager(dialog);
    auto candidats = std::make_shared<QStringList>(construireCandidatsGeocodage(recherche));
    auto geocoder = std::make_shared<std::function<void(int)>>();
    auto hadNetworkError = std::make_shared<bool>(false);

    *geocoder = [manager, mapLabel, mapView, nom, recherche, candidats, geocoder, hadNetworkError](int index) {
        if (index >= candidats->size()) {
            if (*hadNetworkError) {
                afficherErreurCarte(mapLabel, "Connexion internet requise pour afficher la carte (géocodage impossible hors-ligne).");
            } else {
                afficherErreurCarte(mapLabel, "Erreur: Adresse introuvable sur OpenStreetMap. Essayez une adresse plus précise.");
            }
            return;
        }

        QNetworkRequest geocodeRequest(construireUrlNominatim(candidats->at(index)));
        geocodeRequest.setHeader(QNetworkRequest::UserAgentHeader, "FournisseursQt/1.0 (OpenStreetMap)");
        QNetworkReply *geocodeReply = manager->get(geocodeRequest);

        QObject::connect(geocodeReply, &QNetworkReply::finished, mapLabel,
            [geocodeReply, manager, mapLabel, mapView, nom, recherche, candidats, geocoder, hadNetworkError, index]() {
                if (geocodeReply->error() != QNetworkReply::NoError) {
                    *hadNetworkError = true;
                    geocodeReply->deleteLater();
                    (*geocoder)(index + 1);
                    return;
                }

                QJsonParseError parseError;
                const QJsonDocument document = QJsonDocument::fromJson(geocodeReply->readAll(), &parseError);
                geocodeReply->deleteLater();

                if (parseError.error != QJsonParseError::NoError || !document.isArray() || document.array().isEmpty()) {
                    (*geocoder)(index + 1);
                    return;
                }

                const QJsonObject result = document.array().first().toObject();
                const double latitude = result.value(QStringLiteral("lat")).toString().toDouble();
                const double longitude = result.value(QStringLiteral("lon")).toString().toDouble();

                if (!qIsFinite(latitude) || !qIsFinite(longitude)) {
                    (*geocoder)(index + 1);
                    return;
                }

                if (afficherCarteInteractive(mapLabel, nom, recherche, latitude, longitude)) {
                    return;
                }

                mapView->setCenterAndMarker(latitude, longitude, 16);
                mapView->setToolTip(QString("%1\n%2").arg(nom, recherche));
                mapLabel->hide();
                mapView->show();
            });
    };

    (*geocoder)(0);
    
    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnZoomOut = new QPushButton("-", dialog);
    btnZoomOut->setMinimumWidth(46);
    btnZoomOut->setToolTip("Zoom arrière (-)");
    QPushButton *btnZoomIn = new QPushButton("+", dialog);
    btnZoomIn->setMinimumWidth(46);
    btnZoomIn->setToolTip("Zoom avant (+)");
    QPushButton *btnFermer = new QPushButton("Fermer", dialog);
    btnFermer->setMinimumWidth(100);
    btnFermer->setStyleSheet(
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c47a2c, stop:1 #e09a4c); "
        "   border: none; border-radius: 6px; padding: 8px 16px; color: white; font-weight: bold; "
        "} "
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d48a3c, stop:1 #f0aa5c); } "
    );

    QObject::connect(btnZoomIn, &QPushButton::clicked, dialog, [dialog, mapView]() {
        if (mapView->isVisible()) {
            mapView->zoomInStep();
            return;
        }
#if FOURNISSEURS_HAS_WEBENGINE
        if (auto *web = dialog->findChild<QWebEngineView *>()) {
            web->page()->runJavaScript("if (window.map) { map.zoomIn(); }");
        }
#endif
    });

    QObject::connect(btnZoomOut, &QPushButton::clicked, dialog, [dialog, mapView]() {
        if (mapView->isVisible()) {
            mapView->zoomOutStep();
            return;
        }
#if FOURNISSEURS_HAS_WEBENGINE
        if (auto *web = dialog->findChild<QWebEngineView *>()) {
            web->page()->runJavaScript("if (window.map) { map.zoomOut(); }");
        }
#endif
    });

    auto *shortcutPlus = new QShortcut(QKeySequence(Qt::Key_Plus), dialog);
    auto *shortcutMinus = new QShortcut(QKeySequence(Qt::Key_Minus), dialog);
    QObject::connect(shortcutPlus, &QShortcut::activated, btnZoomIn, &QPushButton::click);
    QObject::connect(shortcutMinus, &QShortcut::activated, btnZoomOut, &QPushButton::click);

    QObject::connect(btnFermer, &QPushButton::clicked, dialog, &QDialog::close);
    btnLayout->addStretch();
    btnLayout->addWidget(btnZoomOut);
    btnLayout->addWidget(btnZoomIn);
    btnLayout->addSpacing(8);
    btnLayout->addWidget(btnFermer);
    btnLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    
    dialog->exec();
}

QString construireRechercheMaps(const QString &nom, const QString &adresse)
{
    QStringList parties;

    Q_UNUSED(nom);

    const QString normalisee = adresse.simplified().trimmed();
    const QStringList segments = normalisee.split(QRegularExpression("[,-]"), Qt::SkipEmptyParts);
    for (const QString &segment : segments) {
        const QString propre = nettoyerPourMaps(segment);
        if (!propre.isEmpty()) {
            parties << propre;
        }
    }

    if (parties.isEmpty()) {
        return normalisee;
    }

    QString recherche = parties.join(", ");
    if (!recherche.contains("Tunisie", Qt::CaseInsensitive) &&
        !recherche.contains("Tunis", Qt::CaseInsensitive)) {
        recherche += ", Tunisie";
    }

    return recherche;
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

double borner01(double value)
{
    return std::max(0.0, std::min(1.0, value));
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

struct IAProfileWeights {
    double wOnTime = 0.45;
    double wQuality = 0.35;
    double wBudget = 0.20;
    QString label;
};

IAProfileWeights profileIAFromLabel(const QString &label)
{
    IAProfileWeights p;
    const QString v = label.trimmed().toUpper();
    p.label = label;

    if (v.contains("URGENT")) {
        p.wOnTime = 0.62;
        p.wQuality = 0.25;
        p.wBudget = 0.13;
    } else if (v.contains("ECON")) {
        p.wOnTime = 0.25;
        p.wQuality = 0.20;
        p.wBudget = 0.55;
    } else if (v.contains("PREMIUM")) {
        p.wOnTime = 0.25;
        p.wQuality = 0.60;
        p.wBudget = 0.15;
    } else {
        p.wOnTime = 0.45;
        p.wQuality = 0.35;
        p.wBudget = 0.20;
    }
    return p;
}

QString explicationIA(double pOnTime, double pQuality, double pBudget, double fiabiliteNorm)
{
    QStringList plus;
    QStringList moins;

    if (pOnTime >= 0.75) {
        plus << "delai predit favorable";
    } else if (pOnTime <= 0.45) {
        moins << "risque de retard";
    }

    if (pQuality >= 0.75) {
        plus << "qualite predite elevee";
    } else if (pQuality <= 0.45) {
        moins << "qualite predite fragile";
    }

    if (pBudget >= 0.75) {
        plus << "cout compatible budget";
    } else if (pBudget <= 0.45) {
        moins << "pression budget";
    }

    if (fiabiliteNorm >= 0.75) {
        plus << "fiabilite historique forte";
    } else if (fiabiliteNorm <= 0.45) {
        moins << "fiabilite historique faible";
    }

    if (plus.isEmpty() && moins.isEmpty()) {
        return "profil moyen, sans signal fort";
    }

    QString msg;
    if (!plus.isEmpty()) {
        msg += "+ " + plus.first();
    }
    if (!moins.isEmpty()) {
        if (!msg.isEmpty()) {
            msg += " | ";
        }
        msg += "- " + moins.first();
    }
    return msg;
}

struct RecommendationItem {
    QString id;
    QString nom;
    QString qualite;
    QDate delai;
    double coutUnitaire = 0.0;
    double coutTotal = 0.0;
    int scoreClassique = 0;
    int scoreIA = 0;
    double pOnTime = 0.0;
    double pQuality = 0.0;
    double pBudget = 0.0;
    QString explication;
};


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

    addSlice("1er choix", countA, QColor(107, 165, 57));
    addSlice("2eme choix", countB, QColor(229, 169, 61));
    addSlice("3eme choix", countC, QColor(212, 90, 58));
    addSlice("Autre", countAutre, QColor(143, 143, 143));

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Repartition des fournisseurs par qualite");
    chart->setTitleBrush(QBrush(QColor(74, 39, 23)));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor(255, 250, 245)));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent;");

    auto *barSet = new QBarSet("Nombre de fournisseurs");
    *barSet << countA << countB << countC << countAutre;
    barSet->setColor(QColor(139, 90, 58));

    auto *barSeries = new QBarSeries();
    barSeries->append(barSet);

    auto *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Comparaison par qualite (Histogramme)");
    barChart->setTitleBrush(QBrush(QColor(74, 39, 23)));
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setBackgroundBrush(QBrush(QColor(255, 250, 245)));

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

        const QStringList candidateTables = candidates;
        for (const QString &tableName : candidateTables) {
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
        QMessageBox::information(this, "Localisation", "Selectionnez un fournisseur pour l'afficher sur la carte.");
        return;
    }

    const QString recherche = construireRechercheMaps(nomSelectionne, adresseSelectionnee);
    if (recherche.isEmpty()) {
        QMessageBox::information(this, "Localisation", "Selectionnez un fournisseur avec une adresse exploitable.");
        return;
    }

    afficherCartePopupOpenStreetMap(this, nomSelectionne, recherche);
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

    auto *profilInput = new QComboBox(&besoinsDialog);
    profilInput->addItems({"Equilibre IA", "Urgent (priorite delai)", "Economique (priorite budget)", "Premium qualite"});

    auto *delaiInput = new QDateEdit(QDate::currentDate().addDays(30), &besoinsDialog);
    delaiInput->setCalendarPopup(true);
    delaiInput->setDisplayFormat("dd/MM/yyyy");

    form->addRow("Type matière", typeInput);
    form->addRow("Quantité demandée", quantiteInput);
    form->addRow("Budget max", budgetInput);
    form->addRow("Délai max", delaiInput);
    form->addRow("Profil IA", profilInput);

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
    const IAProfileWeights poidsIA = profileIAFromLabel(profilInput->currentText());

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

        item.scoreClassique = static_cast<int>(std::round(0.40 * qScore + 0.30 * bScore + 0.20 * dScore + 0.10 * fScore));

        const double qNorm = borner01(static_cast<double>(qScore) / 100.0);
        const double bNorm = borner01(static_cast<double>(bScore) / 100.0);
        const double dNorm = borner01(static_cast<double>(dScore) / 100.0);
        const double fNorm = borner01(fScore / 100.0);

        item.pOnTime = borner01(sigmoid(-1.20 + 2.15 * dNorm + 1.35 * fNorm + 0.45 * bNorm));
        item.pQuality = borner01(sigmoid(-1.10 + 2.35 * qNorm + 1.25 * fNorm));
        item.pBudget = borner01(sigmoid(-1.30 + 2.50 * bNorm + 0.30 * dNorm));

        const double risque = borner01(0.60 * (1.0 - fNorm) + 0.25 * (1.0 - dNorm) + 0.15 * (1.0 - bNorm));
        const double scorePred =
            poidsIA.wOnTime * item.pOnTime +
            poidsIA.wQuality * item.pQuality +
            poidsIA.wBudget * item.pBudget -
            0.20 * risque;

        item.scoreIA = static_cast<int>(std::round(100.0 * borner01(scorePred)));
        item.explication = explicationIA(item.pOnTime, item.pQuality, item.pBudget, fNorm);

        candidats.append(item);
    }

    if (candidats.isEmpty()) {
        QMessageBox::information(this, "Recommandation", "Aucun fournisseur actif ne correspond au type saisi.");
        return;
    }

    std::sort(candidats.begin(), candidats.end(), [](const RecommendationItem &a, const RecommendationItem &b) {
        if (a.scoreIA != b.scoreIA) {
            return a.scoreIA > b.scoreIA;
        }
        if (a.scoreClassique != b.scoreClassique) {
            return a.scoreClassique > b.scoreClassique;
        }
        return a.coutTotal < b.coutTotal;
    });

    const int topCount = std::min(3, static_cast<int>(candidats.size()));

    QDialog resultatDialog(this);
    resultatDialog.setWindowTitle("Top 3 fournisseurs recommandes");
    resultatDialog.resize(780, 340);

    auto *layout = new QVBoxLayout(&resultatDialog);
    auto *subtitle = new QLabel(
        QString("Besoin: %1 | Quantite: %2 | Budget: %3 TND | Delai: %4 | Profil: %5")
            .arg(typeCible)
            .arg(quantite)
            .arg(QString::number(budget, 'f', 2))
            .arg(dateLimite.toString("dd/MM/yyyy"))
            .arg(poidsIA.label),
        &resultatDialog);

    auto *table = new QTableWidget(topCount, 9, &resultatDialog);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Score IA", "Confiance", "Score classique", "Qualite", "Delai", "Cout estime", "Explication IA"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    for (int i = 0; i < topCount; ++i) {
        const RecommendationItem &r = candidats.at(i);
        table->setItem(i, 0, new QTableWidgetItem(r.id));
        table->setItem(i, 1, new QTableWidgetItem(r.nom));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(r.scoreIA)));

        const int confiance = static_cast<int>(std::round(100.0 * (r.pOnTime + r.pQuality + r.pBudget) / 3.0));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(confiance) + "%"));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(r.scoreClassique)));
        table->setItem(i, 5, new QTableWidgetItem(r.qualite));
        table->setItem(i, 6, new QTableWidgetItem(r.delai.isValid() ? r.delai.toString("dd/MM/yyyy") : "N/A"));
        table->setItem(i, 7, new QTableWidgetItem(QString::number(r.coutTotal, 'f', 2) + " TND"));
        table->setItem(i, 8, new QTableWidgetItem(r.explication));
    }

    layout->addWidget(subtitle);
    layout->addWidget(table);

    auto *buttonRow = new QHBoxLayout();
    auto *signBtn = new QPushButton("Signer la recommandation", &resultatDialog);
    signBtn->setToolTip("Valider le choix avec une signature manuscrite");
    auto *closeBtn = new QPushButton("Fermer", &resultatDialog);
    connect(signBtn, &QPushButton::clicked, &resultatDialog, [this, &resultatDialog, candidats, topCount, typeCible, quantite, budget, dateLimite, poidsIA]() {
        if (candidats.isEmpty()) {
            QMessageBox::information(&resultatDialog, "Signature", "Aucune recommandation disponible à signer.");
            return;
        }

        SignatureDialog signatureDialog(&resultatDialog);
        signatureDialog.setWindowTitle(QString("Signature de %1").arg(candidats.first().nom));
        if (signatureDialog.exec() != QDialog::Accepted) {
            return;
        }

        QString topSelection;
        const int limite = std::min(topCount, static_cast<int>(candidats.size()));
        for (int i = 0; i < limite; ++i) {
            const RecommendationItem &item = candidats.at(i);
            topSelection += QString("%1. %2 | Score IA=%3 | Score classique=%4 | Coût=%5 TND\n")
                                .arg(i + 1)
                                .arg(item.nom)
                                .arg(item.scoreIA)
                                .arg(item.scoreClassique)
                                .arg(QString::number(item.coutTotal, 'f', 2));
        }

        QString savedPath;
        const QString contexte = QString(
            "Contexte: type=%1, quantite=%2, budget=%3 TND, delai=%4, profil=%5\nTop 3:\n%6")
            .arg(typeCible)
            .arg(quantite)
            .arg(QString::number(budget, 'f', 2))
            .arg(dateLimite.toString("dd/MM/yyyy"))
            .arg(poidsIA.label)
            .arg(topSelection.trimmed());

        if (!sauvegarderSignatureValidation(signatureDialog.signatureImage(), signatureDialog.signataire(), contexte, &savedPath)) {
            QMessageBox::warning(&resultatDialog, "Signature", "La signature n'a pas pu être enregistrée.");
            return;
        }

        QMessageBox::information(
            &resultatDialog,
            "Signature enregistrée",
            QString("La recommandation a été signée et enregistrée.\nFichier: %1").arg(savedPath));
    });

    connect(closeBtn, &QPushButton::clicked, &resultatDialog, &QDialog::accept);
    buttonRow->addWidget(signBtn);
    buttonRow->addStretch();
    buttonRow->addWidget(closeBtn);
    layout->addLayout(buttonRow);

    resultatDialog.exec();
}

