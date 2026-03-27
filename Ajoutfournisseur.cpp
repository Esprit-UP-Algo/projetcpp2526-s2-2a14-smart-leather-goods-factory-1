#include "ajoutfournisseur.h"

#include <QHBoxLayout>

AjoutFournisseur::AjoutFournisseur(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Ajouter un fournisseur");
    setModal(true);
    resize(400, 560);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ID fournisseur
    mainLayout->addWidget(new QLabel("ID fournisseur"));
    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("Auto");
    idEdit->setReadOnly(true);
    mainLayout->addWidget(idEdit);

    // Nom fournisseur
    mainLayout->addWidget(new QLabel("Nom du fournisseur"));
    nomEdit = new QLineEdit(this);
    mainLayout->addWidget(nomEdit);

    // Type matière
    mainLayout->addWidget(new QLabel("Type de matière"));
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Bois", "Métal", "Plastique", "Verre"});
    mainLayout->addWidget(typeCombo);

    // Téléphone
    mainLayout->addWidget(new QLabel("Téléphone"));
    telephoneEdit = new QLineEdit(this);
    mainLayout->addWidget(telephoneEdit);

    // Adresse
    mainLayout->addWidget(new QLabel("Adresse"));
    adresseEdit = new QLineEdit(this);
    mainLayout->addWidget(adresseEdit);

    // Délai de livraison
    mainLayout->addWidget(new QLabel("Délai de livraison"));
    delaiDate = new QDateEdit(QDate::currentDate(), this);
    delaiDate->setCalendarPopup(true);
    mainLayout->addWidget(delaiDate);

    // Qualité matière
    mainLayout->addWidget(new QLabel("Qualité matière"));
    qualiteEdit = new QLineEdit(this);
    mainLayout->addWidget(qualiteEdit);

    // Statut
    mainLayout->addWidget(new QLabel("Statut"));
    statutCombo = new QComboBox(this);
    statutCombo->addItems({"Actif", "Inactif"});
    mainLayout->addWidget(statutCombo);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Enregistrer", this);
    btnCancel = new QPushButton("Annuler", this);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // Connexions
    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}
