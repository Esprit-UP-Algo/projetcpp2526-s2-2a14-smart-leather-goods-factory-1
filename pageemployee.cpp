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

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>


#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>

#include "commandes.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"
#include "pagechat.h"

pageemployee::pageemployee(int idEmployeConnecte, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemployee)
    , m_idEmployeConnecte(idEmployeConnecte)
{
    ui->setupUi(this);

    qDebug() << "pageemployee opened, idEmployeConnecte =" << m_idEmployeConnecte;

    setupTable();
    loadEmployeesTable();

    connect(ui->searchIdEdit,  &QLineEdit::textChanged, this, &pageemployee::applyFilter);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pageemployee::applyFilter);
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
    ui->tableWidget->setColumnCount(10);
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
        "Email"
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
        "SELECT ID_EMPLOYE, NOM, PRENOM, CIN, DATE_NAISSANCE, POSTE, NIVEAU, STATUS, SALAIRE, EMAIL "
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

/* =========================
   Ajouter
========================= */
void pageemployee::on_pushButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Ajouter un employé");
    dlg.setFixedSize(520, 560);

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
        "Responsable Stock",
        "Service Achat",
        "Service Technique",
        "Service Client",
        "Commercial",
        "Comptable",
        "Directeur"
    });

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({"BAC", "LICENCE", "MASTER"});

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

    // ===== Salaire en spinbox =====
    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setDecimals(2);
    salaireSpin->setValue(1500.0);
    salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit();

    main->addWidget(new QLabel("Nom:"));             main->addWidget(nomEdit);
    main->addWidget(new QLabel("Prénom:"));          main->addWidget(prenomEdit);
    main->addWidget(new QLabel("CIN:"));             main->addWidget(cinEdit);
    main->addWidget(new QLabel("Date naissance:"));  main->addWidget(dateNaissEdit);
    main->addWidget(new QLabel("Poste:"));           main->addWidget(posteCombo);
    main->addWidget(new QLabel("Niveau:"));          main->addWidget(niveauCombo);
    main->addWidget(new QLabel("Status:"));          main->addWidget(statusCombo);
    main->addWidget(new QLabel("Salaire:"));         main->addWidget(salaireSpin);
    main->addWidget(new QLabel("Email:"));           main->addWidget(emailEdit);

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
        double sal = salaireSpin->value();

        if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || email.isEmpty()) {
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
            "(NOM, PRENOM, CIN, DATE_NAISSANCE, POSTE, NIVEAU, STATUS, SALAIRE, EMAIL) "
            "VALUES (:nom, :prenom, :cin, :dn, :poste, :niveau, :status, :sal, :email)"
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

    QDialog dlg(this);
    dlg.setWindowTitle("Modifier employé");
    dlg.setFixedSize(520, 580);

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
        "Produits",
        "Fournisseurs",
        "Machines",
        "Commandes",
        "Matieres",
        "Employe"
    });

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({"BAC", "LICENCE", "MASTER"});

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

    // sélectionner la valeur actuelle
    int idxPoste = posteCombo->findText(poste, Qt::MatchFixedString);
    if (idxPoste >= 0) posteCombo->setCurrentIndex(idxPoste);
    else {
        posteCombo->addItem(poste);
        posteCombo->setCurrentText(poste);
    }

    int idxNiveau = niveauCombo->findText(niveau, Qt::MatchFixedString);
    if (idxNiveau >= 0) niveauCombo->setCurrentIndex(idxNiveau);
    else {
        niveauCombo->addItem(niveau);
        niveauCombo->setCurrentText(niveau);
    }

    int idxStatus = statusCombo->findText(status, Qt::MatchFixedString);
    if (idxStatus >= 0) statusCombo->setCurrentIndex(idxStatus);
    else {
        statusCombo->addItem(status);
        statusCombo->setCurrentText(status);
    }

    // ===== Salaire spinbox =====
    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 100000);
    salaireSpin->setDecimals(2);
    salaireSpin->setValue(salaire);
    salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit(email);

    main->addWidget(new QLabel("Nom:"));             main->addWidget(nomEdit);
    main->addWidget(new QLabel("Prénom:"));          main->addWidget(prenomEdit);
    main->addWidget(new QLabel("CIN:"));             main->addWidget(cinEdit);
    main->addWidget(new QLabel("Date naissance:"));  main->addWidget(dateNaissEdit);
    main->addWidget(new QLabel("Poste:"));           main->addWidget(posteCombo);
    main->addWidget(new QLabel("Niveau:"));          main->addWidget(niveauCombo);
    main->addWidget(new QLabel("Status:"));          main->addWidget(statusCombo);
    main->addWidget(new QLabel("Salaire:"));         main->addWidget(salaireSpin);
    main->addWidget(new QLabel("Email:"));           main->addWidget(emailEdit);

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
        q.prepare(
            "UPDATE SMARTLEATHER.EMPLOYE SET "
            "NOM=:nom, PRENOM=:prenom, CIN=:cin, DATE_NAISSANCE=:dn, "
            "POSTE=:poste, NIVEAU=:niveau, STATUS=:status, SALAIRE=:sal, EMAIL=:email "
            "WHERE ID_EMPLOYE=:id"
            );
        q.bindValue(":nom", newNom);
        q.bindValue(":prenom", newPrenom);
        q.bindValue(":cin", newCin);
        q.bindValue(":dn", dateNaissEdit->date());
        q.bindValue(":poste", newPoste);
        q.bindValue(":niveau", newNiveau);
        q.bindValue(":status", newStatus);
        q.bindValue(":sal", sal);
        q.bindValue(":email", newEmail);
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

    // Supprimer d'abord les enfants liés
    q.prepare("DELETE FROM SMARTLEATHER.MESSAGE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        db.rollback();
        QMessageBox::critical(this, "DB Delete", "Erreur suppression MESSAGE :/n" + q.lastError().text());
        return;
    }

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
    QString poste         = ui->tableWidget->item(row, 5) ? ui->tableWidget->item(row, 5)->text() : "";
    QString niveau        = ui->tableWidget->item(row, 6) ? ui->tableWidget->item(row, 6)->text() : "";
    QString salaire       = ui->tableWidget->item(row, 8) ? ui->tableWidget->item(row, 8)->text() : "";
    QString email         = ui->tableWidget->item(row, 9) ? ui->tableWidget->item(row, 9)->text() : "";

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

    painter.drawText(rightX, lineY1,            "Poste : " + poste);
    painter.drawText(rightX, lineY1 + step,     "Niveau : " + niveau);
    painter.drawText(rightX, lineY1 + 2 * step, "Salaire : " + salaire + " DT");
    painter.drawText(rightX, lineY1 + 3 * step, "Email : " + email);

    y = infoRect.bottom() + 100;

    painter.setFont(sectionFont);
    painter.setPen(brownDark);
    painter.drawText(outerRect.left() + 100, y, "Objet : Demande de congé");

    y += 70;

    painter.setPen(QPen(lineColor, 1));
    painter.drawLine(outerRect.left() + 100, y, outerRect.right() - 100, y);

    y += 80;

    painter.setFont(textFont);
    painter.setPen(textColor);

    QString paragraphe =
        "Je soussigné(e), " + nom + " " + prenom +
        ", occupant le poste de " + poste +
        ", sollicite par la présente l'autorisation de bénéficier d'un congé./n/n"
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
        ? ui->tableWidget->item(r, 7)->text().trimmed()
        : "";

        if (status.isEmpty())
            status = "Non défini";

        statusCount[status] += 1;
    }

    int nbActif       = statusCount.value("Actif", 0);
    int nbConge       = statusCount.value("En congé", 0);
    int nbSuspendu    = statusCount.value("Suspendu", 0);
    int nbFormation   = statusCount.value("En formation", 0);
    int nbMaladie     = statusCount.value("Maladie", 0);
    int nbMission     = statusCount.value("Mission", 0);
    int nbDemissionne = statusCount.value("Démissionné", 0);

    QDialog dlg(this);
    dlg.setWindowTitle("Statistiques des employés");
    dlg.setFixedSize(900, 620);
    dlg.setStyleSheet(
        "QDialog { background-color: #f8f4ef; }"
        "QLabel { color: #4b2e1f; font-size: 13px; }"
        "QFrame#card { background-color: white; border: 2px solid #d8c2ad; border-radius: 16px; }"
        "QPushButton { background-color: #6f8f3d; color: white; border-radius: 10px; padding: 10px 22px; font-weight: bold; font-size: 14px; }"
        "QPushButton:hover { background-color: #84a94a; }"
        );

    QVBoxLayout *main = new QVBoxLayout(&dlg);
    main->setContentsMargins(20, 20, 20, 20);
    main->setSpacing(18);

    QLabel *title = new QLabel("Tableau de bord des employés");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #7a451f;");
    main->addWidget(title);

    QHBoxLayout *kpiLayout = new QHBoxLayout();
    kpiLayout->setSpacing(12);

    auto makeCard = [](const QString &big, const QString &small) {
        QFrame *card = new QFrame();
        card->setObjectName("card");
        card->setMinimumHeight(90);

        QVBoxLayout *l = new QVBoxLayout(card);
        l->setContentsMargins(15, 12, 15, 12);

        QLabel *bigLabel = new QLabel(big);
        bigLabel->setAlignment(Qt::AlignCenter);
        bigLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #6b3e1a;");

        QLabel *smallLabel = new QLabel(small);
        smallLabel->setAlignment(Qt::AlignCenter);
        smallLabel->setStyleSheet("font-size: 13px; color: #7b6656;");

        l->addWidget(bigLabel);
        l->addWidget(smallLabel);
        return card;
    };

    kpiLayout->addWidget(makeCard(QString::number(total), "Employés"));
    kpiLayout->addWidget(makeCard(QString::number(nbActif), "Actifs"));
    kpiLayout->addWidget(makeCard(QString::number(nbConge), "En congé"));
    kpiLayout->addWidget(makeCard(QString::number(nbSuspendu), "Suspendus"));

    main->addLayout(kpiLayout);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->setSpacing(18);

    QFrame *chartCard = new QFrame();
    chartCard->setObjectName("card");
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(15, 15, 15, 15);

    QLabel *chartTitle = new QLabel("Répartition par statut");
    chartTitle->setAlignment(Qt::AlignCenter);
    chartTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #6b3e1a;");
    chartLayout->addWidget(chartTitle);

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.45);

    auto addSlice = [&](const QString &label, int value, const QColor &color) {
        if (value <= 0) return;
        QPieSlice *slice = series->append(label, value);
        slice->setBrush(color);
        slice->setLabelVisible(true);
        double pct = total > 0 ? (value * 100.0 / total) : 0.0;
        slice->setLabel(QString("%1/n%2%").arg(label).arg(QString::number(pct, 'f', 1)));
    };

    addSlice("Actif",        nbActif,       QColor("#6f8f3d"));
    addSlice("En congé",     nbConge,       QColor("#d18b2f"));
    addSlice("Suspendu",     nbSuspendu,    QColor("#c4573a"));
    addSlice("En formation", nbFormation,   QColor("#4b88c7"));
    addSlice("Maladie",      nbMaladie,     QColor("#8e6ccf"));
    addSlice("Mission",      nbMission,     QColor("#2f9d8f"));
    addSlice("Démissionné",  nbDemissionne, QColor("#7a7a7a"));

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("");
    chart->legend()->hide();
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(380, 320);

    chartLayout->addWidget(chartView);

    QFrame *detailsCard = new QFrame();
    detailsCard->setObjectName("card");
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsCard);
    detailsLayout->setContentsMargins(18, 18, 18, 18);
    detailsLayout->setSpacing(12);

    QLabel *detailsTitle = new QLabel("Détails des statuts");
    detailsTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #6b3e1a;");
    detailsLayout->addWidget(detailsTitle);

    auto addStatusRow = [&](const QString &label, int value, const QString &color) {
        double pct = total > 0 ? (value * 100.0 / total) : 0.0;

        QFrame *row = new QFrame();
        row->setStyleSheet("background-color:#fbf8f4; border:1px solid #e6d7c8; border-radius:10px;");
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(12, 8, 12, 8);

        QLabel *dot = new QLabel("●");
        dot->setStyleSheet("font-size:18px; color:" + color + ";");

        QLabel *name = new QLabel(label);
        name->setStyleSheet("font-size:14px; font-weight:bold; color:#5a341c;");

        QLabel *val = new QLabel(QString("%1 employé(s)  |  %2%")
                                     .arg(value)
                                     .arg(QString::number(pct, 'f', 1)));
        val->setStyleSheet("font-size:13px; color:#6e5a4d;");

        hl->addWidget(dot);
        hl->addWidget(name);
        hl->addStretch();
        hl->addWidget(val);

        detailsLayout->addWidget(row);
    };

    addStatusRow("Actif",        nbActif,       "#6f8f3d");
    addStatusRow("En congé",     nbConge,       "#d18b2f");
    addStatusRow("Suspendu",     nbSuspendu,    "#c4573a");
    addStatusRow("En formation", nbFormation,   "#4b88c7");
    addStatusRow("Maladie",      nbMaladie,     "#8e6ccf");
    addStatusRow("Mission",      nbMission,     "#2f9d8f");
    addStatusRow("Démissionné",  nbDemissionne, "#7a7a7a");

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
    hide();
    (new commandes(m_idEmployeConnecte, this))->show();
}

void pageemployee::on_pushButton_20_clicked()
{
    hide();
    (new fournisseurs(m_idEmployeConnecte, this))->show();
}

void pageemployee::on_pushButton_21_clicked()
{
    hide();
    (new products(m_idEmployeConnecte, this))->show();
}

void pageemployee::on_pushButton_22_clicked()
{
    hide();
    (new Matieres(m_idEmployeConnecte, this))->show();
}

void pageemployee::on_pushButton_23_clicked()
{
    hide();
    (new pagemachine(m_idEmployeConnecte, this))->show();
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
    hide();
    login *lg = new login(this);
    lg->show();
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
