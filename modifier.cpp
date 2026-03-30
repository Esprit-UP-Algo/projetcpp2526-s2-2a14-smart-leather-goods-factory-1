#include "modifier.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QDate>

Modifier::Modifier(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Modifier Commande");
    setFixedSize(500, 700);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    QLabel *header = new QLabel("MODIFIER COMMANDE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    // Create fields (read-only ID)
    idEdit = new QLineEdit();
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc;");

    clientEdit = new QLineEdit();
    clientEdit->setAlignment(Qt::AlignCenter);

    addressEdit = new QLineEdit();
    addressEdit->setAlignment(Qt::AlignCenter);

    dateCommandeEdit = new QDateTimeEdit();
    dateCommandeEdit->setDisplayFormat("dd/MM/yyyy");
    dateCommandeEdit->setCalendarPopup(true);

    dateLivraisonEdit = new QDateTimeEdit();
    dateLivraisonEdit->setDisplayFormat("dd/MM/yyyy");
    dateLivraisonEdit->setCalendarPopup(true);

    etatCombo = new QComboBox();
    etatCombo->addItems({"En attente", "Confirmée", "En préparation", "Expédiée", "Livrée", "Annulée"});

    montantEdit = new QLineEdit();
    montantEdit->setAlignment(Qt::AlignCenter);

    modePaiementCombo = new QComboBox();
    modePaiementCombo->addItems({"Carte bancaire", "Espèces", "Virement", "Chèque", "PayPal"});

    // Add to layout
    mainLayout->addWidget(new QLabel("🔑 ID Commande :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("👤 Client :"));
    mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(new QLabel("📍 Adresse :"));
    mainLayout->addWidget(addressEdit);
    mainLayout->addWidget(new QLabel("📅 Date commande :"));
    mainLayout->addWidget(dateCommandeEdit);
    mainLayout->addWidget(new QLabel("📅 Date livraison :"));
    mainLayout->addWidget(dateLivraisonEdit);
    mainLayout->addWidget(new QLabel("⚡ État :"));
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(new QLabel("💰 Montant :"));
    mainLayout->addWidget(montantEdit);
    mainLayout->addWidget(new QLabel("💳 Mode paiement :"));
    mainLayout->addWidget(modePaiementCombo);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setObjectName("btnSave");
    btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    setupStyle();
}

void Modifier::setInitialData(const QString &id, const QString &client, const QString &address,
                             const QString &dateCommande, const QString &dateLivraison,
                             const QString &etat, const QString &montant, const QString &modePaiement)
{
    idEdit->setText(id);
    clientEdit->setText(client);
    addressEdit->setText(address);
    dateCommandeEdit->setDate(QDate::fromString(dateCommande, "dd/MM/yyyy"));
    dateLivraisonEdit->setDate(QDate::fromString(dateLivraison, "dd/MM/yyyy"));
    etatCombo->setCurrentText(etat);
    montantEdit->setText(montant);
    modePaiementCombo->setCurrentText(modePaiement);
}

void Modifier::setupStyle()
{
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
