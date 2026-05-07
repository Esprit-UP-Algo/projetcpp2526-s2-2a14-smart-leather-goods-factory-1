#include "qrpaymentdialog.h"
#include "simplepayserver.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QStandardPaths>

// ─────────────────────────────────────────────────────────────────────────────
// CONFIGURATION : Mets ici ton lien Stripe (mode test ou production)
// Exemple : "https://buy.stripe.com/test_abc123"
// Laisse vide ("") pour utiliser le serveur local (meme Wi-Fi requis)
static const QString STRIPE_BASE_URL = "";
// ─────────────────────────────────────────────────────────────────────────────

QrPaymentDialog::QrPaymentDialog(const QString &ref,
                                 const QString &amount,
                                 const QString &clientName,
                                 QWidget *parent)
    : QDialog(parent), m_ref(ref), m_amount(amount), m_clientName(clientName)
{
    setWindowTitle(QString("QR Paiement - ") + ref);
    setFixedSize(440, 620);
    setAttribute(Qt::WA_DeleteOnClose);

    QString qrUrl;

    if (!STRIPE_BASE_URL.isEmpty()) {
        // ── Mode Stripe : lien reel, fonctionne partout ──────────────────────
        qrUrl = STRIPE_BASE_URL;
        // Optionnel : ajouter la ref en parametre si Stripe le supporte
        // qrUrl += "?client_reference_id=" + ref;
        buildUiStatic(qrUrl);
    } else {
        // ── Mode local : serveur HTTP sur le reseau Wi-Fi ────────────────────
        m_server = new SimplePayServer(ref, amount, clientName, this);
        if (!m_server->startListening()) {
            QMessageBox::warning(this, "Erreur",
                "Impossible de demarrer le serveur local.\n"
                "Configurez STRIPE_BASE_URL pour une utilisation sans Wi-Fi.");
            QTimer::singleShot(0, this, &QDialog::reject);
            return;
        }
        qrUrl = m_server->url();
        buildUi();
    }

    loadQrImage(qrUrl);
}

// ── UI ────────────────────────────────────────────────────────────────────────
void QrPaymentDialog::buildUi()
{
    QString serverUrl = m_server ? m_server->url() : "";

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 20);
    root->setSpacing(12);

    // Title
    QLabel *title = new QLabel("QR CODE - PAGE DE PAIEMENT");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:15px; font-weight:800; color:#4a2510; letter-spacing:1px;");
    root->addWidget(title);

    // Info box
    QFrame *box = new QFrame();
    box->setStyleSheet(
        "QFrame { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #6b3e26,stop:1 #4a2717); border-radius:12px; }");
    QVBoxLayout *bl = new QVBoxLayout(box);
    bl->setContentsMargins(16,12,16,12);
    bl->setSpacing(4);
    QLabel *l1 = new QLabel("Commande : " + m_ref);
    l1->setStyleSheet("color:#f0c060; font-weight:800; font-size:13px;");
    QLabel *l2 = new QLabel("Client : " + m_clientName);
    l2->setStyleSheet("color:#fffaf5; font-size:12px;");
    QLabel *l3 = new QLabel("Montant : " + m_amount + " TND");
    l3->setStyleSheet("color:#fffaf5; font-size:16px; font-weight:800;");
    bl->addWidget(l1); bl->addWidget(l2); bl->addWidget(l3);
    root->addWidget(box);

    // QR image
    m_qrLabel = new QLabel("Chargement...");
    m_qrLabel->setAlignment(Qt::AlignCenter);
    m_qrLabel->setFixedHeight(210);
    m_qrLabel->setWordWrap(true);
    m_qrLabel->setStyleSheet(
        "background:#fffaf5; border:2px solid #c9b2a2;"
        "border-radius:12px; font-size:12px; color:#8b6b55;");
    root->addWidget(m_qrLabel);

    // Instructions Wi-Fi
    QLabel *hint = new QLabel("Telephone et PC doivent etre sur le meme reseau Wi-Fi");
    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);
    hint->setStyleSheet("font-size:11px; color:#5b3a29; font-weight:700;");
    root->addWidget(hint);

    // URL principale
    m_urlLabel = new QLabel(serverUrl);
    m_urlLabel->setAlignment(Qt::AlignCenter);
    m_urlLabel->setWordWrap(true);
    m_urlLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_urlLabel->setStyleSheet(
        "font-family:monospace; font-size:11px; font-weight:800; color:#4a2717;"
        "background:#f0e6d8; border-radius:8px; padding:8px;");
    root->addWidget(m_urlLabel);

    // Autres IPs disponibles (si plusieurs cartes réseau)
    QStringList allIps = SimplePayServer::allLocalIps();
    if (allIps.size() > 1) {
        quint16 port = m_server ? m_server->serverPort() : 8080;
        QStringList altUrls;
        for (const QString &ip : allIps)
            altUrls << QString("http://%1:%2").arg(ip).arg(port);
        QLabel *altLabel = new QLabel("Autres adresses a essayer :\n" + altUrls.join("\n"));
        altLabel->setAlignment(Qt::AlignCenter);
        altLabel->setWordWrap(true);
        altLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        altLabel->setStyleSheet(
            "font-family:monospace; font-size:9px; color:#6b4a35;"
            "background:#ece0d4; border-radius:6px; padding:5px;");
        root->addWidget(altLabel);
    }

    // Avertissement pare-feu Windows
    QLabel *fwHint = new QLabel(
        "Si la page ne s'ouvre pas : Panneau de configuration"
        " → Pare-feu Windows → Autoriser le port " +
        (m_server ? QString::number(m_server->serverPort()) : "8080"));
    fwHint->setAlignment(Qt::AlignCenter);
    fwHint->setWordWrap(true);
    fwHint->setStyleSheet("font-size:9px; color:#8b4513; font-style:italic;");
    root->addWidget(fwHint);

    // Buttons — rangée 1
    QHBoxLayout *btns = new QHBoxLayout();
    btns->setSpacing(10);

    QPushButton *btnOpen = new QPushButton("Ouvrir sur ce PC");
    btnOpen->setStyleSheet(
        "QPushButton { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #c4890a,stop:1 #8b4513); color:white;"
        "border-radius:10px; padding:10px; font-weight:800;"
        "border-bottom:3px solid #7a4800; }"
        "QPushButton:hover{background:#d4980c;}"
        "QPushButton:pressed{margin-top:3px;border-bottom:1px solid #7a4800;}");

    m_btnCopy = new QPushButton("Copier URL");
    m_btnCopy->setStyleSheet(
        "QPushButton { background:#b3a398; color:#3a2a20;"
        "border-radius:10px; padding:10px; font-weight:700;"
        "border-bottom:3px solid rgba(0,0,0,.2); }"
        "QPushButton:hover{background:#c4b5ab;}");

    QPushButton *btnClose = new QPushButton("Fermer");
    btnClose->setStyleSheet(
        "QPushButton { background:#7a4a2e; color:#fffaf5;"
        "border-radius:10px; padding:10px; font-weight:700;"
        "border-bottom:3px solid #3a1f14; }"
        "QPushButton:hover{background:#8b5a3c;}");

    connect(btnOpen,   &QPushButton::clicked, this, &QrPaymentDialog::openInBrowser);
    connect(m_btnCopy, &QPushButton::clicked, this, &QrPaymentDialog::copyLink);
    connect(btnClose,  &QPushButton::clicked, this, &QDialog::accept);

    btns->addWidget(btnOpen,   2);
    btns->addWidget(m_btnCopy, 1);
    btns->addWidget(btnClose,  1);
    root->addLayout(btns);

    // Bouton sauvegarde QR — rangée 2
    m_btnSave = new QPushButton("Sauvegarder le QR Code (PNG)");
    m_btnSave->setEnabled(false); // activé après chargement de l'image
    m_btnSave->setStyleSheet(
        "QPushButton { background:#2e6b4a; color:#fffaf5;"
        "border-radius:10px; padding:9px; font-weight:700;"
        "border-bottom:3px solid #1a4a30; }"
        "QPushButton:hover{background:#3a8060;}"
        "QPushButton:disabled{background:#a0b8a8; color:#d0e0d8;}");
    connect(m_btnSave, &QPushButton::clicked, this, &QrPaymentDialog::saveQrImage);
    root->addWidget(m_btnSave);

    setStyleSheet(
        "QDialog { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #f7ede2,stop:1 #ecdcc6);"
        "border:3px dashed #c9b2a2; border-radius:22px; }");
}

void QrPaymentDialog::buildUiStatic(const QString &url)
{
    m_stripeUrl = url;

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 20);
    root->setSpacing(12);

    // Title
    QLabel *title = new QLabel("QR CODE - PAIEMENT EN LIGNE");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:15px; font-weight:800; color:#4a2510; letter-spacing:1px;");
    root->addWidget(title);

    // Info box
    QFrame *box = new QFrame();
    box->setStyleSheet(
        "QFrame { background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #6b3e26,stop:1 #4a2717); border-radius:12px; }");
    QVBoxLayout *bl = new QVBoxLayout(box);
    bl->setContentsMargins(16,12,16,12);
    bl->setSpacing(4);
    QLabel *l1 = new QLabel("Commande : " + m_ref);
    l1->setStyleSheet("color:#f0c060; font-weight:800; font-size:13px;");
    QLabel *l2 = new QLabel("Client : " + m_clientName);
    l2->setStyleSheet("color:#fffaf5; font-size:12px;");
    QLabel *l3 = new QLabel("Montant : " + m_amount + " TND");
    l3->setStyleSheet("color:#fffaf5; font-size:16px; font-weight:800;");
    bl->addWidget(l1); bl->addWidget(l2); bl->addWidget(l3);
    root->addWidget(box);

    // QR image
    m_qrLabel = new QLabel("Chargement...");
    m_qrLabel->setAlignment(Qt::AlignCenter);
    m_qrLabel->setFixedHeight(210);
    m_qrLabel->setWordWrap(true);
    m_qrLabel->setStyleSheet(
        "background:#fffaf5; border:2px solid #c9b2a2;"
        "border-radius:12px; font-size:12px; color:#8b6b55;");
    root->addWidget(m_qrLabel);

    // Instructions
    QLabel *hint = new QLabel(
        "Scannez pour payer via Stripe (fonctionne partout)");
    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);
    hint->setStyleSheet("font-size:11px; color:#5b3a29; font-weight:600;");
    root->addWidget(hint);

    // URL
    m_urlLabel = new QLabel(url);
    m_urlLabel->setAlignment(Qt::AlignCenter);
    m_urlLabel->setWordWrap(true);
    m_urlLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_urlLabel->setStyleSheet(
        "font-family:monospace; font-size:10px; color:#4a6785;"
        "background:#f0e6d8; border-radius:8px; padding:6px;");
    root->addWidget(m_urlLabel);

    // Buttons — rangée 1
    QHBoxLayout *btns = new QHBoxLayout();
    btns->setSpacing(10);

    QPushButton *btnOpen = new QPushButton("Ouvrir le lien");
    btnOpen->setStyleSheet(
        "QPushButton { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #c4890a,stop:1 #8b4513); color:white;"
        "border-radius:10px; padding:10px; font-weight:800;"
        "border-bottom:3px solid #7a4800; }"
        "QPushButton:hover{background:#d4980c;}"
        "QPushButton:pressed{margin-top:3px;border-bottom:1px solid #7a4800;}");

    m_btnCopy = new QPushButton("Copier URL");
    m_btnCopy->setStyleSheet(
        "QPushButton { background:#b3a398; color:#3a2a20;"
        "border-radius:10px; padding:10px; font-weight:700;"
        "border-bottom:3px solid rgba(0,0,0,.2); }"
        "QPushButton:hover{background:#c4b5ab;}");

    QPushButton *btnClose = new QPushButton("Fermer");
    btnClose->setStyleSheet(
        "QPushButton { background:#7a4a2e; color:#fffaf5;"
        "border-radius:10px; padding:10px; font-weight:700;"
        "border-bottom:3px solid #3a1f14; }"
        "QPushButton:hover{background:#8b5a3c;}");

    connect(btnOpen,   &QPushButton::clicked, this, &QrPaymentDialog::openInBrowser);
    connect(m_btnCopy, &QPushButton::clicked, this, &QrPaymentDialog::copyLink);
    connect(btnClose,  &QPushButton::clicked, this, &QDialog::accept);

    btns->addWidget(btnOpen,   2);
    btns->addWidget(m_btnCopy, 1);
    btns->addWidget(btnClose,  1);
    root->addLayout(btns);

    // Bouton sauvegarde QR — rangée 2
    m_btnSave = new QPushButton("Sauvegarder le QR Code (PNG)");
    m_btnSave->setEnabled(false); // activé après chargement de l'image
    m_btnSave->setStyleSheet(
        "QPushButton { background:#2e6b4a; color:#fffaf5;"
        "border-radius:10px; padding:9px; font-weight:700;"
        "border-bottom:3px solid #1a4a30; }"
        "QPushButton:hover{background:#3a8060;}"
        "QPushButton:disabled{background:#a0b8a8; color:#d0e0d8;}");
    connect(m_btnSave, &QPushButton::clicked, this, &QrPaymentDialog::saveQrImage);
    root->addWidget(m_btnSave);

    setStyleSheet(
        "QDialog { background:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        "stop:0 #f7ede2,stop:1 #ecdcc6);"
        "border:3px dashed #c9b2a2; border-radius:22px; }");
}

// ── Load QR from free online API ──────────────────────────────────────────────
void QrPaymentDialog::loadQrImage(const QString &url)
{
    // api.qrserver.com is free, no API key, very reliable
    QString apiUrl = QString("https://api.qrserver.com/v1/create-qr-code/"
                             "?size=200x200&ecc=M&data=")
                     + QString(QUrl::toPercentEncoding(url));

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished,
            this, &QrPaymentDialog::onQrImageLoaded);

    QNetworkRequest req((QUrl(apiUrl)));
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                     QNetworkRequest::NoLessSafeRedirectPolicy);
    nam->get(req);
}

void QrPaymentDialog::onQrImageLoaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        m_qrPixmap = QPixmap(); // reset
        m_qrPixmap.loadFromData(reply->readAll());
        if (!m_qrPixmap.isNull()) {
            applyQrPixmap();
            // Active le bouton de sauvegarde maintenant que l'image est disponible
            if (m_btnSave) m_btnSave->setEnabled(true);
        }
    } else {
        // Pas de connexion internet → affiche toutes les URLs à essayer
        QString allUrls;
        if (m_server) {
            quint16 port = m_server->serverPort();
            QStringList ips = SimplePayServer::allLocalIps();
            if (ips.isEmpty()) {
                allUrls = m_server->url();
            } else {
                QStringList urls;
                for (const QString &ip : ips)
                    urls << QString("http://%1:%2").arg(ip).arg(port);
                allUrls = urls.join("\n");
            }
        }
        m_qrLabel->setText(
            "Pas d'internet — image QR indisponible.\n"
            "Tapez une de ces adresses sur votre telephone\n"
            "(meme reseau Wi-Fi que ce PC) :\n\n" + allUrls);
        m_qrLabel->setStyleSheet(
            "background:#fff9f0; border:2px dashed #c9b2a2;"
            "border-radius:12px; font-size:10px; color:#5b3a29;"
            "font-weight:bold; padding:10px;");
    }
    reply->deleteLater();
}

void QrPaymentDialog::applyQrPixmap()
{
    if (m_qrLabel && !m_qrPixmap.isNull()) {
        m_qrLabel->setPixmap(
            m_qrPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_qrLabel->setStyleSheet(
            "background:#fffaf5; border:2px solid #c9b2a2;"
            "border-radius:12px; padding:8px;");
    }
}

void QrPaymentDialog::saveQrImage()
{
    if (m_qrPixmap.isNull()) {
        QMessageBox::warning(this, "QR Code", "L'image du QR Code n'est pas encore chargée.");
        return;
    }

    // Dossier par défaut : Documents de l'utilisateur
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultName = defaultDir + "/QR_" + m_ref + ".png";

    QString path = QFileDialog::getSaveFileName(
        this,
        "Sauvegarder le QR Code",
        defaultName,
        "Images PNG (*.png);;Images JPEG (*.jpg)");

    if (path.isEmpty()) return; // annulé par l'utilisateur

    // Sauvegarde l'image originale (200x200) en haute qualité
    if (m_qrPixmap.save(path)) {
        QMessageBox::information(this, "Sauvegarde réussie",
            "QR Code sauvegardé :\n" + path);
    } else {
        QMessageBox::critical(this, "Erreur",
            "Impossible de sauvegarder le fichier.\n"
            "Vérifiez les permissions du dossier.");
    }
}

// ── Slots ─────────────────────────────────────────────────────────────────────
void QrPaymentDialog::openInBrowser()
{
    if (m_server)
        QDesktopServices::openUrl(QUrl(m_server->url()));
    else if (!m_stripeUrl.isEmpty())
        QDesktopServices::openUrl(QUrl(m_stripeUrl));
}

void QrPaymentDialog::copyLink()
{
    QString url = m_server ? m_server->url() : m_stripeUrl;
    if (!url.isEmpty())
        QApplication::clipboard()->setText(url);

    m_btnCopy->setText("Copie OK !");
    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    connect(t, &QTimer::timeout, m_btnCopy,
            [this]{ m_btnCopy->setText("Copier URL"); });
    t->start(2000);
}
