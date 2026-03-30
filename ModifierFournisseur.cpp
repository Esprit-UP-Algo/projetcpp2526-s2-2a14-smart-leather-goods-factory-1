#include "modifierfournisseur.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStyle>

namespace {
bool telephoneValide(const QString &telephone)
{
    return QRegularExpression("^\\d{8,15}$").match(telephone).hasMatch();
}
}

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
    nomEdit->setMaxLength(80);

    typeCombo = new QComboBox();
    typeCombo->addItems({"Bois", "Métal", "Plastique", "Verre"});

    telephoneEdit = new QLineEdit();
    telephoneEdit->setPlaceholderText("Telephone obligatoire (8 a 15 chiffres)");
    telephoneEdit->setAlignment(Qt::AlignCenter);
    telephoneEdit->setMaxLength(15);
    telephoneEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));

    adresseEdit = new QLineEdit();
    adresseEdit->setPlaceholderText("Adresse du Fournisseur");
    adresseEdit->setAlignment(Qt::AlignCenter);
    adresseEdit->setMaxLength(150);

    delaiDate = new QDateEdit(QDate::currentDate().addDays(30));
    delaiDate->setCalendarPopup(true);

    qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"1er choix", "2eme choix", "3eme choix"});

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
    mainLayout->addWidget(qualiteCombo);
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
    connect(nomEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(adresseEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(telephoneEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);

    setupStyle();
    mettreAJourValidation();
}

void ModifierFournisseur::setupStyle() {
    this->setStyleSheet(
        "QDialog {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #faf6f1, stop:0.5 #f0e8de, stop:1 #e8ddd0);"
        "   border: none;"
        "}"
        "QLabel#headerLabel {"
        "   color: #4a2517;"
        "   font-size: 22px;"
        "   font-weight: 800;"
        "   letter-spacing: 2px;"
        "   padding: 8px 0;"
        "}"
        "QLabel {"
        "   color: #5b3a28;"
        "   font-weight: 600;"
        "   font-size: 12px;"
        "   background: transparent;"
        "}"
        "QLineEdit, QDateEdit, QComboBox {"
        "   background-color: rgba(255, 255, 255, 0.85);"
        "   border: 2px solid #d4c4b0;"
        "   border-radius: 12px;"
        "   padding: 10px 14px;"
        "   color: #3a2a20;"
        "   font-size: 13px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus {"
        "   border: 2px solid #8b6f5a;"
        "   background-color: white;"
        "}"
        "QLineEdit[error='true'] {"
        "   border: 2px solid #e74c3c;"
        "   background-color: #fdf2f2;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   padding-right: 10px;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "   background-color: #5b3020;"
        "   color: #f5efe8;"
        "}"
        "QCalendarWidget QAbstractItemView {"
        "   background-color: #faf6f1;"
        "   color: #3a2a20;"
        "   selection-background-color: #c9a87c;"
        "   selection-color: #2a1a12;"
        "}"
        "QPushButton#btnSave {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #c47a2c, stop:1 #e09a4c);"
        "   border: none;"
        "   border-radius: 14px;"
        "   padding: 12px 28px;"
        "   font-weight: 700;"
        "   font-size: 13px;"
        "   color: white;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton#btnSave:hover {"
        "   background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #d48a3c, stop:1 #f0aa5c);"
        "}"
        "QPushButton#btnSave:pressed {"
        "   background: #b06a1c;"
        "}"
        "QPushButton#btnSave:disabled {"
        "   background: #b0b0b0;"
        "   color: #e0e0e0;"
        "}"
        "QPushButton#btnCancel {"
        "   background: transparent;"
        "   border: 2px solid #c9b8a5;"
        "   border-radius: 14px;"
        "   padding: 12px 28px;"
        "   font-weight: 600;"
        "   font-size: 13px;"
        "   color: #8b7a6a;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton#btnCancel:hover {"
        "   background: rgba(0,0,0,0.04);"
        "   border-color: #a0907e;"
        "   color: #5b4a3a;"
        "}"
        );
}

void ModifierFournisseur::mettreAJourValidation()
{
    const QString nom = nomEdit->text().trimmed();
    const bool nomValide = nom.size() >= 3;
    const QString adresse = adresseEdit->text().trimmed();
    const bool adresseValide = adresse.size() >= 5;
    const QString telephone = telephoneEdit->text().trimmed();
    const bool telValide = telephoneValide(telephone);

    nomEdit->setProperty("error", !nomValide);
    adresseEdit->setProperty("error", !adresseValide);
    telephoneEdit->setProperty("error", !telValide);
    nomEdit->style()->unpolish(nomEdit);
    nomEdit->style()->polish(nomEdit);
    adresseEdit->style()->unpolish(adresseEdit);
    adresseEdit->style()->polish(adresseEdit);
    telephoneEdit->style()->unpolish(telephoneEdit);
    telephoneEdit->style()->polish(telephoneEdit);

    nomEdit->setToolTip(nomValide ? "" : "Le nom est obligatoire (minimum 3 caracteres).");
    adresseEdit->setToolTip(adresseValide ? "" : "L'adresse est obligatoire (minimum 5 caracteres).");
    telephoneEdit->setToolTip(telValide ? "" : "Le telephone est obligatoire (8 a 15 chiffres).");
    btnSave->setEnabled(nomValide && adresseValide && telValide);
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
    int qualiteIndex = qualiteCombo->findText(qualite);
    if (qualiteIndex >= 0) {
        qualiteCombo->setCurrentIndex(qualiteIndex);
    }

    // Set combobox to matching status
    int statutIndex = statutCombo->findText(statut);
    if (statutIndex >= 0) {
        statutCombo->setCurrentIndex(statutIndex);
    }

    mettreAJourValidation();
}
