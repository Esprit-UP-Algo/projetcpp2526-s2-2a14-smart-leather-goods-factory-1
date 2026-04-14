#include "ajoutfournisseur.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStyle>
#include <QScrollArea>
#include <QLabel>

namespace {
bool telephoneValide(const QString &telephone)
{
    return QRegularExpression("^\\d{8,15}$").match(telephone).hasMatch();
}

bool nomValide(const QString &nom)
{
    return QRegularExpression("^[A-ZÀ-Ý][A-Za-zÀ-ÿ\\s'-]{0,19}$").match(nom).hasMatch();
}

bool idValide(const QString &id)
{
    return QRegularExpression("^[A-Z][A-Z0-9]{2,19}$").match(id).hasMatch();
}
}

AjoutFournisseur::AjoutFournisseur(QWidget *parent)
    : QDialog(parent), m_validationEnabled(false)
{
    setWindowTitle("Ajouter un fournisseur");
    setModal(true);
    resize(580, 750);
    setMinimumSize(560, 680);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // Scroll area for form content
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // Section Identité
    auto *sectionIdentite = new QLabel("IDENTITÉ FOURNISSEUR");
    sectionIdentite->setObjectName("sectionHeader");
    mainLayout->addWidget(sectionIdentite);

    mainLayout->addWidget(new QLabel("ID fournisseur *"));
    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("Ex: F001");
    idEdit->setMaxLength(20);
    idEdit->setMinimumHeight(40);
    mainLayout->addWidget(idEdit);
    idHintLabel = new QLabel("ID obligatoire: commence par une majuscule, 3 a 20 caracteres (A-Z, 0-9).", this);
    idHintLabel->setObjectName("hintLabel");
    idHintLabel->setVisible(false);
    mainLayout->addWidget(idHintLabel);

    mainLayout->addWidget(new QLabel("Nom du fournisseur *"));
    nomEdit = new QLineEdit(this);
    nomEdit->setMaxLength(20);
    nomEdit->setPlaceholderText("Ex: Dupont");
    nomEdit->setMinimumHeight(40);
    mainLayout->addWidget(nomEdit);
    nomHintLabel = new QLabel("Le nom doit commencer par une majuscule (max 20 caracteres).", this);
    nomHintLabel->setObjectName("hintLabel");
    nomHintLabel->setVisible(false);
    mainLayout->addWidget(nomHintLabel);

    mainLayout->addWidget(new QLabel("Type de matière"));
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Bois", "Métal", "Cuir bovin", "Cuir ovin", "Cuir synthetique"});
    typeCombo->setMinimumHeight(40);
    mainLayout->addWidget(typeCombo);

    // Section Contact
    mainLayout->addSpacing(8);
    auto *sectionContact = new QLabel("CONTACT & LOCALISATION");
    sectionContact->setObjectName("sectionHeader");
    mainLayout->addWidget(sectionContact);

    mainLayout->addWidget(new QLabel("Téléphone *"));
    telephoneEdit = new QLineEdit(this);
    telephoneEdit->setPlaceholderText("Obligatoire - 8 a 15 chiffres");
    telephoneEdit->setMaxLength(15);
    telephoneEdit->setMinimumHeight(40);
    telephoneEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));
    mainLayout->addWidget(telephoneEdit);

    mainLayout->addWidget(new QLabel("Pays *"));
    paysEdit = new QLineEdit(this);
    paysEdit->setMaxLength(40);
    paysEdit->setPlaceholderText("Ex: Tunisie");
    paysEdit->setMinimumHeight(40);
    mainLayout->addWidget(paysEdit);

    mainLayout->addWidget(new QLabel("Region *"));
    regionEdit = new QLineEdit(this);
    regionEdit->setMaxLength(40);
    regionEdit->setPlaceholderText("Ex: Tunis");
    regionEdit->setMinimumHeight(40);
    mainLayout->addWidget(regionEdit);

    mainLayout->addWidget(new QLabel("Ville *"));
    villeEdit = new QLineEdit(this);
    villeEdit->setMaxLength(40);
    villeEdit->setPlaceholderText("Ex: Lac 2");
    villeEdit->setMinimumHeight(40);
    mainLayout->addWidget(villeEdit);

    mainLayout->addWidget(new QLabel("Rue *"));
    rueEdit = new QLineEdit(this);
    rueEdit->setMaxLength(80);
    rueEdit->setPlaceholderText("Ex: Rue de Marseille");
    rueEdit->setMinimumHeight(40);
    mainLayout->addWidget(rueEdit);

    mainLayout->addWidget(new QLabel("Localisation exacte *"));
    localisationExacteEdit = new QLineEdit(this);
    localisationExacteEdit->setMaxLength(120);
    localisationExacteEdit->setPlaceholderText("Ex: Immeuble A, 2eme etage, porte 5");
    localisationExacteEdit->setMinimumHeight(40);
    mainLayout->addWidget(localisationExacteEdit);

    // Section Conditions
    mainLayout->addSpacing(8);
    auto *sectionConditions = new QLabel("CONDITIONS DE LIVRAISON");
    sectionConditions->setObjectName("sectionHeader");
    mainLayout->addWidget(sectionConditions);

    mainLayout->addWidget(new QLabel("Délai de livraison"));
    delaiDate = new QDateEdit(QDate::currentDate(), this);
    delaiDate->setCalendarPopup(true);
    delaiDate->setMinimumHeight(40);
    mainLayout->addWidget(delaiDate);

    mainLayout->addWidget(new QLabel("Qualité matière"));
    qualiteCombo = new QComboBox(this);
    qualiteCombo->addItems({"1er choix", "2eme choix", "3eme choix"});
    qualiteCombo->setMinimumHeight(40);
    mainLayout->addWidget(qualiteCombo);

    mainLayout->addWidget(new QLabel("Statut"));
    statutCombo = new QComboBox(this);
    statutCombo->addItems({"Actif", "Inactif"});
    statutCombo->setMinimumHeight(40);
    mainLayout->addWidget(statutCombo);

    prixUnitaireSpin = new QDoubleSpinBox(this);
    prixUnitaireSpin->setRange(0.0, 1000000.0);
    prixUnitaireSpin->setDecimals(2);
    prixUnitaireSpin->setSingleStep(1.0);
    prixUnitaireSpin->setValue(0.0);
    prixUnitaireSpin->setSuffix(" TND");
    prixUnitaireSpin->hide();

    capaciteSpin = new QSpinBox(this);
    capaciteSpin->setRange(0, 100000000);
    capaciteSpin->setValue(0);
    capaciteSpin->hide();

    fiabiliteSpin = new QDoubleSpinBox(this);
    fiabiliteSpin->setRange(0.0, 100.0);
    fiabiliteSpin->setDecimals(1);
    fiabiliteSpin->setSingleStep(1.0);
    fiabiliteSpin->setValue(50.0);
    fiabiliteSpin->setSuffix(" %");
    fiabiliteSpin->hide();

    mainLayout->addSpacing(4);
    auto *btnPlus = new QPushButton("+ Plus - Options avancées", this);
    btnPlus->setObjectName("btnPlus");
    btnPlus->setToolTip("Ouvrir les options avancées (prix, capacité, fiabilité)");
    btnPlus->setMinimumHeight(36);
    mainLayout->addWidget(btnPlus, 0, Qt::AlignLeft);

    mainLayout->addStretch();
    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    // Button bar
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(12, 12, 12, 12);
    btnLayout->setSpacing(12);
    btnSave = new QPushButton("Enregistrer", this);
    btnSave->setObjectName("btnSave");
    btnSave->setMinimumHeight(44);
    btnCancel = new QPushButton("Annuler", this);
    btnCancel->setObjectName("btnCancel");
    btnCancel->setMinimumHeight(44);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    outerLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &AjoutFournisseur::validerEtAccepter);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(idEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(nomEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(paysEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(regionEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(villeEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(rueEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(localisationExacteEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(telephoneEdit, &QLineEdit::textChanged, this, &AjoutFournisseur::mettreAJourValidation);
    connect(btnPlus, &QPushButton::clicked, this, &AjoutFournisseur::ouvrirOptionsPlus);

    setupStyle();
    // Don't validate on init - let user start typing first
    btnSave->setEnabled(false);
}

void AjoutFournisseur::ouvrirOptionsPlus()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Plus - Options avancees");
    dialog.setModal(true);
    dialog.resize(420, 240);

    auto *layout = new QVBoxLayout(&dialog);

    auto *prixLabel = new QLabel("Prix unitaire estimé (TND)", &dialog);
    auto *prixInput = new QDoubleSpinBox(&dialog);
    prixInput->setRange(0.0, 1000000.0);
    prixInput->setDecimals(2);
    prixInput->setSingleStep(1.0);
    prixInput->setValue(prixUnitaireSpin->value());
    prixInput->setSuffix(" TND");

    auto *capaciteLabel = new QLabel("Capacité max (unités)", &dialog);
    auto *capaciteInput = new QSpinBox(&dialog);
    capaciteInput->setRange(0, 100000000);
    capaciteInput->setValue(capaciteSpin->value());

    auto *fiabiliteLabel = new QLabel("Taux fiabilité (%)", &dialog);
    auto *fiabiliteInput = new QDoubleSpinBox(&dialog);
    fiabiliteInput->setRange(0.0, 100.0);
    fiabiliteInput->setDecimals(1);
    fiabiliteInput->setSingleStep(1.0);
    fiabiliteInput->setValue(fiabiliteSpin->value());
    fiabiliteInput->setSuffix(" %");

    layout->addWidget(prixLabel);
    layout->addWidget(prixInput);
    layout->addWidget(capaciteLabel);
    layout->addWidget(capaciteInput);
    layout->addWidget(fiabiliteLabel);
    layout->addWidget(fiabiliteInput);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);

    if (dialog.exec() == QDialog::Accepted) {
        prixUnitaireSpin->setValue(prixInput->value());
        capaciteSpin->setValue(capaciteInput->value());
        fiabiliteSpin->setValue(fiabiliteInput->value());
    }
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
        "QLabel#hintLabel {"
        "   color: #b14a3f;"
        "   font-size: 10px;"
        "   font-weight: 500;"
        "   margin-top: -2px;"
        "   margin-bottom: 4px;"
        "}"
        "QLineEdit, QDateEdit, QComboBox, QDoubleSpinBox, QSpinBox {"
        "   background-color: rgba(255, 255, 255, 0.85);"
        "   border: 2px solid #d4c4b0;"
        "   border-radius: 12px;"
        "   padding: 10px 14px;"
        "   color: #3a2a20;"
        "   font-size: 13px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QSpinBox:focus {"
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
    const bool idOk = idValide(id);
    const QString nom = nomEdit->text().trimmed();
    const bool nomOk = nomValide(nom);
    const bool paysValide = !paysEdit->text().trimmed().isEmpty();
    const bool regionValide = !regionEdit->text().trimmed().isEmpty();
    const bool villeValide = !villeEdit->text().trimmed().isEmpty();
    const bool rueValide = !rueEdit->text().trimmed().isEmpty();
    const bool localisationValide = !localisationExacteEdit->text().trimmed().isEmpty();
    const QString telephone = telephoneEdit->text().trimmed();
    const bool telValide = telephoneValide(telephone);

    // Only show errors if validation is enabled (user has edited fields)
    if (m_validationEnabled) {
        idEdit->setProperty("error", !idOk);
        nomEdit->setProperty("error", !nomOk);
        paysEdit->setProperty("error", !paysValide);
        regionEdit->setProperty("error", !regionValide);
        villeEdit->setProperty("error", !villeValide);
        rueEdit->setProperty("error", !rueValide);
        localisationExacteEdit->setProperty("error", !localisationValide);
        telephoneEdit->setProperty("error", !telValide);
        idEdit->style()->unpolish(idEdit);
        idEdit->style()->polish(idEdit);
        nomEdit->style()->unpolish(nomEdit);
        nomEdit->style()->polish(nomEdit);
        paysEdit->style()->unpolish(paysEdit);
        paysEdit->style()->polish(paysEdit);
        regionEdit->style()->unpolish(regionEdit);
        regionEdit->style()->polish(regionEdit);
        villeEdit->style()->unpolish(villeEdit);
        villeEdit->style()->polish(villeEdit);
        rueEdit->style()->unpolish(rueEdit);
        rueEdit->style()->polish(rueEdit);
        localisationExacteEdit->style()->unpolish(localisationExacteEdit);
        localisationExacteEdit->style()->polish(localisationExacteEdit);
        telephoneEdit->style()->unpolish(telephoneEdit);
        telephoneEdit->style()->polish(telephoneEdit);

        idHintLabel->setVisible(!idOk);
        nomHintLabel->setVisible(!nomOk);
    }

    idEdit->setToolTip(idOk ? "" : "ID: majuscule initiale, 3 a 20 caracteres, lettres/chiffres seulement.");
    nomEdit->setToolTip(nomOk ? "" : "Le nom doit commencer par une majuscule et contenir max 20 caracteres.");
    paysEdit->setToolTip(paysValide ? "" : "Le pays est obligatoire.");
    regionEdit->setToolTip(regionValide ? "" : "La region est obligatoire.");
    villeEdit->setToolTip(villeValide ? "" : "La ville est obligatoire.");
    rueEdit->setToolTip(rueValide ? "" : "La rue est obligatoire.");
    localisationExacteEdit->setToolTip(localisationValide ? "" : "La localisation exacte est obligatoire.");
    telephoneEdit->setToolTip(telValide ? "" : "Le telephone est obligatoire (8 a 15 chiffres).");
    btnSave->setEnabled(idOk && nomOk && paysValide && regionValide && villeValide && rueValide && localisationValide && telValide);
}

void AjoutFournisseur::validerEtAccepter()
{
    m_validationEnabled = true;
    mettreAJourValidation();
    if (btnSave->isEnabled()) {
        accept();
    }
}


QString AjoutFournisseur::getAdresse() const
{
    return QString("%1 - %2 - %3 - %4 - %5")
        .arg(paysEdit->text().trimmed(),
             regionEdit->text().trimmed(),
             villeEdit->text().trimmed(),
             rueEdit->text().trimmed(),
             localisationExacteEdit->text().trimmed());
}


