#include "fournisseurs.h"
#include "ui_fournisseurs.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDateEdit>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

#include "commandes.h"
#include "pageemployee.h"
#include "login.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"

fournisseurs::fournisseurs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::fournisseurs)
{
    ui->setupUi(this);
    model = new CustomFournisseurModel(this);
    setupFournisseursTable();
    loadFournisseurs();
    setupSearch();
}

fournisseurs::~fournisseurs()
{
    delete ui;
}

void fournisseurs::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &fournisseurs::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &fournisseurs::filterTable);
}

void fournisseurs::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString nomFilter = ui->searchNomEdit->text();

    QString sql = "SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT FROM FOURNISSEUR WHERE 1=1";
    if (!idFilter.isEmpty()) sql += " AND LOWER(ID_FOURNISSEUR) LIKE LOWER('%" + idFilter + "%')";
    if (!nomFilter.isEmpty()) sql += " AND LOWER(NOM) LIKE LOWER('%" + nomFilter + "%')";

    model->setQuery(sql);
}

void fournisseurs::setupFournisseursTable()
{
    ui->tableWidget->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Type Matière");
    model->setHeaderData(3, Qt::Horizontal, "Téléphone");
    model->setHeaderData(4, Qt::Horizontal, "Adresse");
    model->setHeaderData(5, Qt::Horizontal, "Délai");
    model->setHeaderData(6, Qt::Horizontal, "Qualité");
    model->setHeaderData(7, Qt::Horizontal, "Statut");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 120);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(4, 200);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 90);
    ui->tableWidget->setColumnWidth(7, 85);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void fournisseurs::loadFournisseurs()
{
    model->setQuery("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT FROM FOURNISSEUR");
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur Oracle", model->lastError().text());
    }
}

// Navigation Slots
void fournisseurs::on_pushButton_16_clicked() { hide(); (new commandes(this))->show(); }
void fournisseurs::on_pushButton_17_clicked() { hide(); (new pageemployee(this))->show(); }
void fournisseurs::on_pushButton_15_clicked() { hide(); (new login(this))->show(); }
void fournisseurs::on_pushButton_20_clicked() { hide(); (new fournisseurs(this))->show(); }
void fournisseurs::on_pushButton_21_clicked() { hide(); (new products(this))->show(); }
void fournisseurs::on_pushButton_22_clicked() { hide(); (new Matieres(this))->show(); }
void fournisseurs::on_pushButton_23_clicked() { hide(); (new pagemachine(this))->show(); }

void fournisseurs::on_pushButton_4_clicked() { loadFournisseurs(); }

void fournisseurs::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouveau Fournisseur");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *layout = new QVBoxLayout(ajoutDialog);
    layout->setContentsMargins(40,40,40,40);
    layout->setSpacing(15);

    QString nextId = "FR001";
    QSqlQuery idQ("SELECT MAX(ID_FOURNISSEUR) FROM FOURNISSEUR");
    if(idQ.next() && !idQ.value(0).isNull()){
        QString last = idQ.value(0).toString();
        nextId = QString("FR%1").arg(last.mid(2).toInt() + 1, 3, 10, QChar('0'));
    }

    QLineEdit *idEdit = new QLineEdit(nextId);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; font-weight: bold;");
    
    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom:");
    
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir", "Tissu", "Accessoires", "Emballage"});

    QLineEdit *phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Téléphone:");

    QLineEdit *addrEdit = new QLineEdit();
    addrEdit->setPlaceholderText("Adresse:");

    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate().addDays(7));
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(true);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Inactif"});

    layout->addWidget(new QLabel("ID AUTOMATIQUE:")); layout->addWidget(idEdit);
    layout->addWidget(new QLabel("NOM:")); layout->addWidget(nomEdit);
    layout->addWidget(new QLabel("TYPE:")); layout->addWidget(typeCombo);
    layout->addWidget(new QLabel("TÉLÉPHONE:")); layout->addWidget(phoneEdit);
    layout->addWidget(new QLabel("ADRESSE:")); layout->addWidget(addrEdit);
    layout->addWidget(new QLabel("DÉLAI ESTIMÉ:")); layout->addWidget(dateEdit);
    layout->addWidget(new QLabel("QUALITÉ:")); layout->addWidget(qualiteCombo);
    layout->addWidget(new QLabel("STATUT:")); layout->addWidget(statutCombo);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold;");
    layout->addWidget(btnSave);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;"); allValid = false; }
        };

        check(nomEdit, nomEdit->text().trimmed().length() >= 2);
        
        QRegularExpression phoneRx("^[0-9]{8,12}$");
        check(phoneEdit, phoneRx.match(phoneEdit->text()).hasMatch());
        
        check(addrEdit, addrEdit->text().trimmed().length() >= 5);

        btnSave->setEnabled(allValid);
    };

    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(phoneEdit, &QLineEdit::textChanged, validateForm);
    connect(addrEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("INSERT INTO FOURNISSEUR (ID_FOURNISSEUR, NOM, TYPE_MATIERE, TELEPHONE, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT) VALUES (:id, :nom, :type, :tel, :addr, :delai, :qual, :stat)");
        q.bindValue(":id", idEdit->text());
        q.bindValue(":nom", nomEdit->text());
        q.bindValue(":type", typeCombo->currentText());
        q.bindValue(":tel", phoneEdit->text());
        q.bindValue(":addr", addrEdit->text());
        q.bindValue(":delai", dateEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":qual", qualiteCombo->currentText());
        q.bindValue(":stat", statutCombo->currentText());

        if(q.exec()){
            QMessageBox::information(ajoutDialog, "Succès", "Fournisseur ajouté.");
            loadFournisseurs();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", q.lastError().text());
        }
    });

    ajoutDialog->exec();
}

void fournisseurs::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez un fournisseur."); return; }

    QString id = model->index(row, 0).data().toString();
    
    QDialog *editDialog = new QDialog(this);
    editDialog->setWindowTitle("Modifier Fournisseur");
    editDialog->setFixedSize(650, 800);
    editDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *layout = new QVBoxLayout(editDialog);
    layout->setContentsMargins(40,40,40,40);
    layout->setSpacing(15);

    QLineEdit *nomEdit = new QLineEdit(model->index(row, 1).data().toString());
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir", "Tissu", "Accessoires", "Emballage"});
    typeCombo->setCurrentText(model->index(row, 2).data().toString());

    QLineEdit *phoneEdit = new QLineEdit(model->index(row, 3).data().toString());
    QLineEdit *addrEdit = new QLineEdit(model->index(row, 4).data().toString());

    QDateEdit *dateEdit = new QDateEdit(QDate::fromString(model->index(row, 5).data().toString(), "dd/MM/yyyy"));
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(true);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    qualiteCombo->setCurrentText(model->index(row, 6).data().toString());

    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Inactif"});
    statutCombo->setCurrentText(model->index(row, 7).data().toString());

    layout->addWidget(new QLabel("NOM:")); layout->addWidget(nomEdit);
    layout->addWidget(new QLabel("TYPE:")); layout->addWidget(typeCombo);
    layout->addWidget(new QLabel("TÉLÉPHONE:")); layout->addWidget(phoneEdit);
    layout->addWidget(new QLabel("ADRESSE:")); layout->addWidget(addrEdit);
    layout->addWidget(new QLabel("DÉLAI ESTIMÉ:")); layout->addWidget(dateEdit);
    layout->addWidget(new QLabel("QUALITÉ:")); layout->addWidget(qualiteCombo);
    layout->addWidget(new QLabel("STATUT:")); layout->addWidget(statutCombo);

    QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold;");
    layout->addWidget(btnSave);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;"); allValid = false; }
        };
        check(nomEdit, nomEdit->text().trimmed().length() >= 2);
        QRegularExpression phoneRx("^[0-9]{8,12}$");
        check(phoneEdit, phoneRx.match(phoneEdit->text()).hasMatch());
        check(addrEdit, addrEdit->text().trimmed().length() >= 5);
        btnSave->setEnabled(allValid);
    };

    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(phoneEdit, &QLineEdit::textChanged, validateForm);
    connect(addrEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("UPDATE FOURNISSEUR SET NOM=:nom, TYPE_MATIERE=:type, TELEPHONE=:tel, ADRESSE=:addr, DELAI_LIVRAISON=:delai, QUALITE=:qual, STATUT=:stat WHERE ID_FOURNISSEUR=:id");
        q.bindValue(":nom", nomEdit->text());
        q.bindValue(":type", typeCombo->currentText());
        q.bindValue(":tel", phoneEdit->text());
        q.bindValue(":addr", addrEdit->text());
        q.bindValue(":delai", dateEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":qual", qualiteCombo->currentText());
        q.bindValue(":stat", statutCombo->currentText());
        q.bindValue(":id", id);
        
        if(q.exec()){
            QMessageBox::information(editDialog, "Succès", "Mise à jour réussie.");
            loadFournisseurs();
            editDialog->accept();
        } else {
            QMessageBox::critical(editDialog, "Erreur", q.lastError().text());
        }
    });

    editDialog->exec();
}

void fournisseurs::on_pushButton_3_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) return;
    QString id = model->index(row, 0).data().toString();
    if(QMessageBox::question(this,"Confirmation","Supprimer?") == QMessageBox::Yes){
        QSqlQuery q;
        q.prepare("DELETE FROM FOURNISSEUR WHERE ID_FOURNISSEUR=:id");
        q.bindValue(":id", id);
        if(q.exec()){ loadFournisseurs(); }
    }
}

void fournisseurs::on_pushButton_7_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Export", "fournisseurs.csv", "CSV (*.csv)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::WriteOnly)){
        QTextStream out(&f);
        out << "ID;Nom;Type;Tel;Addr;Delai;Qualite;Statut\n";
        for(int r=0; r<model->rowCount(); r++){
            for(int c=0; c<model->columnCount(); c++){
                out << model->index(r,c).data().toString() << (c==7?"":";");
            }
            out << "\n";
        }
    }
}

void fournisseurs::on_pushButton_9_clicked()
{
    int act=0, inact=0;
    QSqlQuery q("SELECT STATUT, COUNT(*) FROM FOURNISSEUR GROUP BY STATUT");
    while(q.next()) {
        if(q.value(0).toString() == "Actif") act = q.value(1).toInt();
        else inact = q.value(1).toInt();
    }
    int total = act + inact;
    if(total == 0) return;

    QDialog *d = new QDialog(this);
    d->setWindowTitle("Stats");
    d->resize(600,400);
    QVBoxLayout *l = new QVBoxLayout(d);
    
    QHBoxLayout *h = new QHBoxLayout();
    for(auto &s : {qMakePair(QString("Actif"), act), qMakePair(QString("Inactif"), inact)}){
        QVBoxLayout *v = new QVBoxLayout();
        QFrame *bar = new QFrame();
        bar->setFixedWidth(100);
        bar->setFixedHeight(s.second * 200 / total);
        bar->setStyleSheet(QString("background: %1; border-radius: 5px;").arg(s.first=="Actif"?"#4caf50":"#f44336"));
        v->addWidget(new QLabel(QString::number(s.second)), 0, Qt::AlignCenter);
        v->addWidget(bar, 0, Qt::AlignCenter);
        v->addWidget(new QLabel(s.first), 0, Qt::AlignCenter);
        h->addLayout(v);
    }
    l->addLayout(h);
    d->exec();
}
