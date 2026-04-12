#include "barcodeviewer.h"

BarcodeViewer::BarcodeViewer(QString code, QWidget *parent) : QDialog(parent), m_code(code) {
    setWindowTitle("Code à barres - " + code);
    setFixedSize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel *header = new QLabel("GÉNÉRATEUR DE CODE À BARRES");
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("font-size: 14px; font-weight: bold; color: #6b3e26;");
    layout->addWidget(header);
    
    // Placeholder space for the paintEvent
    layout->addSpacing(120);
    
    QLabel *info = new QLabel("REF: " + code);
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("font-family: monospace; font-size: 16px; color: #3a2a20; background: #e8ddd0; padding: 5px; border-radius: 5px;");
    layout->addWidget(info);
    
    layout->addStretch();
    
    QPushButton *btnClose = new QPushButton("FERMER");
    btnClose->setFixedHeight(40);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(btnClose);
    
    setupStyle();
}

void BarcodeViewer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Position du code à barres
    QRect barcodeRect(50, 60, 300, 100);
    painter.fillRect(barcodeRect, Qt::white);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(barcodeRect);

    // Algorithme de génération visuelle déterministe (simulant EAN/Code128)
    int x = barcodeRect.left() + 15;
    int y = barcodeRect.top() + 10;
    int h = barcodeRect.height() - 20;
    
    // Utilisation d'un hash simple de la chaîne pour la cohérence
    uint seed = 0;
    for(QChar c : m_code) seed = ((seed << 5) + seed) + c.unicode();
    
    for (int i = 0; i < 60; ++i) {
        // Déterminer la largeur du trait (1, 2 ou 3 pixels)
        int w = ((seed ^ (i * 997)) % 3) + 1;
        // Déterminer si le trait est noir ou blanc
        bool isBlack = ((seed ^ (i * 443)) % 2) == 0;
        
        if (isBlack && (x + w < barcodeRect.right() - 15)) {
            painter.fillRect(x, y, w, h, Qt::black);
        }
        x += w;
        if (x >= barcodeRect.right() - 15) break;
    }
}

void BarcodeViewer::setupStyle() {
    this->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 2px solid #6b3e26;"
        "   border-radius: 15px;"
        "}"
        "QPushButton {"
        "   background-color: #6b3e26;"
        "   color: white;"
        "   border-radius: 10px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #8c5a3c; }"
    );
}
