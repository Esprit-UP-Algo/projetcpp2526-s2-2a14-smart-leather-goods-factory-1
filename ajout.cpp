#include "ajout.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QDate>

Ajout::Ajout(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Nouvelle Commande");
    setFixedSize(500, 700);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    // Heading
    QLabel *header = new QLabel("DÉTAILS COMMANDE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    // Form Fields for COMMANDES (not machines)
    idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID Commande (ex: C001)");
    idEdit->setAlignment(Qt::AlignCenter);

    clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("Nom du client");
    clientEdit->setAlignment(Qt::AlignCenter);

    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("Adresse de livraison");
    addressEdit->setAlignment(Qt::AlignCenter);

    dateCommandeEdit = new QDateTimeEdit(QDate::currentDate());
    dateCommandeEdit->setDisplayFormat("dd/MM/yyyy");
    dateCommandeEdit->setCalendarPopup(true);

    dateLivraisonEdit = new QDateTimeEdit(QDate::currentDate().addDays(7));
    dateLivraisonEdit->setDisplayFormat("dd/MM/yyyy");
    dateLivraisonEdit->setCalendarPopup(true);

    etatCombo = new QComboBox();
    etatCombo->addItems({"En attente", "Confirmée", "En préparation", "Expédiée", "Livrée", "Annulée"});

    montantEdit = new QLineEdit();
    montantEdit->setPlaceholderText("Montant total (€)");
    montantEdit->setAlignment(Qt::AlignCenter);

    modePaiementCombo = new QComboBox();
    modePaiementCombo->addItems({"Carte bancaire", "Espèces", "Virement", "Chèque", "PayPal"});

    // Add to layout
    mainLayout->addWidget(new QLabel("🔑 ID Commande :"));
    mainLayout->addWidget(idEdit);

    mainLayout->addWidget(new QLabel("👤 Client :"));
    mainLayout->addWidget(clientEdit);

    mainLayout->addWidget(new QLabel("📍 Adresse livraison :"));
    mainLayout->addWidget(addressEdit);

    mainLayout->addWidget(new QLabel("📅 Date commande :"));
    mainLayout->addWidget(dateCommandeEdit);

    mainLayout->addWidget(new QLabel("📅 Date livraison prévue :"));
    mainLayout->addWidget(dateLivraisonEdit);

    mainLayout->addWidget(new QLabel("⚡ État :"));
    mainLayout->addWidget(etatCombo);

    mainLayout->addWidget(new QLabel("💰 Montant total :"));
    mainLayout->addWidget(montantEdit);

    mainLayout->addWidget(new QLabel("💳 Mode de paiement :"));
    mainLayout->addWidget(modePaiementCombo);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setObjectName("btnSave");

    btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // Connections
    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    setupStyle();
}

void Ajout::setupStyle() {
    this->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   margin-bottom: 15px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 13px; }"
        "QLineEdit, QDateTimeEdit, QComboBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   color: #3a2a20;"
        "   font-size: 13px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 12px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "   font-size: 14px;"
        "   min-width: 120px;"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
        );
}
