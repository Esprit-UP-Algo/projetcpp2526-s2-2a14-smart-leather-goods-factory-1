#include "ajoutfournisseur.h"

#include <QHBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStyle>

namespace {
bool telephoneValide(const QString &telephone)
{
    return QRegularExpression("^\\d{8,15}$").match(telephone).hasMatch();
}
}

AjoutFournisseur::AjoutFournisseur(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Ajouter un fournisseur");
    setModal(true);
    resize(400, 560);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(new QLabel("ID fournisseur"));
    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("F0001");
    mainLayout->addWidget(idEdit);

    mainLayout->addWidget(new QLabel("Nom du fournisseur"));
    nomEdit = new QLineEdit(this);
    nomEdit->setMaxLength(80);
    mainLayout->addWidget(nomEdit);

    mainLayout->addWidget(new QLabel("Type de matière"));
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Bois", "Métal", "Plastique", "Verre"});
    mainLayout->addWidget(typeCombo);

    mainLayout->addWidget(new QLabel("Téléphone"));
    telephoneEdit = new QLineEdit(this);
    telephoneEdit->setPlaceholderText("Obligatoire - 8 a 15 chiffres");
    telephoneEdit->setMaxLength(15);
    telephoneEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));
    mainLayout->addWidget(telephoneEdit);

    mainLayout->addWidget(new QLabel("Adresse"));
    adresseEdit = new QLineEdit(this);
    adresseEdit->setMaxLength(150);
    adresseEdit->setPlaceholderText("Obligatoire - minimum 5 caracteres");
    mainLayout->addWidget(adresseEdit);

    mainLayout->addWidget(new QLabel("Délai de livraison"));
    delaiDate = new QDateEdit(QDate::currentDate(), this);
    delaiDate->setCalendarPopup(true);
    mainLayout->addWidget(delaiDate);

    mainLayout->addWidget(new QLabel("Qualité matière"));
    qualiteCombo = new QComboBox(this);
    qualiteCombo->addItems({"1er choix", "2eme choix", "3eme choix"});
    mainLayout->addWidget(qualiteCombo);

    mainLayout->addWidget(new QLabel("Statut"));
    statutCombo = new QComboBox(this);
    statutCombo->addItems({"Actif", "Inactif"});
    mainLayout->addWidget(statutCombo);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Enregistrer", this);
    btnSave->setObjectName("btnSave");
    btnCancel = new QPushButton("Annuler", this);
    btnCancel->setObjectName("btnCancel");

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(idEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(nomEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(adresseEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(telephoneEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);

    setupStyle();
    mettreAJourValidation();
}

void AjoutFournisseur::setupStyle()
{
    this->setStyleSheet(
        "QDialog {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #faf6f1, stop:0.5 #f0e8de, stop:1 #e8ddd0);"
        "   border: none;"
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
        "       stop:0 #6d9b3a, stop:1 #8fb85a);"
        "   border: none;"
        "   border-radius: 14px;"
        "   padding: 12px 28px;"
        "   font-weight: 700;"
        "   font-size: 13px;"
        "   color: white;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton#btnSave:hover {"
        "   background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7dab4a, stop:1 #9fc86a);"
        "}"
        "QPushButton#btnSave:pressed {"
        "   background: #5a8a2a;"
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

void AjoutFournisseur::mettreAJourValidation()
{
    const QString id = idEdit->text().trimmed();
    const bool idValide = id.size() >= 3;
    const QString nom = nomEdit->text().trimmed();
    const bool nomValide = nom.size() >= 3;
    const QString adresse = adresseEdit->text().trimmed();
    const bool adresseValide = adresse.size() >= 5;
    const QString telephone = telephoneEdit->text().trimmed();
    const bool telValide = telephoneValide(telephone);

    idEdit->setProperty("error", !idValide);
    nomEdit->setProperty("error", !nomValide);
    adresseEdit->setProperty("error", !adresseValide);
    telephoneEdit->setProperty("error", !telValide);
    idEdit->style()->unpolish(idEdit);
    idEdit->style()->polish(idEdit);
    nomEdit->style()->unpolish(nomEdit);
    nomEdit->style()->polish(nomEdit);
    adresseEdit->style()->unpolish(adresseEdit);
    adresseEdit->style()->polish(adresseEdit);
    telephoneEdit->style()->unpolish(telephoneEdit);
    telephoneEdit->style()->polish(telephoneEdit);

    idEdit->setToolTip(idValide ? "" : "L'ID est obligatoire (minimum 3 caracteres).");
    nomEdit->setToolTip(nomValide ? "" : "Le nom est obligatoire (minimum 3 caracteres).");
    adresseEdit->setToolTip(adresseValide ? "" : "L'adresse est obligatoire (minimum 5 caracteres).");
    telephoneEdit->setToolTip(telValide ? "" : "Le telephone est obligatoire (8 a 15 chiffres).");
    btnSave->setEnabled(idValide && nomValide && adresseValide && telValide);
}


