#include "pageemployee.h"
#include "ui_pageemployee.h"

#include <QtSql>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QMap>
#include <QApplication>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDate>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTextEdit>
#include <functional>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>


#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>

#include "commandes.h"
#include "login.h"
#include "fournisseurs.h"
#include "produit.h"
#include "produitswindow.h"
#include "matieres.h"
#include "pagemachine.h"
#include "pagechat.h"

static QString currentRoleForUser(int idEmploye)
{
    QSqlQuery q;
    q.prepare("SELECT POSTE FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", idEmploye);
    if (!q.exec() || !q.next()) return QString();
    return q.value(0).toString().trimmed();
}

static bool denyIfRoleMismatch(QWidget *parent, int idEmploye, const QString &targetRole)
{
    const QString role = currentRoleForUser(idEmploye);
    if (role.compare(targetRole, Qt::CaseInsensitive) == 0) return false;
    QMessageBox::warning(parent, "Accès refusé",
                         "Vous n'avez pas accès à cette page.");
    return true;
}

pageemployee::pageemployee(int idEmployeConnecte, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemployee)
    , m_idEmployeConnecte(idEmployeConnecte)
{
    ui->setupUi(this);
    if (ui->groupBox) ui->groupBox->hide();

    // Premium Sidebar Setup (same visual style as page machine)
    QString navBtnStyle =
        "QPushButton {"
        "  background: transparent; border: none; color: #c9b8a5;"
        "  text-align: left; padding-left: 20px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 255, 255, 0.1); color: white; border-left: 4px solid #c9a87c;"
        "}";

    QWidget *sidebar = new QWidget(this);
    sidebar->setGeometry(0, 0, 240, 750);
    sidebar->setStyleSheet("background-color: #3a1f14;");

    QLabel *logoLab = new QLabel(sidebar);
    logoLab->setGeometry(20, 10, 211, 121);
    logoLab->setPixmap(QPixmap(":/Logo.png"));
    logoLab->setScaledContents(true);
    logoLab->show();
    logoLab->raise();

    QVBoxLayout *navLayout = new QVBoxLayout(sidebar);
    navLayout->setContentsMargins(0, 160, 0, 20);
    navLayout->setSpacing(5);

    auto addNavBtn = [&](const QString &txt, const std::function<void()> &handler, bool active = false) {
        QPushButton *btn = new QPushButton("  " + txt);
        btn->setMinimumHeight(45);
        if (active) {
            btn->setStyleSheet(navBtnStyle + "QPushButton { background-color: rgba(255,255,255,0.1); color:white; border-left:4px solid #c9a87c; }");
        } else {
            btn->setStyleSheet(navBtnStyle);
            if (handler) {
                connect(btn, &QPushButton::clicked, this, handler);
            }
        }
        navLayout->addWidget(btn);
        return btn;
    };

    addNavBtn("Employés", {}, true);
    addNavBtn("Produits", [this]() { on_pushButton_21_clicked(); });
    addNavBtn("Commandes", [this]() { on_pushButton_6_clicked(); });
    addNavBtn("Fournisseurs", [this]() { on_pushButton_20_clicked(); });
    addNavBtn("Matières", [this]() { on_pushButton_22_clicked(); });
    addNavBtn("Machines", [this]() { on_pushButton_23_clicked(); });

    navLayout->addSpacing(30);
    
    // Professional Separator
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: rgba(255,255,255,0.1); max-height: 1px; margin: 10px 20px;");
    navLayout->addWidget(line);

    QPushButton *logoutBtnSide = addNavBtn("Déconnexion", [this]() { on_pushButton_8_clicked(); });
    logoutBtnSide->setStyleSheet(navBtnStyle + 
        "QPushButton:hover { background-color: rgba(220, 53, 69, 0.2); color: #ff9999; border-left: 4px solid #cc3333; }"
    );

    sidebar->raise();
    sidebar->show();

    // 🔥 Connexion Arduino

    setupTable();
    loadEmployeesTable();
    qDebug() << ">>> avant verifierBadgeRFID";
    qDebug() << ">>> apres verifierBadgeRFID";
    connect(ui->searchIdEdit,  &QLineEdit::textChanged, this, &pageemployee::applyFilter);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pageemployee::applyFilter);

    connect(logoutBtnSide, &QPushButton::clicked, this, &pageemployee::on_pushButton_8_clicked);
}

pageemployee::~pageemployee()
{
    delete ui;
}


QString pageemployee::getCinEmployeConnecte() const
{
    QSqlQuery q;
    q.prepare("SELECT CIN FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", m_idEmployeConnecte);

    if (!q.exec()) {
        qDebug() << "Erreur récupération CIN =" << q.lastError().text();
        return QString();
    }

    if (q.next()) {
        return q.value(0).toString().trimmed();
    }

    return QString();
}

QString pageemployee::displayNameEmployeConnecte() const
{
    QSqlQuery q;
    q.prepare("SELECT NOM, PRENOM FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", m_idEmployeConnecte);

    if (!q.exec()) {
        qDebug() << "displayNameEmployeConnecte error =" << q.lastError().text();
        return QStringLiteral("Employe %1").arg(m_idEmployeConnecte);
    }

    if (!q.next())
        return QStringLiteral("Employe %1").arg(m_idEmployeConnecte);

    const QString nom = q.value(0).toString().trimmed();
    const QString prenom = q.value(1).toString().trimmed();
    QString disp = (prenom + QLatin1Char(' ') + nom).trimmed();
    if (disp.isEmpty())
        disp = QStringLiteral("Employe %1").arg(m_idEmployeConnecte);
    return disp;
}


void pageemployee::on_btnSaveFacePhoto_clicked()
{
    QString cin = getCinEmployeConnecte();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Face ID", "Impossible de récupérer le CIN de l'employé connecté.");
        return;
    }

    QString saveDir = "C:/Users/AZERTY/Desktop/qt/database_faces";
    QDir().mkpath(saveDir);

    QString scriptPath = "C:/Users/AZERTY/Desktop/qt/capture_face.py";

    QProcess *proc = new QProcess(this);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");

    proc->setProcessEnvironment(env);

    QString program = "C:/Users/AZERTY/Desktop/qt/venv311/Scripts/python.exe";
    QStringList arguments;
    arguments << scriptPath << cin;

    QMessageBox::information(this, "Face ID", "La caméra va s'ouvrir.\nAppuyez sur S pour enregistrer la photo.");

    proc->start(program, arguments);

    if (!proc->waitForStarted(3000)) {
        QMessageBox::critical(this, "Face ID", "Impossible de lancer la capture photo.");
        proc->deleteLater();
        return;
    }

    connect(proc, &QProcess::readyReadStandardOutput, this, [=]() {
        QString output = QString::fromUtf8(proc->readAllStandardOutput()).trimmed();
        qDebug() << "capture_face.py output =" << output;

        if (output.contains("PHOTO_SAVED:")) {
            QMessageBox::information(this, "Face ID", "Photo enregistrée avec succès.");
        }
        else if (output.contains("CAMERA_ERROR")) {
            QMessageBox::warning(this, "Face ID", "Caméra non disponible.");
        }
        else if (output.contains("CAPTURE_CANCELLED")) {
            QMessageBox::warning(this, "Face ID", "Capture annulée.");
        }
    });

    connect(proc,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,
            [=](int, QProcess::ExitStatus) {
                proc->deleteLater();
            });
}

/* =========================
   Setup tableWidget
========================= */
void pageemployee::setupTable()
{
    // On garde ID_EMPLOYE en colonne 0 (cachée)
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels({
        "ID",
        "Nom",
        "Prénom",
        "CIN",
        "Date naissance",
        "Poste",
        "Niveau",
        "Status",
        "Salaire",
        "Email",
        "RFID UID"
    });

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // Cacher l'ID technique
    ui->tableWidget->setColumnHidden(0, true);
}

/* =========================
   Get selected ID
========================= */
int pageemployee::currentEmployeeId() const
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) return -1;

    QTableWidgetItem *it = ui->tableWidget->item(row, 0); // colonne cachée ID
    if (!it) return -1;

    bool ok = false;
    int id = it->text().toInt(&ok);
    return ok ? id : -1;
}

/* =========================
   Load data from DB → tableWidget
========================= */
void pageemployee::loadEmployeesTable(const QString &whereClause, const QVariantList &binds)
{
    ui->tableWidget->setRowCount(0);

    QString sql =
        "SELECT ID_EMPLOYE, NOM, PRENOM, CIN, DATE_NAISSANCE, POSTE, NIVEAU, STATUS, SALAIRE, EMAIL, RFID_UID "
        "FROM SMARTLEATHER.EMPLOYE";

    if (!whereClause.trimmed().isEmpty())
        sql += " WHERE " + whereClause;

    sql += " ORDER BY NOM, PRENOM";

    QSqlQuery q;
    q.prepare(sql);

    for (int i = 0; i < binds.size(); ++i)
        q.bindValue(i, binds[i]);

    if (!q.exec()) {
        QMessageBox::critical(this, "DB", q.lastError().text());
        return;
    }

    int r = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(r);

        ui->tableWidget->setItem(r, 0, new QTableWidgetItem(q.value(0).toString())); // ID
        ui->tableWidget->setItem(r, 1, new QTableWidgetItem(q.value(1).toString())); // NOM
        ui->tableWidget->setItem(r, 2, new QTableWidgetItem(q.value(2).toString())); // PRENOM
        ui->tableWidget->setItem(r, 3, new QTableWidgetItem(q.value(3).toString())); // CIN

        QString dateStr;
        if (q.value(4).metaType().id() == QMetaType::QDate)
            dateStr = q.value(4).toDate().toString("dd/MM/yyyy");
        else {
            QString raw = q.value(4).toString();
            if (raw.contains('T'))
                dateStr = QDate::fromString(raw.left(10), "yyyy-MM-dd").toString("dd/MM/yyyy");
            else
                dateStr = raw;
        }

        ui->tableWidget->setItem(r, 4, new QTableWidgetItem(dateStr)); // DATE
        ui->tableWidget->setItem(r, 5, new QTableWidgetItem(q.value(5).toString())); // POSTE
        ui->tableWidget->setItem(r, 6, new QTableWidgetItem(q.value(6).toString())); // NIVEAU
        ui->tableWidget->setItem(r, 7, new QTableWidgetItem(q.value(7).toString())); // STATUS
        ui->tableWidget->setItem(r, 8, new QTableWidgetItem(QString::number(q.value(8).toDouble(), 'f', 2))); // SALAIRE
        ui->tableWidget->setItem(r, 9, new QTableWidgetItem(q.value(9).toString())); // EMAIL
        ui->tableWidget->setItem(r, 10, new QTableWidgetItem(q.value(10).toString())); // RFID_UID

        r++;
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setColumnHidden(0, true);
}

/* =========================
   Filter (DB)
========================= */
void pageemployee::applyFilter()
{
    QString cinTxt = ui->searchIdEdit->text().trimmed();
    QString nomTxt = ui->searchNomEdit->text().trimmed();

    QStringList cond;
    QVariantList binds;

    if (!cinTxt.isEmpty()) {
        for (const QChar &c : cinTxt) {
            if (!c.isDigit()) {
                loadEmployeesTable("1=0");
                return;
            }
        }

        cond << "CIN LIKE ?";
        binds << ("%" + cinTxt + "%");
    }

    if (!nomTxt.isEmpty()) {
        cond << "UPPER(NOM) LIKE UPPER(?)";
        binds << ("%" + nomTxt + "%");
    }

    if (cond.isEmpty())
        loadEmployeesTable();
    else
        loadEmployeesTable(cond.join(" AND "), binds);
}

void pageemployee::on_search_clicked()
{
    applyFilter();
}

void pageemployee::on_actualiser_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNomEdit->clear();
    loadEmployeesTable();
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

/* =========================
   Ajouter
========================= */
void pageemployee::on_pushButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Ajouter un employé");
    dlg.setFixedSize(550, 720);

    QVBoxLayout *main = new QVBoxLayout(&dlg);

    QLineEdit *nomEdit = new QLineEdit();
    QLineEdit *prenomEdit = new QLineEdit();
    QLineEdit *cinEdit = new QLineEdit();
    cinEdit->setMaxLength(8);
    cinEdit->setPlaceholderText("Ex: 12345678");

    QDateEdit *dateNaissEdit = new QDateEdit(QDate::currentDate());
    dateNaissEdit->setDisplayFormat("dd/MM/yyyy");
    dateNaissEdit->setCalendarPopup(true);

    // ===== ComboBox au lieu de QLineEdit =====
    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({
        "PRODUITS",
        "FOURNISSEURS",
        "MACHINES",
        "COMMANDES",
        "MATIERES",
        "EMPLOYE"
    });

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({"BAC", "LICENCE", "MASTER"});

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({
        "ACTIF",
        "EN CONGE",
        "SUSPENDU",
        "EN FORMATION",
        "MALADIE",
        "MISSION",
        "DEMISSIONNE"
    });

    // ===== Salaire en spinbox =====
    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setDecimals(2);
    salaireSpin->setValue(1500.0);
    salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit();

    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Mot de passe (8 car, min, maj, num, spec)");

    QLineEdit *rfidEdit = new QLineEdit();
    rfidEdit->setPlaceholderText("Scannez le badge ou saisissez l'UID");

    connect(passwordEdit, &QLineEdit::textChanged, [&]() {
        QString mdp = passwordEdit->text();
        if (mdp.isEmpty()) {
            passwordEdit->setStyleSheet("");
        } else if (passwordValide(mdp)) {
            passwordEdit->setStyleSheet("border:2px solid green;");
        } else {
            passwordEdit->setStyleSheet("border:2px solid red;");
        }
    });

    main->addWidget(new QLabel("Nom:"));             main->addWidget(nomEdit);
    main->addWidget(new QLabel("Prénom:"));          main->addWidget(prenomEdit);
    main->addWidget(new QLabel("CIN:"));             main->addWidget(cinEdit);
    main->addWidget(new QLabel("Date naissance:"));  main->addWidget(dateNaissEdit);
    main->addWidget(new QLabel("Poste:"));           main->addWidget(posteCombo);
    main->addWidget(new QLabel("Niveau:"));          main->addWidget(niveauCombo);
    main->addWidget(new QLabel("Status:"));          main->addWidget(statusCombo);
    main->addWidget(new QLabel("Salaire:"));         main->addWidget(salaireSpin);
    main->addWidget(new QLabel("Email:"));           main->addWidget(emailEdit);
    main->addWidget(new QLabel("Mot de passe:"));    main->addWidget(passwordEdit);
    main->addWidget(new QLabel("RFID UID:"));        main->addWidget(rfidEdit);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *save = new QPushButton("Enregistrer");
    QPushButton *cancel = new QPushButton("Annuler");
    btns->addWidget(save);
    btns->addWidget(cancel);
    main->addLayout(btns);

    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    connect(save, &QPushButton::clicked, [&]() {
        QString nom = nomEdit->text().trimmed();
        QString prenom = prenomEdit->text().trimmed();
        QString cin = cinEdit->text().trimmed();
        QString poste = posteCombo->currentText().trimmed();
        QString niveau = niveauCombo->currentText().trimmed();
        QString status = statusCombo->currentText().trimmed();
        QString email = emailEdit->text().trimmed();
        QString password = passwordEdit->text();
        QString rfid = rfidEdit->text().trimmed();
        double sal = salaireSpin->value();

        if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "Nom, Prénom, CIN et Email sont obligatoires.");
            return;
        }

        if (cin.length() != 8) {
            QMessageBox::warning(&dlg, "Validation", "Le CIN doit contenir exactement 8 chiffres.");
            return;
        }

        for (const QChar &c : cin) {
            if (!c.isDigit()) {
                QMessageBox::warning(&dlg, "Validation", "Le CIN doit contenir uniquement des chiffres.");
                return;
            }
        }

        if (!passwordValide(password)) {
            QMessageBox::warning(&dlg, "Validation", "Le mot de passe doit contenir au moins 8 caractères (majuscule, minuscule, chiffre, spécial).");
            return;
        }

        if (!email.contains('@') || !email.contains('.')) {
            QMessageBox::warning(&dlg, "Validation", "Email invalide.");
            return;
        }

        if (dateNaissEdit->date() > QDate::currentDate()) {
            QMessageBox::warning(&dlg, "Validation", "Date de naissance invalide.");
            return;
        }

        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM SMARTLEATHER.EMPLOYE "
                      "WHERE TRIM(CIN)=TRIM(:cin) "
                      "OR LOWER(TRIM(EMAIL))=LOWER(TRIM(:email))");
        check.bindValue(":cin", cin);
        check.bindValue(":email", email);

        if (!check.exec()) {
            QMessageBox::critical(&dlg, "DB", check.lastError().text());
            return;
        }

        if (check.next() && check.value(0).toInt() > 0) {
            QMessageBox::warning(&dlg, "Validation", "CIN ou Email existe déjà.");
            return;
        }

        QSqlQuery q;
        q.prepare(
            "INSERT INTO SMARTLEATHER.EMPLOYE "
            "(NOM, PRENOM, CIN, DATE_NAISSANCE, POSTE, NIVEAU, STATUS, SALAIRE, EMAIL, PASSWORD, RFID_UID) "
            "VALUES (:nom, :prenom, :cin, :dn, :poste, :niveau, :status, :sal, :email, :password, :rfid)"
            );
        q.bindValue(":nom", nom);
        q.bindValue(":prenom", prenom);
        q.bindValue(":cin", cin);
        q.bindValue(":dn", dateNaissEdit->date());
        q.bindValue(":poste", poste);
        q.bindValue(":niveau", niveau);
        q.bindValue(":status", status);
        q.bindValue(":sal", sal);
        q.bindValue(":email", email);
        q.bindValue(":password", password);
        q.bindValue(":rfid", rfid);

        if (!q.exec()) {
            QMessageBox::critical(&dlg, "DB Insert", q.lastError().text());
            return;
        }

        QMessageBox::information(&dlg, "Succès", "Employé ajouté avec succès.");
        dlg.accept();
        applyFilter();
    });

    dlg.exec();
}


/* =========================
   Modifier
========================= */
void pageemployee::on_pushButton_2_clicked()
{
    int id = currentEmployeeId();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection", "Sélectionnez un employé à modifier.");
        return;
    }

    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Sélection", "Aucune ligne sélectionnée.");
        return;
    }

    QString nom    = ui->tableWidget->item(row, 1) ? ui->tableWidget->item(row, 1)->text() : "";
    QString prenom = ui->tableWidget->item(row, 2) ? ui->tableWidget->item(row, 2)->text() : "";
    QString cin    = ui->tableWidget->item(row, 3) ? ui->tableWidget->item(row, 3)->text() : "";
    QDate dn       = QDate::fromString(ui->tableWidget->item(row, 4) ? ui->tableWidget->item(row, 4)->text() : "", "dd/MM/yyyy");
    QString poste  = ui->tableWidget->item(row, 5) ? ui->tableWidget->item(row, 5)->text().trimmed() : "";
    QString niveau = ui->tableWidget->item(row, 6) ? ui->tableWidget->item(row, 6)->text().trimmed() : "";
    QString status = ui->tableWidget->item(row, 7) ? ui->tableWidget->item(row, 7)->text().trimmed() : "";
    double salaire = ui->tableWidget->item(row, 8) ? ui->tableWidget->item(row, 8)->text().replace("DT","").trimmed().toDouble() : 0.0;
    QString email  = ui->tableWidget->item(row, 9) ? ui->tableWidget->item(row, 9)->text() : "";
    QString rfid   = ui->tableWidget->item(row, 10) ? ui->tableWidget->item(row, 10)->text() : "";

    QDialog dlg(this);
    dlg.setWindowTitle("Modifier employé");
    dlg.setFixedSize(550, 720);

    QVBoxLayout *main = new QVBoxLayout(&dlg);

    QLineEdit *nomEdit = new QLineEdit(nom);
    QLineEdit *prenomEdit = new QLineEdit(prenom);
    QLineEdit *cinEdit = new QLineEdit(cin);
    cinEdit->setMaxLength(8);

    QDateEdit *dateNaissEdit = new QDateEdit(dn.isValid() ? dn : QDate::currentDate());
    dateNaissEdit->setDisplayFormat("dd/MM/yyyy");
    dateNaissEdit->setCalendarPopup(true);

    // ===== ComboBox =====
    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({
        "EMPLOYE",
    "PRODUITS",
    "FOURNISSEURS",
    "COMMANDES",
    "MATIERES",
    "MACHINES"
    });

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({"BAC", "LICENCE", "MASTER"});

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({
        "ACTIF",
        "EN CONGE",
        "SUSPENDU",
        "EN FORMATION",
        "MALADIE",
        "MISSION",
        "DEMISSIONNE"
    });

    // sélectionner la valeur actuelle (forcer majuscules pour éviter conflits)
    int idxPoste = posteCombo->findText(poste.toUpper(), Qt::MatchFixedString);
    if (idxPoste >= 0) {
        posteCombo->setCurrentIndex(idxPoste);
    } else {
        // Si valeur inconnue, on force sur EMPLOYÉ par défaut pour éviter le crash DB
        int defIdx = posteCombo->findText("EMPLOYE", Qt::MatchFixedString);
        if (defIdx >= 0) posteCombo->setCurrentIndex(defIdx);
    }

    int idxNiveau = niveauCombo->findText(niveau, Qt::MatchFixedString);
    if (idxNiveau >= 0) niveauCombo->setCurrentIndex(idxNiveau);
    else {
        niveauCombo->addItem(niveau);
        niveauCombo->setCurrentText(niveau);
    }

    int idxStatus = statusCombo->findText(status.toUpper(), Qt::MatchFixedString);
    if (idxStatus >= 0) {
        statusCombo->setCurrentIndex(idxStatus);
    } else {
        // Si valeur inconnue, on force sur ACTIF par défaut
        int defIdx = statusCombo->findText("ACTIF", Qt::MatchFixedString);
        if (defIdx >= 0) statusCombo->setCurrentIndex(defIdx);
    }

    // ===== Salaire spinbox =====
    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setDecimals(2);
    salaireSpin->setValue(salaire);
    salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit(email);

    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Laisser vide pour ne pas modifier");

    QLineEdit *rfidEdit = new QLineEdit(rfid);
    rfidEdit->setPlaceholderText("RFID UID");

    connect(passwordEdit, &QLineEdit::textChanged, [&]() {
        QString mdp = passwordEdit->text();
        if (mdp.isEmpty()) {
            passwordEdit->setStyleSheet("");
        } else if (passwordValide(mdp)) {
            passwordEdit->setStyleSheet("border:2px solid green;");
        } else {
            passwordEdit->setStyleSheet("border:2px solid red;");
        }
    });

    main->addWidget(new QLabel("Nom:"));             main->addWidget(nomEdit);
    main->addWidget(new QLabel("Prénom:"));          main->addWidget(prenomEdit);
    main->addWidget(new QLabel("CIN:"));             main->addWidget(cinEdit);
    main->addWidget(new QLabel("Date naissance:"));  main->addWidget(dateNaissEdit);
    main->addWidget(new QLabel("Poste:"));           main->addWidget(posteCombo);
    main->addWidget(new QLabel("Niveau:"));          main->addWidget(niveauCombo);
    main->addWidget(new QLabel("Status:"));          main->addWidget(statusCombo);
    main->addWidget(new QLabel("Salaire:"));         main->addWidget(salaireSpin);
    main->addWidget(new QLabel("Email:"));           main->addWidget(emailEdit);
    main->addWidget(new QLabel("Nouveau mot de passe:")); main->addWidget(passwordEdit);
    main->addWidget(new QLabel("RFID UID:"));        main->addWidget(rfidEdit);

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *save = new QPushButton("Mettre à jour");
    QPushButton *cancel = new QPushButton("Annuler");
    btns->addWidget(save);
    btns->addWidget(cancel);
    main->addLayout(btns);

    connect(cancel, &QPushButton::clicked, &dlg, &QDialog::reject);

    connect(save, &QPushButton::clicked, [&]() {
        QString newNom = nomEdit->text().trimmed();
        QString newPrenom = prenomEdit->text().trimmed();
        QString newCin = cinEdit->text().trimmed();
        QString newPoste = posteCombo->currentText().trimmed();
        QString newNiveau = niveauCombo->currentText().trimmed();
        QString newStatus = statusCombo->currentText().trimmed();
        QString newEmail = emailEdit->text().trimmed();
        QString newPassword = passwordEdit->text();
        QString newRfid = rfidEdit->text().trimmed();
        double sal = salaireSpin->value();

        if (newNom.isEmpty() || newPrenom.isEmpty() || newCin.isEmpty() || newEmail.isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "Nom, Prénom, CIN et Email sont obligatoires.");
            return;
        }

        if (newCin.length() != 8) {
            QMessageBox::warning(&dlg, "Validation", "Le CIN doit contenir exactement 8 chiffres.");
            return;
        }

        for (const QChar &c : newCin) {
            if (!c.isDigit()) {
                QMessageBox::warning(&dlg, "Validation", "Le CIN doit contenir uniquement des chiffres.");
                return;
            }
        }

        if (!newPassword.isEmpty() && !passwordValide(newPassword)) {
            QMessageBox::warning(&dlg, "Validation", "Le mot de passe doit contenir au moins 8 caractères (majuscule, minuscule, chiffre, spécial).");
            return;
        }

        if (!newEmail.contains('@') || !newEmail.contains('.')) {
            QMessageBox::warning(&dlg, "Validation", "Email invalide.");
            return;
        }

        if (dateNaissEdit->date() > QDate::currentDate()) {
            QMessageBox::warning(&dlg, "Validation", "Date de naissance invalide.");
            return;
        }

        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM SMARTLEATHER.EMPLOYE "
                      "WHERE ID_EMPLOYE <> :id "
                      "AND (TRIM(CIN)=TRIM(:cin) OR LOWER(TRIM(EMAIL))=LOWER(TRIM(:email)))");
        check.bindValue(":id", id);
        check.bindValue(":cin", newCin);
        check.bindValue(":email", newEmail);

        if (!check.exec()) {
            QMessageBox::critical(&dlg, "DB", check.lastError().text());
            return;
        }

        if (check.next() && check.value(0).toInt() > 0) {
            QMessageBox::warning(&dlg, "Validation", "CIN ou Email existe déjà pour un autre employé.");
            return;
        }

        QSqlQuery q;
        QString sql = "UPDATE SMARTLEATHER.EMPLOYE SET "
                      "NOM=:nom, PRENOM=:prenom, CIN=:cin, DATE_NAISSANCE=:dn, "
                      "POSTE=:poste, NIVEAU=:niveau, STATUS=:status, SALAIRE=:sal, EMAIL=:email, RFID_UID=:rfid";
        if (!newPassword.isEmpty()) {
            sql += ", PASSWORD=:password";
        }
        sql += " WHERE ID_EMPLOYE=:id";
        
        q.prepare(sql);
        q.bindValue(":nom", newNom);
        q.bindValue(":prenom", newPrenom);
        q.bindValue(":cin", newCin);
        q.bindValue(":dn", dateNaissEdit->date());
        q.bindValue(":poste", newPoste);
        q.bindValue(":niveau", newNiveau);
        q.bindValue(":status", newStatus);
        q.bindValue(":sal", sal);
        q.bindValue(":email", newEmail);
        q.bindValue(":rfid", newRfid);
        if (!newPassword.isEmpty()) {
            q.bindValue(":password", newPassword);
        }
        q.bindValue(":id", id);

        if (!q.exec()) {
            QMessageBox::critical(&dlg, "DB Update", q.lastError().text());
            return;
        }

        QMessageBox::information(&dlg, "Succès", "Employé modifié avec succès.");
        dlg.accept();
        applyFilter();
    });

    dlg.exec();
}
/* =========================
   Supprimer
========================= */
void pageemployee::on_pushButton_3_clicked()
{
    int id = currentEmployeeId();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection", "Sélectionnez un employé à supprimer.");
        return;
    }

    if (id == m_idEmployeConnecte) {
        QMessageBox::warning(this, "Suppression", "Impossible de supprimer l'employé connecté.");
        return;
    }

    auto rep = QMessageBox::question(
        this,
        "Confirmation",
        QString("Voulez-vous supprimer l'employé ID = %1 ?").arg(id),
        QMessageBox::Yes | QMessageBox::No
        );
    if (rep != QMessageBox::Yes)
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::critical(this, "DB", "Impossible de démarrer la transaction.");
        return;
    }

    QSqlQuery q(db);

    // La table MESSAGE n'existe plus dans la base de données, 
    // on passe directement à la suppression de l'employé.

    // Ajouter ici d'autres tables enfants si nécessaire
    // ex:
    // q.prepare("DELETE FROM SMARTLEATHER.AUTRE_TABLE WHERE ID_EMPLOYE = :id");

    q.prepare("DELETE FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        db.rollback();
        QMessageBox::critical(this, "DB Delete", "Erreur suppression EMPLOYE :/n" + q.lastError().text());
        return;
    }

    if (!db.commit()) {
        db.rollback();
        QMessageBox::critical(this, "DB Delete", "Erreur commit transaction.");
        return;
    }

    QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
    applyFilter();
}

/* =========================
   Export PDF
========================= */
void pageemployee::on_pushButton_7_clicked()
{
    int row = ui->tableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection",
                             "Veuillez sélectionner un employé dans le tableau.");
        return;
    }

    QString nom           = ui->tableWidget->item(row, 1) ? ui->tableWidget->item(row, 1)->text() : "";
    QString prenom        = ui->tableWidget->item(row, 2) ? ui->tableWidget->item(row, 2)->text() : "";
    QString cin           = ui->tableWidget->item(row, 3) ? ui->tableWidget->item(row, 3)->text() : "";
    QString dateNaissance = ui->tableWidget->item(row, 4) ? ui->tableWidget->item(row, 4)->text() : "";
    QString email         = ui->tableWidget->item(row, 9) ? ui->tableWidget->item(row, 9)->text() : "";

    // Step 1: collect leave request details from a dedicated form dialog.
    QDialog leaveDialog(this);
    leaveDialog.setWindowTitle("Détails de la demande de congé");
    leaveDialog.setFixedSize(460, 330);

    QVBoxLayout *leaveLayout = new QVBoxLayout(&leaveDialog);
    leaveLayout->setContentsMargins(18, 18, 18, 18);
    leaveLayout->setSpacing(10);

    QLabel *typeLabel = new QLabel("Type de congé :");
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Congé annuel", "Congé maladie", "Congé maternité", "Congé paternité", "Congé exceptionnel"});

    QLabel *dureeLabel = new QLabel("Durée demandée (jours) :");
    QSpinBox *dureeSpin = new QSpinBox();
    dureeSpin->setRange(1, 365);
    dureeSpin->setValue(1);

    QLabel *remarqueLabel = new QLabel("Remarques :");
    QTextEdit *remarqueEdit = new QTextEdit();
    remarqueEdit->setPlaceholderText("Saisir une remarque...");
    remarqueEdit->setFixedHeight(90);

    QHBoxLayout *leaveButtons = new QHBoxLayout();
    QPushButton *btnOk = new QPushButton("Valider");
    QPushButton *btnCancel = new QPushButton("Annuler");
    leaveButtons->addStretch();
    leaveButtons->addWidget(btnOk);
    leaveButtons->addWidget(btnCancel);

    leaveLayout->addWidget(typeLabel);
    leaveLayout->addWidget(typeCombo);
    leaveLayout->addWidget(dureeLabel);
    leaveLayout->addWidget(dureeSpin);
    leaveLayout->addWidget(remarqueLabel);
    leaveLayout->addWidget(remarqueEdit);
    leaveLayout->addLayout(leaveButtons);

    connect(btnOk, &QPushButton::clicked, &leaveDialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &leaveDialog, &QDialog::reject);

    if (leaveDialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString typeConge = typeCombo->currentText();
    const int dureeJours = dureeSpin->value();
    const QString remarque = remarqueEdit->toPlainText().trimmed();

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Enregistrer la demande de congé en PDF",
        "demande_conge_" + nom + "_" + prenom + ".pdf",
        "Fichiers PDF (*.pdf)"
        );

    if (fileName.isEmpty())
        return;

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(12, 12, 12, 12));

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de générer le PDF.");
        return;
    }

    const int pageW = pdf.width();
    const int pageH = pdf.height();
    const int margin = 150;
    const QRect outerRect(margin, margin, pageW - 2 * margin, pageH - 2 * margin);

    QColor brownDark(92, 52, 28);
    QColor brown(122, 74, 38);
    QColor beige(248, 239, 228);
    QColor lineColor(201, 178, 162);
    QColor textColor(50, 35, 25);

    painter.fillRect(outerRect, Qt::white);
    painter.setPen(QPen(brownDark, 6));
    painter.drawRoundedRect(outerRect, 40, 40);

    QFont titleFont("Times New Roman", 20, QFont::Bold);
    QFont subFont("Arial", 10, QFont::Bold);
    QFont sectionFont("Arial", 11, QFont::Bold);
    QFont textFont("Arial", 10);

    int y = outerRect.top() + 220;

    painter.setPen(brownDark);
    painter.setFont(titleFont);
    painter.drawText(QRect(outerRect.left(), y, outerRect.width(), 120),
                     Qt::AlignCenter, "DEMANDE DE CONGÉ");

    y += 140;

    painter.setFont(subFont);
    painter.setPen(brown);
    painter.drawText(QRect(outerRect.left(), y, outerRect.width() - 100, 60),
                     Qt::AlignRight, "Date : " + QDate::currentDate().toString("dd/MM/yyyy"));

    y += 100;

    painter.setPen(QPen(lineColor, 2));
    painter.drawLine(outerRect.left() + 80, y, outerRect.right() - 80, y);

    y += 60;

    QRect infoRect(outerRect.left() + 100, y, outerRect.width() - 200, 950);
    painter.setPen(Qt::NoPen);
    painter.setBrush(beige);
    painter.drawRoundedRect(infoRect, 25, 25);

    painter.setPen(brownDark);
    painter.setFont(sectionFont);
    painter.drawText(QRect(infoRect.left() + 30, infoRect.top() + 25, 400, 50),
                     Qt::AlignLeft, "Informations de l'employé");

    painter.setPen(QPen(lineColor, 1));
    painter.drawLine(infoRect.left() + 30, infoRect.top() + 80, infoRect.right() - 30, infoRect.top() + 80);

    painter.setFont(textFont);
    painter.setPen(textColor);

    int leftX = infoRect.left() + 40;
    int rightX = infoRect.left() + infoRect.width() / 2 + 20;
    int lineY1 = infoRect.top() + 150;
    int step = 120;

    painter.drawText(leftX,  lineY1,            "Nom : " + nom);
    painter.drawText(leftX,  lineY1 + step,     "Prénom : " + prenom);
    painter.drawText(leftX,  lineY1 + 2 * step, "CIN : " + cin);
    painter.drawText(leftX,  lineY1 + 3 * step, "Date de naissance : " + dateNaissance);

    painter.drawText(rightX, lineY1,            "Type de congé : " + typeConge);
    painter.drawText(rightX, lineY1 + step,     "Durée demandée : " + QString::number(dureeJours) + " jour(s)");
    painter.drawText(rightX, lineY1 + 2 * step, "Date de demande : " + QDate::currentDate().toString("dd/MM/yyyy"));
    painter.drawText(rightX, lineY1 + 3 * step, "Email : " + email);

    y = infoRect.bottom() + 100;

    painter.setFont(sectionFont);
    painter.setPen(brownDark);
    painter.drawText(outerRect.left() + 100, y, "Objet : " + typeConge);

    y += 70;

    painter.setPen(QPen(lineColor, 1));
    painter.drawLine(outerRect.left() + 100, y, outerRect.right() - 100, y);

    y += 80;

    painter.setFont(textFont);
    painter.setPen(textColor);

    QString paragraphe =
        "Je soussigné(e), " + nom + " " + prenom +
        ", sollicite par la présente l'autorisation de bénéficier d'un " + typeConge +
        " pour une durée de " + QString::number(dureeJours) + " jour(s).\n\n"
        "Remarques : " + (remarque.isEmpty() ? QString("Aucune") : remarque) + "\n\n"
        "Je vous prie de bien vouloir examiner favorablement ma demande. "
        "Je reste à votre disposition pour toute information complémentaire.";

    QRect textRect(outerRect.left() + 100, y, outerRect.width() - 200, 700);
    painter.drawText(textRect, Qt::TextWordWrap | Qt::AlignJustify, paragraphe);

    y = textRect.bottom() + 180;

    painter.setFont(sectionFont);
    painter.setPen(brownDark);
    painter.drawText(QRect(outerRect.left() + 100, y, 500, 50),
                     Qt::AlignLeft, "Signature de l'employé");

    painter.setPen(QPen(brown, 2));
    painter.drawLine(outerRect.right() - 1700, y + 140, outerRect.right() - 300, y + 140);

    painter.setPen(QPen(lineColor, 1));
    painter.drawLine(outerRect.left() + 80, outerRect.bottom() - 180,
                     outerRect.right() - 80, outerRect.bottom() - 180);

    painter.setFont(QFont("Arial", 8));
    painter.setPen(QColor(120, 100, 90));
    painter.drawText(QRect(outerRect.left(), outerRect.bottom() - 140, outerRect.width(), 60),
                     Qt::AlignCenter,
                     "Document généré automatiquement - Smart Leather Factory");

    painter.end();

    QMessageBox::information(this, "Succès", "PDF de demande de congé généré avec succès.");
}

/* =========================
   Statistiques
========================= */
void pageemployee::on_pushButton_9_clicked()
{
    int total = ui->tableWidget->rowCount();
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun employé à afficher.");
        return;
    }

    QMap<QString, int> statusCount;

    for (int r = 0; r < total; ++r) {
        QString status = ui->tableWidget->item(r, 7)
        ? ui->tableWidget->item(r, 7)->text().trimmed().toUpper()
        : "";

        if (status.isEmpty())
            status = "NON DEFINI";

        statusCount[status] += 1;
    }

    int nbActif       = statusCount.value("ACTIF", 0);
    int nbConge       = statusCount.value("EN CONGE", 0);
    int nbSuspendu    = statusCount.value("SUSPENDU", 0);
    int nbFormation   = statusCount.value("EN FORMATION", 0);
    int nbMaladie     = statusCount.value("MALADIE", 0);
    int nbMission     = statusCount.value("MISSION", 0);
    int nbDemissionne = statusCount.value("DEMISSIONNE", 0);

    QDialog dlg(this);
    dlg.setWindowTitle("Statistiques des employés");
    dlg.setFixedSize(1000, 750); // Slightly larger to fit more info
    dlg.setStyleSheet(
        "QDialog { background-color: #f1e7dc; }"
        "QLabel { color: #3a2a20; font-size: 13px; }"
        "QFrame#card {"
        "  background-color: #fffaf5;"
        "  border: 2px solid #b08a6b;"
        "  border-radius: 14px;"
        "}"
        "QFrame#rowCard {"
        "  background-color:#fffaf5;"
        "  border:1px solid #dcc8b7;"
        "  border-radius:10px;"
        "}"
        "QPushButton {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a4a2e, stop:1 #5b2f1d);"
        "  color: #fffaf5;"
        "  border-radius: 10px;"
        "  border: 2px solid #3a1f14;"
        "  padding: 9px 24px;"
        "  font-weight: 700;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5a3a, stop:1 #6b3e26);"
        "}"
        );

    QVBoxLayout *main = new QVBoxLayout(&dlg);
    main->setContentsMargins(20, 20, 20, 20);
    main->setSpacing(15);

    QLabel *title = new QLabel("Tableau de bord des employés");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 28px; font-weight: 800; color: #5b2f1d; letter-spacing: 1px;");
    main->addWidget(title);

    // --- KPI Section (Two Rows) ---
    QVBoxLayout *kpisMainLayout = new QVBoxLayout();
    kpisMainLayout->setSpacing(10);

    QHBoxLayout *kpiRow1 = new QHBoxLayout();
    kpiRow1->setSpacing(12);

    QHBoxLayout *kpiRow2 = new QHBoxLayout();
    kpiRow2->setSpacing(12);

    auto makeCard = [](const QString &big, const QString &small, const QString &color = "#5b2f1d") {
        QFrame *card = new QFrame();
        card->setObjectName("card");
        card->setMinimumHeight(85);

        QVBoxLayout *l = new QVBoxLayout(card);
        l->setContentsMargins(10, 10, 10, 10);

        QLabel *bigLabel = new QLabel(big);
        bigLabel->setAlignment(Qt::AlignCenter);
        bigLabel->setStyleSheet(QString("font-size: 28px; font-weight: 800; color: %1;").arg(color));

        QLabel *smallLabel = new QLabel(small);
        smallLabel->setAlignment(Qt::AlignCenter);
        smallLabel->setStyleSheet("font-size: 12px; color: #7b6656; font-weight: 600;");

        l->addWidget(bigLabel);
        l->addWidget(smallLabel);
        return card;
    };

    // Row 1: Total and primary statuses
    kpiRow1->addWidget(makeCard(QString::number(total), "Employés"));
    kpiRow1->addWidget(makeCard(QString::number(nbActif), "Actifs", "#6f8f3d"));
    kpiRow1->addWidget(makeCard(QString::number(nbConge), "En congé", "#d18b2f"));
    kpiRow1->addWidget(makeCard(QString::number(nbSuspendu), "Suspendus", "#c4573a"));

    // Row 2: Secondary statuses
    kpiRow2->addWidget(makeCard(QString::number(nbFormation), "En formation", "#4b88c7"));
    kpiRow2->addWidget(makeCard(QString::number(nbMaladie), "Maladie", "#8e6ccf"));
    kpiRow2->addWidget(makeCard(QString::number(nbMission), "Mission", "#2f9d8f"));
    kpiRow2->addWidget(makeCard(QString::number(nbDemissionne), "Démissionnés", "#7a7a7a"));

    kpisMainLayout->addLayout(kpiRow1);
    kpisMainLayout->addLayout(kpiRow2);
    main->addLayout(kpisMainLayout);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->setSpacing(18);

    QFrame *chartCard = new QFrame();
    chartCard->setObjectName("card");
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(15, 15, 15, 15);

    QLabel *chartTitle = new QLabel("Répartition par statut");
    chartTitle->setAlignment(Qt::AlignCenter);
    chartTitle->setStyleSheet("font-size: 24px; font-weight: 800; color: #5b2f1d;");
    chartLayout->addWidget(chartTitle);

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.45);

    auto addSlice = [&](const QString &label, int value, const QColor &color) {
        if (value <= 0) return;
        QPieSlice *slice = series->append(label, value);
        slice->setBrush(color);
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside); // Move labels outside for better readability
        double pct = total > 0 ? (value * 100.0 / total) : 0.0;
        slice->setLabel(QString("%1 (%2%)").arg(label).arg(QString::number(pct, 'f', 1)));
        slice->setLabelColor(QColor("#5b2f1d"));
        slice->setLabelFont(QFont("Segoe UI", 9, QFont::Bold));
    };

    addSlice("Actif",        nbActif,       QColor("#6f8f3d"));
    addSlice("En conge",     nbConge,       QColor("#d18b2f"));
    addSlice("Suspendu",     nbSuspendu,    QColor("#c4573a"));
    addSlice("En formation", nbFormation,   QColor("#4b88c7"));
    addSlice("Maladie",      nbMaladie,     QColor("#8e6ccf"));
    addSlice("Mission",      nbMission,     QColor("#2f9d8f"));
    addSlice("Demissionne",  nbDemissionne, QColor("#7a7a7a"));

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(" ");
    chart->legend()->setVisible(true); // Enable legend for additional clarity
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(QFont("Segoe UI", 9));
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(10, 10, 10, 10));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 340);

    chartLayout->addWidget(chartView);

    QFrame *detailsCard = new QFrame();
    detailsCard->setObjectName("card");
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsCard);
    detailsLayout->setContentsMargins(18, 18, 18, 18);
    detailsLayout->setSpacing(12);

    QLabel *detailsTitle = new QLabel("Détails des statuts");
    detailsTitle->setStyleSheet("font-size: 24px; font-weight: 800; color: #5b2f1d;");
    detailsLayout->addWidget(detailsTitle);

    auto addStatusRow = [&](const QString &label, int value, const QString &color) {
        double pct = total > 0 ? (value * 100.0 / total) : 0.0;

        QFrame *row = new QFrame();
        row->setObjectName("rowCard");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(12, 8, 12, 8);

        QLabel *dot = new QLabel("●");
        dot->setStyleSheet("font-size:18px; color:" + color + ";");

        QLabel *name = new QLabel(label);
        name->setStyleSheet("font-size:17px; font-weight:800; color:#5b2f1d;");

        QLabel *val = new QLabel(QString("%1 employé(s)  |  %2%")
                                     .arg(value)
                                     .arg(QString::number(pct, 'f', 1)));
        val->setStyleSheet("font-size:16px; color:#6e5a4d; font-weight:600;");

        hl->addWidget(dot);
        hl->addWidget(name);
        hl->addStretch();
        hl->addWidget(val);

        detailsLayout->addWidget(row);
    };

    addStatusRow("Actif",        nbActif,       "#6f8f3d");
    addStatusRow("En conge",     nbConge,       "#d18b2f");
    addStatusRow("Suspendu",     nbSuspendu,    "#c4573a");
    addStatusRow("En formation", nbFormation,   "#4b88c7");
    addStatusRow("Maladie",      nbMaladie,     "#8e6ccf");
    addStatusRow("Mission",      nbMission,     "#2f9d8f");
    addStatusRow("Demissionne",  nbDemissionne, "#7a7a7a");

    detailsLayout->addStretch();

    centerLayout->addWidget(chartCard, 1);
    centerLayout->addWidget(detailsCard, 1);

    main->addLayout(centerLayout);

    QPushButton *close = new QPushButton("Fermer");
    connect(close, &QPushButton::clicked, &dlg, &QDialog::accept);
    main->addWidget(close, 0, Qt::AlignCenter);

    dlg.exec();
}

/* =========================
   Navigation
========================= */
void pageemployee::on_pushButton_6_clicked()
{
    if (denyIfRoleMismatch(this, m_idEmployeConnecte, "Commandes")) return;
    auto *next = new commandes(m_idEmployeConnecte, nullptr);
    next->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_20_clicked()
{
    if (denyIfRoleMismatch(this, m_idEmployeConnecte, "Fournisseurs")) return;
    auto *next = new fournisseurs(m_idEmployeConnecte, nullptr);
    next->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_21_clicked()
{
    if (denyIfRoleMismatch(this, m_idEmployeConnecte, "Produits")) return;
    auto *next = new produitswindow(m_idEmployeConnecte, nullptr);
    next->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_22_clicked()
{
    if (denyIfRoleMismatch(this, m_idEmployeConnecte, "Matieres")) return;
    auto *next = new Matieres(m_idEmployeConnecte, nullptr);
    next->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_23_clicked()
{
    if (denyIfRoleMismatch(this, m_idEmployeConnecte, "Machines")) return;
    auto *next = new pagemachine(m_idEmployeConnecte, nullptr);
    next->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_4_clicked()
{
    qDebug() << "before opening chat, m_idEmployeConnecte =" << m_idEmployeConnecte;
    pagechat *chat = new pagechat(m_idEmployeConnecte, displayNameEmployeConnecte(), this, nullptr);
    chat->show();
    this->hide();
}

void pageemployee::on_pushButton_5_clicked()
{
    auto *lg = new login(nullptr);
    lg->show();
    this->close();
    this->deleteLater();
}

void pageemployee::on_pushButton_8_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logout",
                                  "Voulez-vous vous déconnecter ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        login *lg = new login();
        lg->show();
        this->close();
    }
}
