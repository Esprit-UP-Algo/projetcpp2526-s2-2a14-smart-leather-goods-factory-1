#include "pageemployee.h" // Force recompile
#include "ui_pageemployee.h"

#include <QtSql>
#include <QMessageBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QSqlError>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QMap>
#include <QApplication>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDate>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>


#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>

#include "commandes.h"
#include "login.h"
#include "fournisseurs.h"
#include "produitswindow.h"
#include "matieres.h"
#include "pagemachine.h"
#include "pagechat.h"

static const char* DIALOG_BASE_STYLE = R"(
QDialog {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #faf6f1, stop:0.5 #f0e8de, stop:1 #e8ddd0);
    border: none;
}
QLabel#headerLabel {
    color: #4a2517;
    font-size: 22px;
    font-weight: 800;
    letter-spacing: 2px;
    padding: 8px 0;
}
QLabel#subHeaderLabel {
    color: #8b6f5a;
    font-size: 11px;
    letter-spacing: 1px;
    margin-bottom: 15px;
}
QLabel {
    color: #5b3a28;
    font-weight: 600;
    font-size: 12px;
    background: transparent;
}
QLabel#errorLabel {
    color: #c0392b;
    font-size: 11px;
    font-weight: 600;
    font-style: italic;
    background: transparent;
    padding: 0 2px;
}
QLineEdit, QDoubleSpinBox, QDateEdit, QComboBox, QSpinBox {
    background-color: rgba(255, 255, 255, 0.85);
    border: 2px solid #d4c4b0;
    border-radius: 12px;
    padding: 10px 14px;
    color: #3a2a20;
    font-size: 13px;
    selection-background-color: #c9a87c;
}
QLineEdit:focus, QDoubleSpinBox:focus, QDateEdit:focus, QComboBox:focus, QSpinBox:focus {
    border: 2px solid #8b6f5a;
    background-color: white;
}
QLineEdit[error="true"] { border: 2px solid #e74c3c; background-color: #fdf2f2; }
QComboBox::drop-down { border: none; padding-right: 10px; }
QComboBox QAbstractItemView {
    background-color: #faf6f1; border: 2px solid #d4c4b0; border-radius: 8px;
    selection-background-color: #c9a87c; padding: 4px; color: #3a2a20;
}
)";

static const char* BTN_SAVE_GREEN = R"(
QPushButton#btnSave {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #6d9b3a, stop:1 #8fb85a);
    border: none; border-radius: 14px; padding: 12px 28px;
    font-weight: 700; font-size: 13px; color: white; letter-spacing: 1px;
}
QPushButton#btnSave:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7dab4a, stop:1 #9fc86a); }
QPushButton#btnSave:pressed { background: #5a8a2a; }
QPushButton#btnCancel {
    background: transparent; border: 2px solid #c9b8a5; border-radius: 14px;
    padding: 12px 28px; font-weight: 600; font-size: 13px; color: #8b7a6a; letter-spacing: 1px;
}
QPushButton#btnCancel:hover { background: rgba(0,0,0,0.04); border-color: #a0907e; color: #5b4a3a; }
)";

static const char* BTN_SAVE_AMBER = R"(
QPushButton#btnSave {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c47a2c, stop:1 #e09a4c);
    border: none; border-radius: 14px; padding: 12px 28px;
    font-weight: 700; font-size: 13px; color: white; letter-spacing: 1px;
}
QPushButton#btnSave:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #d48a3c, stop:1 #f0aa5c); }
QPushButton#btnSave:pressed { background: #b06a1c; }
QPushButton#btnCancel {
    background: transparent; border: 2px solid #c9b8a5; border-radius: 14px;
    padding: 12px 28px; font-weight: 600; font-size: 13px; color: #8b7a6a; letter-spacing: 1px;
}
QPushButton#btnCancel:hover { background: rgba(0,0,0,0.04); border-color: #a0907e; color: #5b4a3a; }
)";


void addShadow(QWidget* w, int blur=20, int off=4) {
    QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect();
    e->setBlurRadius(blur); e->setXOffset(0); e->setYOffset(off);
    e->setColor(QColor(0,0,0,80)); w->setGraphicsEffect(e);
}

QWidget* createSeparator() {
    QWidget* s = new QWidget(); s->setFixedHeight(2);
    s->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 transparent, stop:0.5 #dcd1c5, stop:1 transparent);");
    return s;
}

void setFieldError(QWidget* w, QLabel* err, bool visible, const QString &msg="") {
    w->setStyleSheet(visible ? "border: 2px solid #d9534f; background: #fff5f5; border-radius:10px; padding:10px;" : "");
    err->setText(msg); err->setVisible(visible);
}

pageemployee::pageemployee(int idEmployeConnecte, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemployee)
    , m_idEmployeConnecte(idEmployeConnecte)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    
    // Hide old sidebar and cleanup
    if (ui->groupBox) ui->groupBox->hide();
    if (ui->groupBox_2) ui->groupBox_2->hide();
    if (ui->layoutWidget) ui->layoutWidget->setStyleSheet("background: transparent;");

    // Premium Sidebar Setup
    QString navBtnStyle =
        "QPushButton {"
        "  background: transparent; border: none; color: #c9b8a5;"
        "  text-align: left; padding-left: 20px; font-size: 14px; font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 255, 255, 0.1); color: white; border-left: 4px solid #c9a87c;"
        "}";

    QWidget *sidebar = new QWidget(this);
    sidebar->setGeometry(0, 0, 240, 900); 
    sidebar->setStyleSheet("background-color: #3a1f14;");

    // Smart Leather Logo on Sidebar
    QLabel *logoLab = new QLabel(sidebar); // Parent to sidebar
    logoLab->setGeometry(20, 10, 211, 121);
    logoLab->setPixmap(QPixmap(":/Logo.png"));
    logoLab->setScaledContents(true);
    logoLab->show();
    logoLab->raise();
    
    QVBoxLayout *navLayout = new QVBoxLayout(sidebar);
    navLayout->setContentsMargins(0, 160, 0, 20);
    navLayout->setSpacing(5);

    auto addNavBtn = [&](const QString &txt, const char* slot, bool active = false) {
        QPushButton *btn = new QPushButton("  " + txt);
        btn->setMinimumHeight(45);
        if (active) {
            btn->setStyleSheet(navBtnStyle + "QPushButton { background-color: rgba(255,255,255,0.1); color:white; border-left:4px solid #c9a87c; }");
        } else {
            btn->setStyleSheet(navBtnStyle);
            connect(btn, SIGNAL(clicked()), this, slot);
        }
        navLayout->addWidget(btn);
        return btn;
    };

    addNavBtn("Employés", nullptr, true);
    addNavBtn("Produits", SLOT(navToProduits()));
    addNavBtn("Commandes", SLOT(navToCommandes()));
    addNavBtn("Fournisseurs", SLOT(navToFournisseurs()));
    addNavBtn("Matières", SLOT(navToMatieres()));
    addNavBtn("Machines", SLOT(navToMachines()));

    navLayout->addStretch();
    addNavBtn("Déconnexion", SLOT(navToLogout()));

    // Central layout adjustment (clearing the 240px sidebar)
    if(ui->label_8) {
        ui->label_8->setGeometry(260, 50, 600, 60);
        ui->label_8->setText("✦ GESTION DES EMPLOYÉS");
    }

    // Align search widgets
    if(ui->label_3) ui->label_3->setGeometry(260, 160, 60, 31);
    if(ui->searchIdEdit) {
        ui->searchIdEdit->setGeometry(320, 160, 140, 31);
        ui->searchIdEdit->setStyleSheet("background: white; border-radius: 8px; padding: 4px; border: 1px solid #dcd1c5;");
        ui->searchIdEdit->setPlaceholderText("Email...");
    }
    if(ui->label_4) ui->label_4->setGeometry(480, 160, 60, 31);
    if(ui->searchNomEdit) {
        ui->searchNomEdit->setGeometry(540, 160, 140, 31);
        ui->searchNomEdit->setStyleSheet("background: white; border-radius: 8px; padding: 4px; border: 1px solid #dcd1c5;");
        ui->searchNomEdit->setPlaceholderText("Nom...");
    }
    if(ui->search) {
        ui->search->setGeometry(700, 160, 100, 31);
        ui->search->setText("Filtre");
    }
    if(ui->actualiser) {
        ui->actualiser->setGeometry(810, 160, 100, 31);
        ui->actualiser->setText("Actualiser");
    }
    if(ui->pushButton_9) {
        ui->pushButton_9->setGeometry(930, 160, 120, 31);
        ui->pushButton_9->setText("Stats RH");
    }
    if(ui->pushButton_7) {
        ui->pushButton_7->setGeometry(1060, 160, 120, 31);
        ui->pushButton_7->setText("Certificats PDF");
    }

    // Table View
    if(ui->tableWidget) {
        ui->tableWidget->setGeometry(260, 230, 1050, 490);
        ui->tableWidget->setStyleSheet(
            "QTableWidget { background: white; border: 2px solid #c9b8a5; border-radius: 16px; gridline-color: #f5eee6; selection-background-color: #f5eee6; selection-color: #3a1f14; }"
        );
        ui->tableWidget->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5b3020, stop:0.5 #4a2517, stop:1 #3a1a10);"
            "    color: #f5efe8; padding: 10px; border: none; font-weight: bold; font-size: 11px; letter-spacing: 1px;"
            "}"
        );
    }

    // CRUD Buttons (bottom)
    if(ui->pushButton) ui->pushButton->setGeometry(260, 730, 150, 40);
    if(ui->pushButton_2) ui->pushButton_2->setGeometry(420, 730, 150, 40);
    if(ui->pushButton_3) ui->pushButton_3->setGeometry(580, 730, 150, 40);
    if(ui->pushButton_4) ui->pushButton_4->setGeometry(740, 730, 150, 40);

    setupTable();
    loadEmployeesTable();

    connect(ui->searchIdEdit,  &QLineEdit::textChanged, this, &pageemployee::applyFilter);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pageemployee::applyFilter);
    
    sidebar->raise();
    if(ui->tableWidget) ui->tableWidget->raise();
}


pageemployee::~pageemployee()
{
    delete ui;
}

void pageemployee::navToCommandes() { hide(); (new commandes(m_idEmployeConnecte, this))->show(); }
void pageemployee::navToFournisseurs() { hide(); (new fournisseurs(m_idEmployeConnecte, this))->show(); }
void pageemployee::navToProduits() { hide(); (new produitswindow(m_idEmployeConnecte, this))->show(); }
void pageemployee::navToMatieres() { hide(); (new Matieres(m_idEmployeConnecte, this))->show(); }
void pageemployee::navToMachines() { hide(); (new pagemachine(m_idEmployeConnecte, this))->show(); }
void pageemployee::navToLogout() { hide(); (new login())->show(); }

void pageemployee::on_pushButton_6_clicked() { navToCommandes(); }
void pageemployee::on_pushButton_11_clicked() { on_actualiser_clicked(); }
void pageemployee::on_pushButton_12_clicked() { navToFournisseurs(); }
void pageemployee::on_pushButton_21_clicked() { navToProduits(); }
void pageemployee::on_pushButton_22_clicked() { navToMatieres(); }
void pageemployee::on_pushButton_23_clicked() { navToMachines(); }
void pageemployee::on_pushButton_5_clicked() { navToLogout(); }
void pageemployee::on_pushButton_20_clicked() { navToFournisseurs(); }

// ... [Keep getCinEmployeConnecte and on_btnSaveFacePhoto_clicked as is] ...

void pageemployee::setupTable()
{
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels({
        "ID", "CIN", "Nom", "Prénom", "Naissance",
        "Poste", "Niveau", "Salaire (DT)", "Email", "MDP", "Status"
    });
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnHidden(9, true);
}

void pageemployee::loadEmployeesTable(const QString &whereClause, const QVariantList &binds)
{
    ui->tableWidget->setRowCount(0);
    QString sql = "SELECT ID_EMPLOYE, CIN, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY'), POSTE, NIVEAU, SALAIRE, EMAIL, PASSWORD, STATUS FROM SMARTLEATHER.EMPLOYE";
    if (!whereClause.isEmpty()) sql += " WHERE " + whereClause;
    sql += " ORDER BY NOM ASC";

    QSqlQuery q; q.prepare(sql);
    for (int i=0; i<binds.size(); ++i) q.bindValue(i, binds[i]);
    if (q.exec()) {
        int r=0;
        while(q.next()){
            ui->tableWidget->insertRow(r);
            for(int c=0; c<11; ++c) ui->tableWidget->setItem(r, c, new QTableWidgetItem(q.value(c).toString()));
            r++;
        }
    }
}

void pageemployee::applyFilter()
{
    QString email = ui->searchIdEdit->text().trimmed();
    QString nom = ui->searchNomEdit->text().trimmed();
    QStringList cond; QVariantList binds;
    if(!email.isEmpty()) { cond << "UPPER(EMAIL) LIKE UPPER(?)"; binds << "%"+email+"%"; }
    if(!nom.isEmpty()) { cond << "UPPER(NOM) LIKE UPPER(?)"; binds << "%"+nom+"%"; }
    if(cond.isEmpty()) loadEmployeesTable();
    else loadEmployeesTable(cond.join(" AND "), binds);
}

void pageemployee::on_search_clicked() { applyFilter(); }
void pageemployee::on_actualiser_clicked() { ui->searchIdEdit->clear(); ui->searchNomEdit->clear(); loadEmployeesTable(); }

// ═══════════════════════════════════════════════
//   AJOUTER EMPLOYÉ
// ═══════════════════════════════════════════════
void pageemployee::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvel Employé");
    dialog.setFixedSize(500, 780);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(35, 25, 35, 20);
    mainLayout->setSpacing(5);

    QLabel *header = new QLabel("✦ NOUVEL EMPLOYÉ");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *cinEdit = new QLineEdit(); cinEdit->setPlaceholderText("CIN (8 chiffres)");
    QLabel *cinErr = new QLabel(); cinErr->setObjectName("errorLabel"); cinErr->hide();
    
    QLineEdit *nomEdit = new QLineEdit(); nomEdit->setPlaceholderText("Nom de famille");
    QLineEdit *prenomEdit = new QLineEdit(); prenomEdit->setPlaceholderText("Prénom");

    QDateEdit *dateNaiss = new QDateEdit(QDate::currentDate().addYears(-20));
    dateNaiss->setCalendarPopup(true);

    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur","Responsable Production","Opérateur","Comptable","Commercial","Magasinier","Stagiaire","Ressource Humaine","Responsable Stock","Service Achat","Service Technique","Service Client"});

    QComboBox *niveauCombo = new QComboBox();
    niveauCombo->addItems({"BAC","LICENCE","MASTER"});

    QDoubleSpinBox *salaireSpin = new QDoubleSpinBox();
    salaireSpin->setRange(0, 99999); salaireSpin->setSuffix(" DT");

    QLineEdit *emailEdit = new QLineEdit(); emailEdit->setPlaceholderText("email@exemple.com");
    QLineEdit *passEdit = new QLineEdit(); passEdit->setEchoMode(QLineEdit::Password);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Actif", "En congé", "Suspendu", "En formation", "Maladie", "Mission", "Démissionné"});

    mainLayout->addWidget(new QLabel("IDENTITÉ (CIN)")); mainLayout->addWidget(cinEdit); mainLayout->addWidget(cinErr);
    
    QHBoxLayout *h1 = new QHBoxLayout();
    QVBoxLayout *h1a = new QVBoxLayout(); h1a->addWidget(new QLabel("NOM")); h1a->addWidget(nomEdit);
    QVBoxLayout *h1b = new QVBoxLayout(); h1b->addWidget(new QLabel("PRÉNOM")); h1b->addWidget(prenomEdit);
    h1->addLayout(h1a); h1->addLayout(h1b); mainLayout->addLayout(h1);

    mainLayout->addWidget(new QLabel("DATE DE NAISSANCE")); mainLayout->addWidget(dateNaiss);
    mainLayout->addWidget(new QLabel("POSTE OCCUPÉ")); mainLayout->addWidget(posteCombo);

    QHBoxLayout *h2 = new QHBoxLayout();
    QVBoxLayout *h2a = new QVBoxLayout(); h2a->addWidget(new QLabel("NIVEAU")); h2a->addWidget(niveauCombo);
    QVBoxLayout *h2b = new QVBoxLayout(); h2b->addWidget(new QLabel("SALAIRE")); h2b->addWidget(salaireSpin);
    h2->addLayout(h2a); h2->addLayout(h2b); mainLayout->addLayout(h2);

    mainLayout->addWidget(new QLabel("CONTACT (EMAIL)")); mainLayout->addWidget(emailEdit);
    mainLayout->addWidget(new QLabel("MOT DE PASSE")); mainLayout->addWidget(passEdit);
    mainLayout->addWidget(new QLabel("STATUT")); mainLayout->addWidget(statusCombo);

    mainLayout->addSpacing(15);
    mainLayout->addWidget(createSeparator());

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  ENREGISTRER  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btns->addWidget(btnSave); btns->addWidget(btnCancel);
    mainLayout->addLayout(btns);

    auto validate = [&]() {
        bool ok = true;
        if(cinEdit->text().trimmed().length() != 8) { setFieldError(cinEdit, cinErr, true, "CIN doit avoir 8 chiffres"); ok = false; } else setFieldError(cinEdit, cinErr, false);
        btnSave->setEnabled(ok); return ok;
    };
    btnSave->setEnabled(false); connect(cinEdit, &QLineEdit::textChanged, validate);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery q;
        q.prepare("INSERT INTO SMARTLEATHER.EMPLOYE (ID_EMPLOYE, CIN, NOM, PRENOM, DATE_NAISSANCE, POSTE, NIVEAU, SALAIRE, EMAIL, PASSWORD, STATUS) "
                  "VALUES (SMARTLEATHER.SEQ_EMPLOYE.NEXTVAL, :cin, :nom, :prenom, :dn, :poste, :niv, :sal, :email, :pass, :st)");
        q.bindValue(":cin", cinEdit->text().trimmed());
        q.bindValue(":nom", nomEdit->text().trimmed().toUpper());
        q.bindValue(":prenom", prenomEdit->text().trimmed());
        q.bindValue(":dn", dateNaiss->date());
        q.bindValue(":poste", posteCombo->currentText());
        q.bindValue(":niv", niveauCombo->currentText());
        q.bindValue(":sal", salaireSpin->value());
        q.bindValue(":email", emailEdit->text().trimmed());
        q.bindValue(":pass", passEdit->text().trimmed());
        q.bindValue(":st", statusCombo->currentText());

        if (q.exec()) { QMessageBox::information(this, "Succès", "Employé ajouté !"); loadEmployeesTable(); }
        else { QMessageBox::critical(this, "Erreur", q.lastError().text()); }
    }
}

// ═══════════════════════════════════════════════
//   MODIFIER EMPLOYÉ
// ═══════════════════════════════════════════════
void pageemployee::on_pushButton_2_clicked()
{
    int id = currentEmployeeId();
    if (id < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé."); return; }

    int r = ui->tableWidget->currentRow();
    QString cin = ui->tableWidget->item(r, 1)->text();
    QString nom = ui->tableWidget->item(r, 2)->text();
    QString pre = ui->tableWidget->item(r, 3)->text();
    QDate dn = QDate::fromString(ui->tableWidget->item(r, 4)->text(), "dd/MM/yyyy");
    QString pos = ui->tableWidget->item(r, 5)->text();
    QString niv = ui->tableWidget->item(r, 6)->text();
    double sal = ui->tableWidget->item(r, 7)->text().toDouble();
    QString em = ui->tableWidget->item(r, 8)->text();
    QString st = ui->tableWidget->item(r, 10)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier Employé");
    dialog.setFixedSize(500, 780);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(35, 25, 35, 20);

    QLabel *header = new QLabel("✦ MODIFIER EMPLOYÉ");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *cinEdit = new QLineEdit(cin);
    QLineEdit *nomEdit = new QLineEdit(nom);
    QLineEdit *preEdit = new QLineEdit(pre);
    QDateEdit *dnEdit = new QDateEdit(dn); dnEdit->setCalendarPopup(true);
    QComboBox *posCombo = new QComboBox(); 
    posCombo->addItems({"Directeur","Responsable Production","Opérateur","Comptable","Commercial","Magasinier","Stagiaire","Ressource Humaine","Responsable Stock","Service Achat","Service Technique","Service Client"});
    posCombo->setCurrentText(pos);
    QComboBox *nivCombo = new QComboBox(); nivCombo->addItems({"BAC","LICENCE","MASTER"}); nivCombo->setCurrentText(niv);
    QDoubleSpinBox *salSpin = new QDoubleSpinBox(); salSpin->setRange(0, 99999); salSpin->setValue(sal);
    QLineEdit *emEdit = new QLineEdit(em);
    QComboBox *stCombo = new QComboBox(); 
    stCombo->addItems({"Actif", "En congé", "Suspendu", "En formation", "Maladie", "Mission", "Démissionné"}); 
    stCombo->setCurrentText(st);

    mainLayout->addWidget(new QLabel("CIN")); mainLayout->addWidget(cinEdit);
    mainLayout->addWidget(new QLabel("NOM")); mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("PRÉNOM")); mainLayout->addWidget(preEdit);
    mainLayout->addWidget(new QLabel("DATE DE NAISSANCE")); mainLayout->addWidget(dnEdit);
    mainLayout->addWidget(new QLabel("POSTE")); mainLayout->addWidget(posCombo);
    mainLayout->addWidget(new QLabel("NIVEAU")); mainLayout->addWidget(nivCombo);
    mainLayout->addWidget(new QLabel("SALAIRE")); mainLayout->addWidget(salSpin);
    mainLayout->addWidget(new QLabel("EMAIL")); mainLayout->addWidget(emEdit);
    mainLayout->addWidget(new QLabel("STATUT")); mainLayout->addWidget(stCombo);

    mainLayout->addSpacing(15);
    mainLayout->addWidget(createSeparator());

    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  Mettre à jour  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btns->addWidget(btnSave); btns->addWidget(btnCancel);
    mainLayout->addLayout(btns);

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery q;
        q.prepare("UPDATE SMARTLEATHER.EMPLOYE SET CIN=:cin, NOM=:nom, PRENOM=:pre, DATE_NAISSANCE=:dn, POSTE=:pos, NIVEAU=:niv, SALAIRE=:sal, EMAIL=:em, STATUS=:st WHERE ID_EMPLOYE=:id");
        q.bindValue(":cin", cinEdit->text().trimmed());
        q.bindValue(":nom", nomEdit->text().trimmed().toUpper());
        q.bindValue(":pre", preEdit->text().trimmed());
        q.bindValue(":dn", dnEdit->date());
        q.bindValue(":pos", posCombo->currentText());
        q.bindValue(":niv", nivCombo->currentText());
        q.bindValue(":sal", salSpin->value());
        q.bindValue(":em", emEdit->text().trimmed());
        q.bindValue(":st", stCombo->currentText());
        q.bindValue(":id", id);

        if (q.exec()) { QMessageBox::information(this, "Succès", "Données mises à jour !"); loadEmployeesTable(); }
        else { QMessageBox::critical(this, "Erreur", q.lastError().text()); }
    }
}

// ═══════════════════════════════════════════════
//   SUPPRIMER EMPLOYÉ
// ═══════════════════════════════════════════════
void pageemployee::on_pushButton_3_clicked()
{
    int id = currentEmployeeId();
    if (id < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé."); return; }

    auto reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment licencier cet employé ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery q; q.prepare("DELETE FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
        q.bindValue(":id", id);
        if (q.exec()) { QMessageBox::information(this, "Succès", "Employé supprimé."); loadEmployeesTable(); }
        else { QMessageBox::critical(this, "Erreur", q.lastError().text()); }
    }
}

// ═══════════════════════════════════════════════
//   EXPORT PDF (DOMMAGE / CONGÉ)
// ═══════════════════════════════════════════════
void pageemployee::on_pushButton_7_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez un employé."); return; }

    QString cin = ui->tableWidget->item(row, 1)->text();
    QString nom = ui->tableWidget->item(row, 2)->text();
    QString pre = ui->tableWidget->item(row, 3)->text();
    QString pos = ui->tableWidget->item(row, 5)->text();

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "Demande_Conge_" + nom + ".pdf", "*.pdf");
    if (fileName.isEmpty()) return;

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    QPainter painter(&pdf);

    painter.setPen(QPen(QColor("#3a1f14"), 8));
    painter.drawRect(100, 100, 9300, 13800);

    painter.setFont(QFont("Times", 24, QFont::Bold));
    painter.drawText(QRect(0, 1000, 9500, 200), Qt::AlignCenter, "LUXURY LEATHER - RH");
    
    painter.setFont(QFont("Arial", 14));
    painter.drawText(500, 2500, "Objet : Demande de congé / Certificat de travail");
    painter.drawText(500, 3200, "Employé : " + nom + " " + pre);
    painter.drawText(500, 3600, "CIN : " + cin);
    painter.drawText(500, 4000, "Poste : " + pos);

    painter.drawText(500, 5500, "Je soussigné, la direction de LUXURY LEATHER, certifie que l'employé");
    painter.drawText(500, 5900, "susnommé fait partie de nos effectifs premium.");

    painter.drawText(6000, 12000, "Fait à Tunis, le " + QDate::currentDate().toString());
    painter.end();
    QMessageBox::information(this, "PDF", "Document généré avec succès.");
}

// ═══════════════════════════════════════════════
//   STATISTIQUES (LUXURY STYLE)
// ═══════════════════════════════════════════════
void pageemployee::on_pushButton_9_clicked()
{
    int total = ui->tableWidget->rowCount();
    if (total == 0) return;

    QMap<QString, int> stats;
    for(int i=0; i<total; ++i) stats[ui->tableWidget->item(i, 10)->text()]++;

    QDialog dlg(this);
    dlg.setWindowTitle("Dashboard RH");
    dlg.setFixedSize(800, 600);
    dlg.setStyleSheet("QDialog { background: #fdfaf7; }");

    QVBoxLayout *main = new QVBoxLayout(&dlg);
    QLabel *title = new QLabel("RÉPARTITION DES EFFECTIFS");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #3a1f14;");
    title->setAlignment(Qt::AlignCenter);
    main->addWidget(title);

    QPieSeries *series = new QPieSeries();
    for(auto it = stats.begin(); it != stats.end(); ++it) {
        QPieSlice *slice = series->append(it.key(), it.value());
        slice->setLabelVisible(true);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statuts des employés");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundVisible(false);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    main->addWidget(view);

    QPushButton *btnClose = new QPushButton("FERMER");
    btnClose->setStyleSheet("background: #3a1f14; color: white; padding: 10px; border-radius: 8px;");
    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);
    main->addWidget(btnClose);

    dlg.exec();
}

void pageemployee::on_pushButton_4_clicked() { hide(); (new pagechat(m_idEmployeConnecte, this))->show(); }
void pageemployee::on_pushButton_8_clicked() { navToLogout(); }
void pageemployee::on_btnSaveFacePhoto_clicked() { QMessageBox::information(this, "Photo Facial", "Fonctionnalité d'enregistrement facial (Placeholder)."); }

int pageemployee::currentEmployeeId() const {
    int r = ui->tableWidget->currentRow();
    if (r < 0) return -1;
    return ui->tableWidget->item(r, 0)->text().toInt();
}

QString pageemployee::getCinEmployeConnecte() const {
    QSqlQuery q; q.prepare("SELECT CIN FROM SMARTLEATHER.EMPLOYE WHERE ID_EMPLOYE = :id");
    q.bindValue(":id", m_idEmployeConnecte);
    if (q.exec() && q.next()) return q.value(0).toString();
    return "";
}
