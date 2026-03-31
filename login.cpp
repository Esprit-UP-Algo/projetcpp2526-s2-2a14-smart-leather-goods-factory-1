#include "login.h"
#include "ui_login.h"
#include "commandes.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_btnSignup_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Inscription nouvel employé");
    dlg.setFixedSize(450, 550);
    dlg.setModal(true);
    dlg.setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel {"
        "   color: #3a2a20;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}"
        "QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {"
        "   border: 2px solid #6b3e26;"
        "}"
        "QPushButton {"
        "   border-radius: 10px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "}"
        "QPushButton[text='Enregistrer'] {"
        "   background-color: #6f8f3d;"
        "}"
        "QPushButton[text='Enregistrer']:hover {"
        "   background-color: #8ab048;"
        "}"
        "QPushButton[text='Annuler'] {"
        "   background-color: #b3a398;"
        "   color: #3a2a20;"
        "}"
        "QPushButton[text='Annuler']:hover {"
        "   background-color: #c4b5a8;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(&dlg);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);
    QLabel *header = new QLabel("📝 INSCRIPTION NOUVEL EMPLOYÉ");
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #6b3e26; margin-bottom: 10px;");
    mainLayout->addWidget(header);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(10);
    formLayout->setLabelAlignment(Qt::AlignRight);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ex: E007");
    formLayout->addRow("🔑 ID Employé :", idEdit);

    QLineEdit *nomEdit = new QLineEdit();
    formLayout->addRow("👤 Nom :", nomEdit);

    QLineEdit *prenomEdit = new QLineEdit();
    formLayout->addRow("👤 Prénom :", prenomEdit);

    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur", "Responsable Production", "Opérateur", "Comptable", "Commercial", "Magasinier", "Stagiaire"});
    formLayout->addRow("💼 Poste :", posteCombo);

    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setValue(1500.00);
    salaireSpin->setSuffix(" €");
    formLayout->addRow("💰 Salaire :", salaireSpin);

    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    formLayout->addRow("📅 Date embauche :", dateEdit);

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Congé", "Formation", "Inactif"});
    formLayout->addRow("⚡ Statut :", statutCombo);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("✅ Enregistrer");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("❌ Annuler");
    buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet("background-color: #6f8f3d; color: white;");
    buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet("background-color: #b3a398; color: #3a2a20;");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        if (idEdit->text().isEmpty() || nomEdit->text().isEmpty() || prenomEdit->text().isEmpty()) {
            QMessageBox::warning(&dlg, "Champs manquants", "Veuillez remplir tous les champs.");
            return;
        }
        dlg.accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString id = idEdit->text();
        QString nom = nomEdit->text();
        QString prenom = prenomEdit->text();
        QString poste = posteCombo->currentText();
        double salaire = salaireSpin->value();
        QDate date = dateEdit->date();
        QString statut = statutCombo->currentText();
        QMessageBox::information(this, "Succès", "Inscription réussie !");
    }
}

void login::on_btnLogin_clicked()
{
    hide();
    commandes *lg = new commandes(this);
    lg->show();
}
