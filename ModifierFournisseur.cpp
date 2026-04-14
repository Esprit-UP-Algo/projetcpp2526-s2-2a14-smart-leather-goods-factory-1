#include "modifierfournisseur.h"

#include <QDialogButtonBox>
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
}

ModifierFournisseur::ModifierFournisseur(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Modifier le Fournisseur");
    setModal(true);
    resize(580, 750);
    setMinimumSize(560, 680);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    QWidget *scrollWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    QLabel *header = new QLabel("MODIFIER FOURNISSEUR", this);
    header->setObjectName("headerLabel");
    mainLayout->addWidget(header);

    mainLayout->addWidget(new QLabel("Nom du fournisseur *", this));
    nomEdit = new QLineEdit(this);
    nomEdit->setPlaceholderText("Nom du fournisseur");
    nomEdit->setMaxLength(20);
    nomEdit->setMinimumHeight(40);
    mainLayout->addWidget(nomEdit);

    nomHintLabel = new QLabel("Le nom doit commencer par une majuscule (max 20 caracteres).", this);
    nomHintLabel->setObjectName("hintLabel");
    nomHintLabel->setVisible(false);
    mainLayout->addWidget(nomHintLabel);

    mainLayout->addWidget(new QLabel("Type de matière", this));
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Bois", "Métal", "Cuir bovin", "Cuir ovin", "Cuir synthetique"});
    typeCombo->setMinimumHeight(40);
    mainLayout->addWidget(typeCombo);

    mainLayout->addWidget(new QLabel("Téléphone *", this));
    telephoneEdit = new QLineEdit(this);
    telephoneEdit->setPlaceholderText("Telephone obligatoire (8 a 15 chiffres)");
    telephoneEdit->setMaxLength(15);
    telephoneEdit->setMinimumHeight(40);
    telephoneEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^\\d{0,15}$"), this));
    mainLayout->addWidget(telephoneEdit);

    mainLayout->addWidget(new QLabel("Pays *", this));
    paysEdit = new QLineEdit(this);
    paysEdit->setPlaceholderText("Pays");
    paysEdit->setMaxLength(40);
    paysEdit->setMinimumHeight(40);
    mainLayout->addWidget(paysEdit);

    mainLayout->addWidget(new QLabel("Region *", this));
    regionEdit = new QLineEdit(this);
    regionEdit->setPlaceholderText("Region");
    regionEdit->setMaxLength(40);
    regionEdit->setMinimumHeight(40);
    mainLayout->addWidget(regionEdit);

    mainLayout->addWidget(new QLabel("Ville *", this));
    villeEdit = new QLineEdit(this);
    villeEdit->setPlaceholderText("Ville");
    villeEdit->setMaxLength(40);
    villeEdit->setMinimumHeight(40);
    mainLayout->addWidget(villeEdit);

    mainLayout->addWidget(new QLabel("Rue *", this));
    rueEdit = new QLineEdit(this);
    rueEdit->setPlaceholderText("Rue");
    rueEdit->setMaxLength(80);
    rueEdit->setMinimumHeight(40);
    mainLayout->addWidget(rueEdit);

    mainLayout->addWidget(new QLabel("Localisation exacte *", this));
    localisationExacteEdit = new QLineEdit(this);
    localisationExacteEdit->setPlaceholderText("Localisation exacte");
    localisationExacteEdit->setMaxLength(120);
    localisationExacteEdit->setMinimumHeight(40);
    mainLayout->addWidget(localisationExacteEdit);

    mainLayout->addWidget(new QLabel("Délai de livraison", this));
    delaiDate = new QDateEdit(QDate::currentDate().addDays(30), this);
    delaiDate->setCalendarPopup(true);
    delaiDate->setMinimumHeight(40);
    mainLayout->addWidget(delaiDate);

    mainLayout->addWidget(new QLabel("Qualité matière", this));
    qualiteCombo = new QComboBox(this);
    qualiteCombo->addItems({"1er choix", "2eme choix", "3eme choix"});
    qualiteCombo->setMinimumHeight(40);
    mainLayout->addWidget(qualiteCombo);

    mainLayout->addWidget(new QLabel("Statut", this));
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

    auto *btnPlus = new QPushButton("+ Plus", this);
    btnPlus->setObjectName("btnPlus");
    btnPlus->setToolTip("Ouvrir les options avancées (prix, capacité, fiabilité)");
    btnPlus->setMinimumHeight(36);
    mainLayout->addWidget(btnPlus, 0, Qt::AlignLeft);

    mainLayout->addStretch();
    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(12, 12, 12, 12);
    btnLayout->setSpacing(12);

    btnSave = new QPushButton("Mettre à jour", this);
    btnSave->setObjectName("btnSave");
    btnSave->setMinimumHeight(44);
    btnCancel = new QPushButton("Annuler", this);
    btnCancel->setObjectName("btnCancel");
    btnCancel->setMinimumHeight(44);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    outerLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &ModifierFournisseur::validerEtAccepter);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(nomEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(paysEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(regionEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(villeEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(rueEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(localisationExacteEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(telephoneEdit, &QLineEdit::textChanged, this, &ModifierFournisseur::mettreAJourValidation);
    connect(btnPlus, &QPushButton::clicked, this, &ModifierFournisseur::ouvrirOptionsPlus);

    setupStyle();
    btnSave->setEnabled(false);
}

void ModifierFournisseur::ouvrirOptionsPlus()
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
        nomEdit->setProperty("error", !nomOk);
        paysEdit->setProperty("error", !paysValide);
        regionEdit->setProperty("error", !regionValide);
        villeEdit->setProperty("error", !villeValide);
        rueEdit->setProperty("error", !rueValide);
        localisationExacteEdit->setProperty("error", !localisationValide);
        telephoneEdit->setProperty("error", !telValide);
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

        nomHintLabel->setVisible(!nomOk);
    }

    nomEdit->setToolTip(nomOk ? "" : "Le nom doit commencer par une majuscule et contenir max 20 caracteres.");
    paysEdit->setToolTip(paysValide ? "" : "Le pays est obligatoire.");
    regionEdit->setToolTip(regionValide ? "" : "La region est obligatoire.");
    villeEdit->setToolTip(villeValide ? "" : "La ville est obligatoire.");
    rueEdit->setToolTip(rueValide ? "" : "La rue est obligatoire.");
    localisationExacteEdit->setToolTip(localisationValide ? "" : "La localisation exacte est obligatoire.");
    telephoneEdit->setToolTip(telValide ? "" : "Le telephone est obligatoire (8 a 15 chiffres).");
    btnSave->setEnabled(nomOk && paysValide && regionValide && villeValide && rueValide && localisationValide && telValide);
}

void ModifierFournisseur::validerEtAccepter()
{
    m_validationEnabled = true;
    mettreAJourValidation();
    if (btnSave->isEnabled()) {
        accept();
    }
}

// Helper to fill data
void ModifierFournisseur::setInitialData(QString nom, QString type, QString tel,
                                         QString addr, QDate delai, QString qualite,
                                         QString statut, double prixUnitaire,
                                         int capaciteMax, double tauxFiabilite) {
    nomEdit->setText(nom);

    // Set combobox to matching type
    int typeIndex = typeCombo->findText(type);
    if (typeIndex >= 0) {
        typeCombo->setCurrentIndex(typeIndex);
    }

    telephoneEdit->setText(tel);
    const QStringList parts = addr.split("-", Qt::SkipEmptyParts);
    if (parts.size() >= 5) {
        paysEdit->setText(parts.at(0).trimmed());
        regionEdit->setText(parts.at(1).trimmed());
        villeEdit->setText(parts.at(2).trimmed());
        rueEdit->setText(parts.at(3).trimmed());
        localisationExacteEdit->setText(parts.at(4).trimmed());
    } else if (parts.size() >= 3) {
        paysEdit->setText(parts.at(0).trimmed());
        regionEdit->setText(parts.at(1).trimmed());
        villeEdit->setText(parts.at(2).trimmed());
    } else {
        paysEdit->setText(addr.trimmed());
    }
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

    prixUnitaireSpin->setValue(prixUnitaire);
    capaciteSpin->setValue(capaciteMax);
    fiabiliteSpin->setValue(tauxFiabilite);

    // Enable validation after initial data is set
    m_validationEnabled = true;
    mettreAJourValidation();
}

QString ModifierFournisseur::getAdresse() const
{
    return QString("%1 - %2 - %3 - %4 - %5")
        .arg(paysEdit->text().trimmed(),
             regionEdit->text().trimmed(),
             villeEdit->text().trimmed(),
             rueEdit->text().trimmed(),
             localisationExacteEdit->text().trimmed());
}
