#include "modifier.h"
#include <QHBoxLayout>
#include <QMessageBox>


Modifier::Modifier(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Modifier la Commande");
    setFixedSize(450, 700);

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
    dateOrder->setTime(QTime(0, 0, 0));
    dateOrder->setEnabled(false); // Disable updating date of commande

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

    // Add widgets to layout
    mainLayout->addWidget(new QLabel("Identifiants (Lecture seule) :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("Client :"));
    mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(errorClient);
    mainLayout->addWidget(new QLabel("Logistique :"));
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

    connect(clientEdit, &QLineEdit::textChanged, this, &Modifier::validateForm);
    connect(addressEdit, &QLineEdit::textChanged, this, &Modifier::validateForm);
    connect(amountEdit, &QLineEdit::textChanged, this, &Modifier::validateForm);
    connect(dateDelivery, &QDateTimeEdit::dateChanged, this, &Modifier::validateForm);
    connect(dateOrder, &QDateTimeEdit::dateChanged, this, &Modifier::validateForm);

    setupStyle();
    validateForm();
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

void Modifier::setInitialData(QString id, QString client, QString addr, QString amount) {
    idEdit->setText(id);
    clientEdit->setText(client);
    addressEdit->setText(addr);
    amountEdit->setText(amount);
    validateForm();
}


void Modifier::validateForm() {
    bool isValid = true;
    QString normalStyle = "QLineEdit { background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px; padding: 8px; color: #3a2a20; } QLineEdit:focus { border: 2px solid #6b3e26; }";
    QString errorStyle = "QLineEdit { background-color: #fff9f9; border: 2px solid #a23b2a; border-radius: 10px; padding: 8px; color: #a23b2a; }";

    QString dateNormalStyle = "QDateTimeEdit { background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px; padding: 8px; color: #3a2a20; }";
    QString dateErrorStyle = "QDateTimeEdit { background-color: #fff9f9; border: 2px solid #a23b2a; border-radius: 10px; padding: 8px; color: #a23b2a; }";

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
        btnSave->setStyleSheet("QPushButton#btnSave { background-color: #c47a2c; } QPushButton { border-radius: 10px; padding: 10px; font-weight: bold; color: white; border-bottom: 3px solid rgba(0,0,0,0.2); } QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }");
    }
}

void Modifier::accept() {
    if (!btnSave->isEnabled()) return;
    QDialog::accept();
}

void Modifier::setDates(const QDateTime &dateOrderIn, const QDateTime &dateDeliveryIn) {
    dateOrder->setDateTime(dateOrderIn);
    dateDelivery->setDateTime(dateDeliveryIn);
    validateForm();
}
