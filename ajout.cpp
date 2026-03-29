#include "ajout.h"
#include <QMessageBox>


Ajout::Ajout(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Nouvelle Commande");
    setFixedSize(450, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    // Heading
    QLabel *header = new QLabel("DÉTAILS COMMANDE");
    header->setObjectName("headerLabel");
    mainLayout->addWidget(header);

    // Form Fields

    referenceEdit = new QLineEdit();
    referenceEdit->setPlaceholderText("Référence");
    referenceEdit->setAlignment(Qt::AlignCenter);

    clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("ID Client");
    clientEdit->setAlignment(Qt::AlignCenter);

    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("Adresse Livraison");
    addressEdit->setAlignment(Qt::AlignCenter);

    dateOrder = new QDateTimeEdit(QDateTime::currentDateTime());
    dateOrder->setCalendarPopup(true);
    dateOrder->setDisplayFormat("yyyy-MM-dd");
    dateOrder->setReadOnly(true); // User cannot change the order date
    dateOrder->setEnabled(false); // Visual cue that it's locked

    dateDelivery = new QDateTimeEdit(QDateTime::currentDateTime().addDays(7));
    dateDelivery->setCalendarPopup(true);
    dateDelivery->setDisplayFormat("yyyy-MM-dd");
    dateDelivery->setTime(QTime(0, 0, 0));

    statusCombo = new QComboBox();
    statusCombo->addItems({"En attente", "En production", "Livrée"});

    amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("Montant Total");
    amountEdit->setAlignment(Qt::AlignCenter);

    // Add to layout
    mainLayout->addWidget(new QLabel("Informations :"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(clientEdit);

    mainLayout->addWidget(addressEdit);
    mainLayout->addWidget(dateOrder);
    mainLayout->addWidget(dateDelivery);
    mainLayout->addWidget(new QLabel("Statut & Paiement :"));
    mainLayout->addWidget(statusCombo);
    mainLayout->addWidget(amountEdit);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Enregistrer");
    btnSave->setObjectName("btnSave");

    btnCancel = new QPushButton("Annuler");
    btnCancel->setObjectName("btnCancel");

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // Connections
    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    setupStyle();
}


void Ajout::accept() {
    if (dateDelivery->date() < dateOrder->date()) {
        QMessageBox::warning(this, "Erreur de date", 
            "La date de livraison ne peut pas être antérieure à la date de commande.");
        return; 
    }
    QDialog::accept(); // Close the dialog and return Accepted
}

void Ajout::setupStyle() {
    this->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;" /* The Stitch */
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   margin-bottom: 10px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; }"
        "QLineEdit, QDateTimeEdit, QComboBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "   color: #3a2a20;"
        "}"
        "QLineEdit:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 10px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
        );
}
