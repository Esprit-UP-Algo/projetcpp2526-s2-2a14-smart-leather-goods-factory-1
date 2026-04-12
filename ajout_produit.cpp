#include "ajout_produit.h"

AjoutProduit::AjoutProduit(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Nouveau produit");
  setFixedSize(450, 700);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(30, 30, 30, 30);
  mainLayout->setSpacing(12);

  // Heading
  QLabel *header = new QLabel("DÉTAILS PRODUIT");
  header->setObjectName("headerLabel");
  mainLayout->addWidget(header);

  // Form Fields
  idEdit = new QLineEdit();
  idEdit->setPlaceholderText("ID Produit");
  idEdit->setAlignment(Qt::AlignCenter);

  referenceEdit = new QLineEdit();
  referenceEdit->setPlaceholderText("Référence");
  referenceEdit->setAlignment(Qt::AlignCenter);

  categorieEdit = new QLineEdit();
  categorieEdit->setPlaceholderText("Catégorie");
  categorieEdit->setAlignment(Qt::AlignCenter);

  prixEdit = new QLineEdit();
  prixEdit->setPlaceholderText("Prix unitaire");
  prixEdit->setAlignment(Qt::AlignCenter);

  quantiteSpinBox = new QSpinBox();
  quantiteSpinBox->setMaximum(100000);
  quantiteSpinBox->setMinimum(0);
  quantiteSpinBox->setAlignment(Qt::AlignCenter);

  statusCombo = new QComboBox();
  statusCombo->addItems(
      {"En approvisionnement", "En stock", "Rupture de stock"});

  dateCreationEdit = new QDateTimeEdit(QDateTime::currentDateTime());
  dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
  dateCreationEdit->setCalendarPopup(true);

  // Add to layout
  mainLayout->addWidget(new QLabel("Identifiant Produit :"));
  mainLayout->addWidget(idEdit);
  mainLayout->addWidget(new QLabel("Référence :"));
  mainLayout->addWidget(referenceEdit);
  mainLayout->addWidget(new QLabel("Catégorie :"));
  mainLayout->addWidget(categorieEdit);
  mainLayout->addWidget(new QLabel("Prix unitaire (DT) :"));
  mainLayout->addWidget(prixEdit);
  mainLayout->addWidget(new QLabel("Quantité en stock :"));
  mainLayout->addWidget(quantiteSpinBox);
  mainLayout->addWidget(new QLabel("Statut :"));
  mainLayout->addWidget(statusCombo);
  mainLayout->addWidget(new QLabel("Date de création :"));
  mainLayout->addWidget(dateCreationEdit);

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
  connect(btnSave, &QPushButton::clicked, this, &AjoutProduit::accept);
  connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

  setupStyle();
}

void AjoutProduit::accept() {
  if (idEdit->text().trimmed().isEmpty() ||
      referenceEdit->text().trimmed().isEmpty() ||
      categorieEdit->text().trimmed().isEmpty()) {
    QMessageBox::warning(this, "Erreur de saisie",
                         "Veuillez remplir tous les champs obligatoires (ID, "
                         "Référence, Catégorie).");
    return;
  }

  bool ok;
  float prix = prixEdit->text().toFloat(&ok);
  if (!ok || prix <= 0) {
    QMessageBox::warning(this, "Erreur de saisie",
                         "Le prix doit être un nombre valide supérieur à 0.");
    return;
  }

  if (quantiteSpinBox->value() < 0) {
    QMessageBox::warning(this, "Erreur de saisie",
                         "La quantité en stock ne peut pas être négative.");
    return;
  }

  QDialog::accept();
}

void AjoutProduit::setupStyle() {
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
      "QLineEdit, QDateTimeEdit, QComboBox, QSpinBox {"
      "   background-color: #fffaf5;"
      "   border: 1px solid #c9b2a2;"
      "   border-radius: 10px;"
      "   padding: 8px;"
      "   color: #3a2a20;"
      "}"
      "QLineEdit:focus, QSpinBox:focus { border: 2px solid #6b3e26; }"
      "QPushButton {"
      "   border-radius: 10px; padding: 10px; font-weight: bold; color: white;"
      "   border-bottom: 3px solid rgba(0,0,0,0.2);"
      "}"
      "QPushButton#btnSave { background-color: #6f8f3d; }"
      "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
      "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid "
      "rgba(0,0,0,0.2); }");
}
