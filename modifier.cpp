#include "modifier.h"
#include <QHBoxLayout>
#include <QMessageBox>


Modifier::Modifier(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Modifier la Commande");
    setFixedSize(450, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    // Title
    QLabel *header = new QLabel("MODIFIER COMMANDE");
    header->setObjectName("headerLabel");
    mainLayout->addWidget(header);

    // Form Fields
    idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID Commande");
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true); // Usually ID shouldn't be changed

    clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("ID Client");
    clientEdit->setAlignment(Qt::AlignCenter);

    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("Adresse Livraison");
    addressEdit->setAlignment(Qt::AlignCenter);

    dateOrder = new QDateTimeEdit(QDateTime::currentDateTime());
    dateOrder->setCalendarPopup(true);
    dateOrder->setDisplayFormat("yyyy-MM-dd");
    dateOrder->setTime(QTime(0, 0, 0));
    dateOrder->setEnabled(false); // Disable updating date of commande

    dateDelivery = new QDateTimeEdit(QDateTime::currentDateTime().addDays(7));
    dateDelivery->setCalendarPopup(true);
    dateDelivery->setDisplayFormat("yyyy-MM-dd");
    dateDelivery->setTime(QTime(0, 0, 0));

    statusCombo = new QComboBox();
    statusCombo->addItems({"En attente", "En production", "Livrée"});

    amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("Montant Total");
    amountEdit->setAlignment(Qt::AlignCenter);

    // Add widgets to layout
    mainLayout->addWidget(new QLabel("Identifiants (Lecture seule) :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("Client :"));
    mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(new QLabel("Logistique :"));
    mainLayout->addWidget(addressEdit);
    mainLayout->addWidget(dateOrder);
    mainLayout->addWidget(dateDelivery);
    mainLayout->addWidget(new QLabel("Statut & Paiement :"));
    mainLayout->addWidget(statusCombo);
    mainLayout->addWidget(amountEdit);

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Mettre à jour");
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

void Modifier::setupStyle() {
    this->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
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
        "QPushButton#btnSave { background-color: #c47a2c; }" /* Original Modifier color */
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
        );
}

// Helper to fill data (Add this logic to your class)
void Modifier::setInitialData(QString id, QString client, QString addr, QString amount) {
    idEdit->setText(id);
    clientEdit->setText(client);
    addressEdit->setText(addr);
    amountEdit->setText(amount);
}


void Modifier::accept() {
    if (dateDelivery->date() < dateOrder->date()) {
        QMessageBox::warning(this, "Erreur de date", 
            "La date de livraison ne peut pas être antérieure à la date de commande.");
        return;
    }
    QDialog::accept();
}

void Modifier::setDates(const QDateTime &dateOrderIn, const QDateTime &dateDeliveryIn) {
    dateOrder->setDateTime(dateOrderIn);
    dateDelivery->setDateTime(dateDeliveryIn);
}
