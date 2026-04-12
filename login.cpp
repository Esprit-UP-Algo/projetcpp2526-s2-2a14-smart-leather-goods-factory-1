#include "login.h"
#include "pageemployee.h"
#include "ui_login.h"
#include "commandes.h"
#include "faceid.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QToolTip>



#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QVariant>

static bool emailValide(const QString &email)
{
    QRegularExpression rx(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return rx.match(email.trimmed()).hasMatch();
}

static bool passwordValide(const QString &password)
{
    if (password.length() < 8)
        return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (QChar c : password) {
        if (c.isUpper()) hasUpper = true;
        else if (c.isLower()) hasLower = true;
        else if (c.isDigit()) hasDigit = true;
        else if (!c.isSpace()) hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

static QString forceMotDePasse(const QString &password)
{
    if (password.isEmpty())
        return "";

    int score = 0;

    if (password.length() >= 8) score++;
    if (password.contains(QRegularExpression("[A-Z]"))) score++;
    if (password.contains(QRegularExpression("[a-z]"))) score++;
    if (password.contains(QRegularExpression("[0-9]"))) score++;
    if (password.contains(QRegularExpression(R"([^A-Za-z0-9])"))) score++;

    if (score <= 2) return "Faible";
    if (score <= 4) return "Moyen";
    return "Fort";
}



login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
    , faceId(new FaceID(this))
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);

    ui->username->setPlaceholderText("Ex: nom@email.com");
    ui->password->setPlaceholderText("Mot de passe");

    QRegularExpression allowedEmailChars(R"([A-Za-z0-9@._%+\-]*)");
    ui->username->setValidator(new QRegularExpressionValidator(allowedEmailChars, this));

    connect(ui->username, &QLineEdit::textChanged, this, [=](const QString &text){
        if (text.trimmed().isEmpty()) {
            ui->username->setStyleSheet("");
        } else if (emailValide(text)) {
            ui->username->setStyleSheet("border:2px solid green;");
        } else {
            ui->username->setStyleSheet("border:2px solid red;");
        }
    });

    connect(ui->password, &QLineEdit::textChanged, this, [=](const QString &text){
        if (text.isEmpty()) {
            ui->password->setStyleSheet("");
        } else if (text.length() >= 8) {
            ui->password->setStyleSheet("border:2px solid green;");
        } else {
            ui->password->setStyleSheet("border:2px solid red;");
        }
    });


    connect(ui->btnFaceId, &QPushButton::clicked,
            this, &login::on_btnFaceId_clicked);

    connect(faceId, &FaceID::faceRecognized, this, [=](const QString &cin){

        loadingDialog->hide();
        ui->btnFaceId->setEnabled(true);

        if (!verifierLoginFaceId(cin)) {
            QMessageBox::warning(this, "Face ID", "Utilisateur non reconnu.");
        }
    });

    connect(faceId, &FaceID::faceRejected, this, [=](){

        loadingDialog->hide();
        ui->btnFaceId->setEnabled(true);

        QMessageBox::warning(this, "Face ID", "Reconnaissance échouée.");
    });

    QSqlQuery test;

    if (test.exec("SELECT COUNT(*) FROM SMARTLEATHER.EMPLOYE")) {

        if (test.next()) {
            qDebug() << "COUNT EMPLOYE =" << test.value(0).toInt();
        }

    } else {

        qDebug() << "Erreur COUNT =" << test.lastError().text();
    }


    if (test.exec("SELECT ID_EMPLOYE, CIN, EMAIL FROM SMARTLEATHER.EMPLOYE")) {

        qDebug() << "========= TEST TABLE EMPLOYE =========";

        while (test.next()) {

            qDebug() << "ID =" << test.value(0).toInt()
            << " | CIN =" << test.value(1).toString()
            << " | EMAIL =" << test.value(2).toString();
        }

    } else {

        qDebug() << "Erreur lecture table =" << test.lastError().text();
    }

    loadingDialog = new QProgressDialog("Reconnaissance faciale en cours...", "", 0, 0, this);
    loadingDialog->setWindowTitle("Face ID");
    loadingDialog->setCancelButton(nullptr);
    loadingDialog->setMinimumDuration(0);
    loadingDialog->close();


}

login::~login()
{
    delete ui;
}




void login::on_btnFaceId_clicked()
{
    ui->btnFaceId->setEnabled(false);

    loadingDialog->show();

    faceId->startFaceRecognition();
}



bool login::verifierLoginFaceId(const QString &email) // Repurposed from CIN
{
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE, NOM FROM SMARTLEATHER.EMPLOYE WHERE UPPER(TRIM(EMAIL)) = UPPER(TRIM(:email))");
    query.bindValue(":email", email.trimmed());

    if (!query.exec()) {
        qDebug() << "SQL ERROR FaceID =" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int idEmp = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QMessageBox::information(this, "Face ID", "Face ID reconnu\nBienvenue " + nom);
        pageemployee *fen = new pageemployee(idEmp);
        fen->show();
        this->hide();
        return true;
    }
    return false;
}

bool login::verifierLogin(const QString &email, const QString &password)
{
    QSqlQuery query;

    QString sql =
        "SELECT ID_EMPLOYE, EMAIL, PASSWORD "
        "FROM SMARTLEATHER.EMPLOYE "
        "WHERE UPPER(TRIM(EMAIL)) = UPPER(TRIM(:email))";

    query.prepare(sql);
    query.bindValue(":email", email.trimmed());

    if (!query.exec()) {
        qDebug() << "SQL ERROR =" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int idEmp = query.value(0).toInt();
        QString passwordDB = query.value(2).toString();

        qDebug() << "Email login =>" << email << "| Password DB =" << passwordDB;

        if (passwordDB.trimmed() == password.trimmed()) {
            pageemployee *fen = new pageemployee(idEmp);
            fen->show();
            this->hide();
            return true;
        }
    } else {
        qDebug() << "Aucun employé trouvé avec cet email.";
    }

    return false;
}

bool login::emailExiste(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTLEATHER.EMPLOYE WHERE TRIM(UPPER(EMAIL)) = TRIM(UPPER(?))");
    query.addBindValue(email.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur SQL emailExiste =" << query.lastError().text();
        return false;  // En cas d'erreur SQL : ne pas bloquer
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "emailExiste count =" << count << "pour email =" << email;
        return count > 0;
    }

    return false;
}

bool login::idExiste(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL idExiste =" << query.lastError().text();
        return true;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

void login::on_btnLogin_clicked()
{
    QString email = ui->username->text().trimmed();
    QString password = ui->password->text().trimmed();

    // Cas Auto-Login : Les deux champs sont vides
    if (email.isEmpty() && password.isEmpty()) {
        QSqlQuery query;
        if (query.exec("SELECT ID_EMPLOYE FROM SMARTLEATHER.EMPLOYE WHERE ROWNUM = 1")) {
            if (query.next()) {
                int idEmp = query.value(0).toInt();
                pageemployee *fen = new pageemployee(idEmp);
                fen->show();
                this->hide();
                return;
            } else {
                QMessageBox::warning(this, "Auto-Login", "La table des employés est vide.\nInscrivez-vous d'abord.");
                return;
            }
        } else {
            QMessageBox::critical(this, "Erreur DB", "Impossible de lire la table EMPLOYE :\n" + query.lastError().text());
            return;
        }
    }

    // Cas Connexion Normale
    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Connexion", "Veuillez saisir votre email et mot de passe.");
        return;
    }

    if (!verifierLogin(email, password)) {
        QMessageBox::warning(this, "Échec", "Identifiants incorrects.");
    }
}


void login::on_btnSignup_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Inscription nouvel employé");
    dlg.setFixedSize(470, 820);
    dlg.setModal(true);

    dlg.setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"

        "QLabel {"
        "   color: #5a341c;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "}"

        "QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {"
        "   background-color: #fffaf5;"
        "   color: #3b2415;"
        "   border: 2px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}"

        "QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {"
        "   border: 2px solid #8b5a2b;"
        "   background-color: #ffffff;"
        "}"

        "QPushButton {"
        "   border-radius: 12px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-size: 15px;"
        "   border: 3px solid #6b3e1a;"
        "}"

        "QPushButton[text='Enregistrer'] {"
        "   background-color: #6f8f3d;"
        "   color: white;"
        "}"

        "QPushButton[text='Enregistrer']:hover {"
        "   background-color: #84a94a;"
        "}"

        "QPushButton[text='Annuler'] {"
        "   background-color: #d8c7b8;"
        "   color: #5a341c;"
        "}"

        "QPushButton[text='Annuler']:hover {"
        "   background-color: #cab5a3;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(&dlg);
    QFormLayout *formLayout = new QFormLayout();

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom");

    QLineEdit *prenomEdit = new QLineEdit();
    prenomEdit->setPlaceholderText("Prénom");

    QLineEdit *cinEdit = new QLineEdit();
    cinEdit->setPlaceholderText("Ex: 12345678");
    cinEdit->setMaxLength(8);
    cinEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,8}$"), cinEdit));

    QDateEdit *dateNaissanceEdit = new QDateEdit();
    dateNaissanceEdit->setCalendarPopup(true);
    dateNaissanceEdit->setDisplayFormat("dd/MM/yyyy");
    dateNaissanceEdit->setDate(QDate::currentDate());

    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({
        "Responsable Stock",
        "Service Achat",
        "Service Technique",
        "Service Client",
        "Commercial",
        "Comptable",
        "Directeur"
    });

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({
        "BAC",
        "LICENCE",
        "MASTER"
    });

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({
        "Actif",
        "En congé",
        "Suspendu",
        "En formation",
        "Maladie",
        "Mission",
        "Démissionné"
    });

    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setValue(1500.0);
    salaireSpin->setDecimals(2);
    salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Ex: nom@email.com");
    emailEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression(R"([A-Za-z0-9@._%+\-]*)"), emailEdit));

    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("8 caractères min, Maj, min, chiffre, spécial");

    QLabel *cinHint = new QLabel("");
    QLabel *emailHint = new QLabel("");
    QLabel *passwordHint = new QLabel("");
    QLabel *passwordStrength = new QLabel("");

    cinHint->setStyleSheet("color:red; font-size:11px;");
    emailHint->setStyleSheet("color:red; font-size:11px;");
    passwordHint->setStyleSheet("color:red; font-size:11px;");
    passwordStrength->setStyleSheet("color:#5a341c; font-size:11px;");

    formLayout->addRow("Nom :", nomEdit);
    formLayout->addRow("Prénom :", prenomEdit);

    {
        QWidget *w = new QWidget();
        QVBoxLayout *lay = new QVBoxLayout(w);
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(2);
        lay->addWidget(cinEdit);
        lay->addWidget(cinHint);
        formLayout->addRow("CIN :", w);
    }

    formLayout->addRow("Date naissance :", dateNaissanceEdit);
    formLayout->addRow("Poste :", posteCombo);
    formLayout->addRow("Niveau :", niveauCombo);
    formLayout->addRow("Statut :", statusCombo);
    formLayout->addRow("Salaire :", salaireSpin);

    {
        QWidget *w = new QWidget();
        QVBoxLayout *lay = new QVBoxLayout(w);
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(2);
        lay->addWidget(emailEdit);
        lay->addWidget(emailHint);
        formLayout->addRow("Email :", w);
    }

    {
        QWidget *w = new QWidget();
        QVBoxLayout *lay = new QVBoxLayout(w);
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(2);
        lay->addWidget(passwordEdit);
        lay->addWidget(passwordHint);
        lay->addWidget(passwordStrength);
        formLayout->addRow("Mot de passe :", w);
    }

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setText("Enregistrer");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("Annuler");
    mainLayout->addWidget(buttonBox);

    connect(cinEdit, &QLineEdit::textChanged, [&]() {
        QString cin = cinEdit->text().trimmed();

        if (cin.isEmpty()) {
            cinEdit->setStyleSheet("");
            cinHint->setText("");
        } else if (cin.length() == 8) {
            cinEdit->setStyleSheet("border:2px solid green;");
            cinHint->setStyleSheet("color:green; font-size:11px;");
            cinHint->setText("CIN valide");
        } else {
            cinEdit->setStyleSheet("border:2px solid red;");
            cinHint->setStyleSheet("color:red; font-size:11px;");
            cinHint->setText("Le CIN doit contenir exactement 8 chiffres");
        }
    });

    connect(emailEdit, &QLineEdit::textChanged, [&]() {
        QString email = emailEdit->text().trimmed();

        if (email.isEmpty()) {
            emailEdit->setStyleSheet("");
            emailHint->setText("");
        } else if (emailValide(email)) {
            emailEdit->setStyleSheet("border:2px solid green;");
            emailHint->setStyleSheet("color:green; font-size:11px;");
            emailHint->setText("Email valide");
        } else {
            emailEdit->setStyleSheet("border:2px solid red;");
            emailHint->setStyleSheet("color:red; font-size:11px;");
            emailHint->setText("Format email invalide");
        }
    });

    connect(passwordEdit, &QLineEdit::textChanged, [&]() {
        QString mdp = passwordEdit->text();
        QString force = forceMotDePasse(mdp);

        if (mdp.isEmpty()) {
            passwordEdit->setStyleSheet("");
            passwordHint->setText("");
            passwordStrength->setText("");
            return;
        }

        if (passwordValide(mdp)) {
            passwordEdit->setStyleSheet("border:2px solid green;");
            passwordHint->setStyleSheet("color:green; font-size:11px;");
            passwordHint->setText("Mot de passe valide");
        } else {
            passwordEdit->setStyleSheet("border:2px solid red;");
            passwordHint->setStyleSheet("color:red; font-size:11px;");
            passwordHint->setText("8 caractères min + majuscule + minuscule + chiffre + spécial");
        }

        if (force == "Faible")
            passwordStrength->setStyleSheet("color:red; font-size:11px;");
        else if (force == "Moyen")
            passwordStrength->setStyleSheet("color:orange; font-size:11px;");
        else
            passwordStrength->setStyleSheet("color:green; font-size:11px;");

        passwordStrength->setText("Force : " + force);
    });

        connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        QString nom = nomEdit->text().trimmed();
        QString prenom = prenomEdit->text().trimmed();
        QString cin = cinEdit->text().trimmed();
        QDate dateNaiss = dateNaissanceEdit->date();
        QString email = emailEdit->text().trimmed();
        QString password = passwordEdit->text().trimmed();
        QString role = posteCombo->currentText().trimmed();
        QString niveau = niveauCombo->currentText().trimmed();
        QString statut = statusCombo->currentText().trimmed();
        double salaire = salaireSpin->value();

        if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || password.isEmpty() || cin.isEmpty()) {
            QMessageBox::warning(&dlg, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
            return; // Garder la popup ouverte
        }

        if (emailExiste(email)) {
            QMessageBox::warning(&dlg, "Erreur", "Cet email existe déjà.");
            return; // Garder la popup ouverte
        }

        QSqlQuery query;
        query.prepare(
            "INSERT INTO SMARTLEATHER.EMPLOYE "
            "(ID_EMPLOYE, NOM, PRENOM, EMAIL, PASSWORD, POSTE, STATUS, NIVEAU, SALAIRE, CIN, DATE_NAISSANCE) "
            "VALUES (SMARTLEATHER.SEQ_EMPLOYE.NEXTVAL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
            );

        query.addBindValue(nom);
        query.addBindValue(prenom);
        query.addBindValue(email);
        query.addBindValue(password);
        query.addBindValue(role);
        query.addBindValue(statut);
        query.addBindValue(niveau);
        query.addBindValue(salaire);
        query.addBindValue(cin);
        query.addBindValue(dateNaiss);

        if (!query.exec()) {
            QMessageBox::critical(&dlg, "Erreur SQL", "Échec de l'inscription :\n" + query.lastError().text());
            return;
        }

        QMessageBox::information(&dlg, "Succès", "Inscription réussie !");
        dlg.accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    dlg.exec();
}

bool login::cinExiste(const QString &cin)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SMARTLEATHER.EMPLOYE WHERE TRIM(CIN) = TRIM(?)");
    query.addBindValue(cin.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur SQL cinExiste =" << query.lastError().text();
        return false;  // En cas d'erreur SQL : ne pas bloquer l'inscription
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "cinExiste count =" << count << "pour CIN =" << cin;
        return count > 0;
    }

    return false;
}

int login::getIdEmployeByEmail(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE FROM SMARTLEATHER.EMPLOYE WHERE TRIM(UPPER(EMAIL)) = TRIM(UPPER(?))");
    query.addBindValue(email.trimmed());

    if (!query.exec()) {
        qDebug() << "Erreur getIdEmployeByEmail =" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}
