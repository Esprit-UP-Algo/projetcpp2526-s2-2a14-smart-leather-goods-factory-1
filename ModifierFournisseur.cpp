#include "modifierfournisseur.h"

ModifierFournisseur::ModifierFournisseur(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Modifier le Fournisseur");
    setFixedSize(450, 650);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    // Title
    QLabel *header = new QLabel("MODIFIER FOURNISSEUR");
    header->setObjectName("headerLabel");
    mainLayout->addWidget(header);

    // Form Fields
    nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom du Fournisseur");
    nomEdit->setAlignment(Qt::AlignCenter);

    typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir bovin", "Cuir ovin (mouton)", "Cuir caprin (chèvre)", "Cuir de veau"});

    telephoneEdit = new QLineEdit();
    telephoneEdit->setPlaceholderText("Numéro de Téléphone");
    telephoneEdit->setAlignment(Qt::AlignCenter);

    adresseEdit = new QLineEdit();
    adresseEdit->setPlaceholderText("Adresse du Fournisseur");
    adresseEdit->setAlignment(Qt::AlignCenter);

    delaiDate = new QDateEdit(QDate::currentDate().addDays(30));
    delaiDate->setCalendarPopup(true);

    qualiteEdit = new QLineEdit();
    qualiteEdit->setPlaceholderText("Qualité de Matière");
    qualiteEdit->setAlignment(Qt::AlignCenter);

    statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Inactif"});

    // Add widgets to layout
    mainLayout->addWidget(new QLabel("Informations générales :"));
    mainLayout->addWidget(nomEdit);

    mainLayout->addWidget(new QLabel("Type de matière :"));
    mainLayout->addWidget(typeCombo);

    mainLayout->addWidget(new QLabel("Contact :"));
    mainLayout->addWidget(telephoneEdit);
    mainLayout->addWidget(adresseEdit);

    mainLayout->addWidget(new QLabel("Délai de livraison :"));
    mainLayout->addWidget(delaiDate);

    mainLayout->addWidget(new QLabel("Qualité & Statut :"));
    mainLayout->addWidget(qualiteEdit);
    mainLayout->addWidget(statutCombo);

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

void ModifierFournisseur::setupStyle() {
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
        "QLineEdit, QDateEdit, QComboBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "   color: #3a2a20;"
        "}"
        "QLineEdit:focus { border: 2px solid #6b3e26; }"
        "QComboBox::drop-down {"
        "   border-left: 1px solid #c9b2a2;"
        "   border-radius: 0px 10px 10px 0px;"
        "}"
        "QPushButton {"
        "   border-radius: 10px; padding: 10px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "}"
        "QPushButton#btnSave { background-color: #d07a2d; }" /* Orange pour modifier */
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
        );
}

// Helper to fill data
void ModifierFournisseur::setInitialData(QString nom, QString type, QString tel,
                                         QString addr, QDate delai, QString qualite,
                                         QString statut) {
    nomEdit->setText(nom);

    // Set combobox to matching type
    int typeIndex = typeCombo->findText(type);
    if (typeIndex >= 0) {
        typeCombo->setCurrentIndex(typeIndex);
    }

    telephoneEdit->setText(tel);
    adresseEdit->setText(addr);
    delaiDate->setDate(delai);
    qualiteEdit->setText(qualite);

    // Set combobox to matching status
    int statutIndex = statutCombo->findText(statut);
    if (statutIndex >= 0) {
        statutCombo->setCurrentIndex(statutIndex);
    }
}
