#include "pageemployee.h"
#include "ui_pageemployee.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
#include <QDateEdit>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "commandes.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

pageemployee::pageemployee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pageemployee)
{
    ui->setupUi(this);
    model = new CustomEmployeeModel(this);
    setupEmployeesTable();
    loadEmployees();
    setupSearch();
}

pageemployee::~pageemployee()
{
    delete ui;
}

void pageemployee::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pageemployee::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pageemployee::filterTable);
}

void pageemployee::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString nomFilter = ui->searchNomEdit->text();

    QString sql = "SELECT ID_EMPLOYEE, NOM, PRENOM, POSTE, SALAIRE, TO_CHAR(DATE_DE_NAISSANCE, 'DD/MM/YYYY'), NIVEAU "
                  "FROM EMPLOYEE WHERE 1=1";
    
    if (!idFilter.isEmpty()) sql += " AND LOWER(ID_EMPLOYEE) LIKE LOWER('%" + idFilter + "%')";
    if (!nomFilter.isEmpty()) sql += " AND LOWER(NOM) LIKE LOWER('%" + nomFilter + "%')";

    model->setQuery(sql);
}

void pageemployee::setupEmployeesTable()
{
    ui->tableWidget->setModel(model);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Poste");
    model->setHeaderData(4, Qt::Horizontal, "Salaire");
    model->setHeaderData(5, Qt::Horizontal, "Embauche");
    model->setHeaderData(6, Qt::Horizontal, "Statut");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 110);
    ui->tableWidget->setColumnWidth(2, 110);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 110);
    ui->tableWidget->setColumnWidth(6, 100);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void pageemployee::loadEmployees()
{
    model->setQuery("SELECT ID_EMPLOYEE, NOM, PRENOM, POSTE, SALAIRE, TO_CHAR(DATE_DE_NAISSANCE, 'DD/MM/YYYY'), NIVEAU FROM EMPLOYEE");
    if (model->lastError().isValid()) {
        qDebug() << "Erreur Oracle:" << model->lastError().text();
    }
}

// Navigation
void pageemployee::on_pushButton_6_clicked() { hide(); (new commandes(this))->show(); }
void pageemployee::on_pushButton_5_clicked() { hide(); (new login(this))->show(); }
void pageemployee::on_pushButton_20_clicked() { hide(); (new fournisseurs(this))->show(); }
void pageemployee::on_pushButton_21_clicked() { hide(); (new products(this))->show(); }
void pageemployee::on_pushButton_22_clicked() { hide(); (new Matieres(this))->show(); }
void pageemployee::on_pushButton_23_clicked() { hide(); (new pagemachine(this))->show(); }

void pageemployee::on_pushButton_4_clicked() { ui->searchIdEdit->clear(); ui->searchNomEdit->clear(); loadEmployees(); }

void pageemployee::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouvel Employé");
    ajoutDialog->setFixedSize(650, 850);
    ajoutDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *layout = new QVBoxLayout(ajoutDialog);
    layout->setContentsMargins(40,40,40,40); layout->setSpacing(12);

    QString nextId = "E001";
    QSqlQuery idQ("SELECT MAX(ID_EMPLOYEE) FROM EMPLOYEE");
    if(idQ.next() && !idQ.value(0).isNull()){
        QString last = idQ.value(0).toString();
        if(last.startsWith("E")) nextId = QString("E%1").arg(last.mid(1).toInt() + 1, 3, 10, QChar('0'));
    }

    QLineEdit *idEdit = new QLineEdit(nextId); idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; font-weight: bold; padding: 10px; border-radius: 8px;");
    
    QLineEdit *nomEdit = new QLineEdit(); nomEdit->setPlaceholderText("Nom");
    QLineEdit *prenomEdit = new QLineEdit(); prenomEdit->setPlaceholderText("Prénom");
    
    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur", "Responsable", "Opérateur", "Comptable", "Commercial", "Magasinier"});

    QLineEdit *salaireEdit = new QLineEdit(); salaireEdit->setPlaceholderText("Salaire (ex: 2500)");

    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy"); dateEdit->setCalendarPopup(true);

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Congé", "Formation", "Inactif"});

    layout->addWidget(new QLabel("ID AUTOMATIQUE:")); layout->addWidget(idEdit);
    layout->addWidget(new QLabel("NOM:")); layout->addWidget(nomEdit);
    layout->addWidget(new QLabel("PRÉNOM:")); layout->addWidget(prenomEdit);
    layout->addWidget(new QLabel("POSTE:")); layout->addWidget(posteCombo);
    layout->addWidget(new QLabel("SALAIRE:")); layout->addWidget(salaireEdit);
    layout->addWidget(new QLabel("DATE EMBAUCHE:")); layout->addWidget(dateEdit);
    layout->addWidget(new QLabel("STATUT:")); layout->addWidget(statutCombo);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    layout->addWidget(btnSave);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };

        QRegularExpression nameRx("^[A-Za-zÀ-ÿ\\s\\-']{2,30}$");
        check(nomEdit, nameRx.match(nomEdit->text()).hasMatch());
        check(prenomEdit, nameRx.match(prenomEdit->text()).hasMatch());
        
        bool okS; double s = salaireEdit->text().toDouble(&okS);
        check(salaireEdit, okS && s > 0);

        btnSave->setEnabled(allValid);
    };

    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(prenomEdit, &QLineEdit::textChanged, validateForm);
    connect(salaireEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("INSERT INTO EMPLOYEE (ID_EMPLOYEE, NOM, PRENOM, POSTE, SALAIRE, DATE_DE_NAISSANCE, NIVEAU) "
                  "VALUES (:id, :n, :p, :po, :s, TO_DATE(:d, 'DD/MM/YYYY'), :st)");
        q.bindValue(":id", idEdit->text());
        q.bindValue(":n", nomEdit->text());
        q.bindValue(":p", prenomEdit->text());
        q.bindValue(":po", posteCombo->currentText());
        q.bindValue(":s", salaireEdit->text().toDouble());
        q.bindValue(":d", dateEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":st", statutCombo->currentText());

        if(q.exec()){
            QMessageBox::information(ajoutDialog, "Succès", "Employé ajouté.");
            loadEmployees(); ajoutDialog->accept();
        } else QMessageBox::critical(ajoutDialog, "Erreur", q.lastError().text());
    });
    ajoutDialog->exec();
}

void pageemployee::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez un employé."); return; }

    QString id = model->index(row, 0).data().toString();

    QDialog *editDialog = new QDialog(this);
    editDialog->setWindowTitle("Modifier Employé");
    editDialog->setFixedSize(650, 850);
    editDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *layout = new QVBoxLayout(editDialog);
    layout->setContentsMargins(40,40,40,40); layout->setSpacing(12);

    QLineEdit *nomEdit = new QLineEdit(model->index(row, 1).data().toString());
    QLineEdit *prenomEdit = new QLineEdit(model->index(row, 2).data().toString());
    QComboBox *posteCombo = new QComboBox();
    posteCombo->addItems({"Directeur", "Responsable", "Opérateur", "Comptable", "Commercial", "Magasinier"});
    posteCombo->setCurrentText(model->index(row, 3).data().toString());

    QLineEdit *salaireEdit = new QLineEdit(model->index(row, 4).data().toString().replace(" €", ""));
    QDateEdit *dateEdit = new QDateEdit(QDate::fromString(model->index(row, 5).data().toString(), "dd/MM/yyyy"));
    dateEdit->setDisplayFormat("dd/MM/yyyy"); dateEdit->setCalendarPopup(true);

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Congé", "Formation", "Inactif"});
    statutCombo->setCurrentText(model->index(row, 6).data().toString());

    layout->addWidget(new QLabel("NOM:")); layout->addWidget(nomEdit);
    layout->addWidget(new QLabel("PRÉNOM:")); layout->addWidget(prenomEdit);
    layout->addWidget(new QLabel("POSTE:")); layout->addWidget(posteCombo);
    layout->addWidget(new QLabel("SALAIRE:")); layout->addWidget(salaireEdit);
    layout->addWidget(new QLabel("DATE EMBAUCHE:")); layout->addWidget(dateEdit);
    layout->addWidget(new QLabel("STATUT:")); layout->addWidget(statutCombo);

    QPushButton *btnUpdate = new QPushButton("💾 Mettre à jour");
    btnUpdate->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    layout->addWidget(btnUpdate);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };
        QRegularExpression nameRx("^[A-Za-zÀ-ÿ\\s\\-']{2,30}$");
        check(nomEdit, nameRx.match(nomEdit->text()).hasMatch());
        check(prenomEdit, nameRx.match(prenomEdit->text()).hasMatch());
        bool okS; double s = salaireEdit->text().toDouble(&okS);
        check(salaireEdit, okS && s > 0);
        btnUpdate->setEnabled(allValid);
    };

    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(prenomEdit, &QLineEdit::textChanged, validateForm);
    connect(salaireEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnUpdate, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("UPDATE EMPLOYEE SET NOM=:n, PRENOM=:p, POSTE=:po, SALAIRE=:s, "
                  "DATE_DE_NAISSANCE=TO_DATE(:d, 'DD/MM/YYYY'), NIVEAU=:st WHERE ID_EMPLOYEE=:id");
        q.bindValue(":n", nomEdit->text()); q.bindValue(":p", prenomEdit->text());
        q.bindValue(":po", posteCombo->currentText()); q.bindValue(":s", salaireEdit->text().toDouble());
        q.bindValue(":d", dateEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":st", statutCombo->currentText()); q.bindValue(":id", id);

        if(q.exec()){
            QMessageBox::information(editDialog, "Succès", "Profil mis à jour.");
            loadEmployees(); editDialog->accept();
        } else QMessageBox::critical(editDialog, "Erreur", q.lastError().text());
    });
    editDialog->exec();
}

void pageemployee::on_pushButton_3_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) return;
    QString id = model->index(row, 0).data().toString();
    if(QMessageBox::question(this, "Supprimer", "Supprimer cet employé ?") == QMessageBox::Yes){
        QSqlQuery q; q.prepare("DELETE FROM EMPLOYEE WHERE ID_EMPLOYEE = :id");
        q.bindValue(":id", id);
        if(q.exec()) loadEmployees();
    }
}

void pageemployee::on_pushButton_7_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Export", "employes.csv", "CSV (*.csv)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::WriteOnly)){
        QTextStream out(&f);
        out << "ID;Nom;Prenom;Poste;Salaire;Embauche;Statut\n";
        for(int r=0; r<model->rowCount(); r++){
            QStringList l;
            for(int c=0; c<model->columnCount(); c++) l << model->index(r,c).data().toString();
            out << l.join(";") << "\n";
        }
        QMessageBox::information(this, "Export", "Export CSV réussi.");
    }
}

void pageemployee::on_pushButton_9_clicked()
{
    int act=0, cong=0, form=0, inac=0;
    double tSal=0;
    QSqlQuery q("SELECT NIVEAU, COUNT(*), SUM(SALAIRE) FROM EMPLOYEE GROUP BY NIVEAU");
    while(q.next()){
        QString s = q.value(0).toString(); int c = q.value(1).toInt();
        if(s=="Actif") act=c; else if(s=="Congé") cong=c; else if(s=="Formation") form=c; else inac=c;
        tSal += q.value(2).toDouble();
    }
    int tot = act+cong+form+inac; if(tot==0) return;

    QDialog *d = new QDialog(this); d->setWindowTitle("Stats"); d->resize(800,600);
    d->setStyleSheet("QDialog { background-color: #f1e7dc; border-radius: 15px; }");
    QVBoxLayout *v = new QVBoxLayout(d);
    
    QHBoxLayout *h = new QHBoxLayout();
    struct SD { QString n; int c; QColor cl; };
    QList<SD> sds = {{"Actif",act,QColor(100,200,100)}, {"Congé",cong,QColor(255,150,150)}, {"Form.",form,QColor(255,255,100)}, {"Inactif",inac,QColor(200,200,200)}};

    for(const auto &s : sds){
        if(s.c==0) continue;
        QVBoxLayout *bc = new QVBoxLayout();
        QFrame *bar = new QFrame(); bar->setFixedWidth(80); bar->setFixedHeight(s.c * 250 / tot);
        bar->setStyleSheet(QString("background: %1; border-radius: 8px; border: 1px solid #3a1f14;").arg(s.cl.name()));
        bc->addWidget(new QLabel(QString::number(s.c)), 0, Qt::AlignCenter);
        bc->addWidget(bar, 0, Qt::AlignCenter);
        bc->addWidget(new QLabel(s.n), 0, Qt::AlignCenter);
        h->addLayout(bc);
    }
    v->addLayout(h);
    v->addWidget(new QLabel(QString("💰 Masse salariale: %1 €").arg(tSal, 0, 'f', 2)), 0, Qt::AlignCenter);
    QPushButton *cl = new QPushButton("Fermer"); cl->setStyleSheet("background: #6b3e26; color: white; padding: 10px; border-radius: 10px;");
    connect(cl, &QPushButton::clicked, d, &QDialog::accept); v->addWidget(cl, 0, Qt::AlignCenter);
    d->exec();
}
