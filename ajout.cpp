#include "ajout.h"
#include <QMessageBox>


Ajout::Ajout(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Nouvelle Commande");
    setFixedSize(450, 700);

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
    errorRef = new QLabel("");
    errorRef->setStyleSheet("color: #a23b2a; font-size: 11px; font-weight: normal; margin-top: -10px; margin-bottom: 5px;");

    clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("ID Client");
    clientEdit->setAlignment(Qt::AlignCenter);
    errorClient = new QLabel("");
    errorClient->setStyleSheet("color: #a23b2a; font-size: 11px; font-weight: normal; margin-top: -10px; margin-bottom: 5px;");

    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("Adresse Livraison");
    addressEdit->setAlignment(Qt::AlignCenter);
    errorAddr = new QLabel("");
    errorAddr->setStyleSheet("color: #a23b2a; font-size: 11px; font-weight: normal; margin-top: -10px; margin-bottom: 5px;");

    dateOrder = new QDateTimeEdit(QDateTime::currentDateTime());
    dateOrder->setCalendarPopup(true);
    dateOrder->setDisplayFormat("yyyy-MM-dd");
    dateOrder->setReadOnly(true); // User cannot change the order date
    dateOrder->setEnabled(false); // Visual cue that it's locked

    dateDelivery = new QDateTimeEdit(QDateTime::currentDateTime().addDays(7));
    dateDelivery->setCalendarPopup(true);
    dateDelivery->setDisplayFormat("yyyy-MM-dd");
    dateDelivery->setTime(QTime(0, 0, 0));
    errorDate = new QLabel("");
    errorDate->setStyleSheet("color: #a23b2a; font-size: 11px; font-weight: normal; margin-top: -10px; margin-bottom: 5px;");

    statusCombo = new QComboBox();
    statusCombo->addItems({"En attente", "En production", "Livrée"});

    amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("Montant Total");
    amountEdit->setAlignment(Qt::AlignCenter);
    errorAmount = new QLabel("");
    errorAmount->setStyleSheet("color: #a23b2a; font-size: 11px; font-weight: normal; margin-top: -10px; margin-bottom: 5px;");

    // Add to layout
    mainLayout->addWidget(new QLabel("Informations :"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(errorRef);
    mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(errorClient);

    mainLayout->addWidget(addressEdit);
    mainLayout->addWidget(errorAddr);
    mainLayout->addWidget(dateOrder);
    mainLayout->addWidget(dateDelivery);
    mainLayout->addWidget(errorDate);
    mainLayout->addWidget(new QLabel("Statut & Paiement :"));
    mainLayout->addWidget(statusCombo);
    mainLayout->addWidget(amountEdit);
    mainLayout->addWidget(errorAmount);

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

    connect(referenceEdit, &QLineEdit::textChanged, this, &Ajout::validateForm);
    connect(clientEdit, &QLineEdit::textChanged, this, &Ajout::validateForm);
    connect(addressEdit, &QLineEdit::textChanged, this, &Ajout::validateForm);
    connect(amountEdit, &QLineEdit::textChanged, this, &Ajout::validateForm);
    connect(dateDelivery, &QDateTimeEdit::dateChanged, this, &Ajout::validateForm);
    connect(dateOrder, &QDateTimeEdit::dateChanged, this, &Ajout::validateForm);

    setupStyle();
    validateForm();
}


void Ajout::validateForm() {
    bool isValid = true;
    QString normalStyle = "QLineEdit { background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px; padding: 8px; color: #3a2a20; } QLineEdit:focus { border: 2px solid #6b3e26; }";
    QString errorStyle = "QLineEdit { background-color: #fff9f9; border: 2px solid #a23b2a; border-radius: 10px; padding: 8px; color: #a23b2a; }";

    QString dateNormalStyle = "QDateTimeEdit { background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px; padding: 8px; color: #3a2a20; }";
    QString dateErrorStyle = "QDateTimeEdit { background-color: #fff9f9; border: 2px solid #a23b2a; border-radius: 10px; padding: 8px; color: #a23b2a; }";

    if (referenceEdit->text().trimmed().isEmpty()) { isValid = false; referenceEdit->setStyleSheet(errorStyle); errorRef->setText("Référence obligatoire"); } 
    else { referenceEdit->setStyleSheet(normalStyle); errorRef->setText(""); }

    if (clientEdit->text().trimmed().isEmpty()) { isValid = false; clientEdit->setStyleSheet(errorStyle); errorClient->setText("ID Client obligatoire"); } 
    else { clientEdit->setStyleSheet(normalStyle); errorClient->setText(""); }

    if (addressEdit->text().trimmed().isEmpty()) { isValid = false; addressEdit->setStyleSheet(errorStyle); errorAddr->setText("Adresse livraison obligatoire"); } 
    else { addressEdit->setStyleSheet(normalStyle); errorAddr->setText(""); }

    bool amountValid;
    double amount = amountEdit->text().toDouble(&amountValid);
    if (!amountValid || amount <= 0) { isValid = false; amountEdit->setStyleSheet(errorStyle); errorAmount->setText("Montant invalide ou manquant"); } 
    else { amountEdit->setStyleSheet(normalStyle); errorAmount->setText(""); }

    if (dateDelivery->date() < dateOrder->date()) { isValid = false; dateDelivery->setStyleSheet(dateErrorStyle); errorDate->setText("La date de livraison doit être > date de commande"); } 
    else { dateDelivery->setStyleSheet(dateNormalStyle); errorDate->setText(""); }

    btnSave->setEnabled(isValid);
    if (!isValid) {
        btnSave->setStyleSheet("QPushButton { background-color: #cccccc; color: #666666; border-radius: 10px; padding: 10px; font-weight: bold; border-bottom: 3px solid rgba(0,0,0,0.2); }");
    } else {
        btnSave->setStyleSheet("QPushButton#btnSave { background-color: #6f8f3d; } QPushButton { border-radius: 10px; padding: 10px; font-weight: bold; color: white; border-bottom: 3px solid rgba(0,0,0,0.2); } QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }");
    }
}

void Ajout::accept() {
    if (!btnSave->isEnabled()) return;
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
