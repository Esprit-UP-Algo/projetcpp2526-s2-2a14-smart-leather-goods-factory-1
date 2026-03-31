#include "commandes.h"
#include "ui_commandes.h"
#include "login.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDateEdit>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

commandes::commandes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::commandes)
{
    ui->setupUi(this);
    model = new CustomCommandeModel(this);
    setupTable();
    loadSampleData();
}

commandes::~commandes()
{
    delete ui;
}

void commandes::setupTable()
{
    ui->tableWidget->setModel(model);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Client");
    model->setHeaderData(2, Qt::Horizontal, "Adresse");
    model->setHeaderData(3, Qt::Horizontal, "Date C.");
    model->setHeaderData(4, Qt::Horizontal, "Date L.");
    model->setHeaderData(5, Qt::Horizontal, "État");
    model->setHeaderData(6, Qt::Horizontal, "Montant");
    model->setHeaderData(7, Qt::Horizontal, "Paiement");
    model->setHeaderData(8, Qt::Horizontal, "Employé");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 180);
    ui->tableWidget->setColumnWidth(3, 100);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 110);
    ui->tableWidget->setColumnWidth(6, 90);
    ui->tableWidget->setColumnWidth(7, 100);
    ui->tableWidget->setColumnWidth(8, 140);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSortingEnabled(true);
}

void commandes::loadSampleData()
{
    model->setQuery("SELECT C.ID_COMMANDE, C.NOMCLIENT, C.ADRESSE_LIVRAISON, "
                    "TO_CHAR(C.DATE_COMMANDE, 'DD/MM/YYYY'), TO_CHAR(C.DATE_LIVRAISON, 'DD/MM/YYYY'), "
                    "C.ETAT_COMMANDE, C.MONTANT_TOTALE, C.MODE_PAIMENT, "
                    "E.NOM || ' ' || E.PRENOM AS EMP_NAME "
                    "FROM COMMANDE C LEFT JOIN EMPLOYEE E ON C.ID_EMPLOYEE = E.ID_EMPLOYEE");
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de chargement des commandes:" << model->lastError().text();
    }
}

void commandes::filterTable(const QString &idFilter, const QString &dateFilter)
{
    QString sql = "SELECT C.ID_COMMANDE, C.NOMCLIENT, C.ADRESSE_LIVRAISON, "
                  "TO_CHAR(C.DATE_COMMANDE, 'DD/MM/YYYY'), TO_CHAR(C.DATE_LIVRAISON, 'DD/MM/YYYY'), "
                  "C.ETAT_COMMANDE, C.MONTANT_TOTALE, C.MODE_PAIMENT, "
                  "E.NOM || ' ' || E.PRENOM AS EMP_NAME "
                  "FROM COMMANDE C LEFT JOIN EMPLOYEE E ON C.ID_EMPLOYEE = E.ID_EMPLOYEE "
                  "WHERE 1=1";

    if (!idFilter.isEmpty()) {
        sql += " AND LOWER(C.ID_COMMANDE) LIKE LOWER('%" + idFilter + "%')";
    }
    if (!dateFilter.isEmpty()) {
        sql += " AND TO_CHAR(C.DATE_COMMANDE, 'DD/MM/YYYY') LIKE '%" + dateFilter + "%'";
    }

    model->setQuery(sql);
}

void commandes::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouvelle Commande");
    ajoutDialog->setFixedSize(700, 850);
    ajoutDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40,40,40,40);
    mainLayout->setSpacing(12);

    // Auto-ID
    QString nextId = "CM001";
    QSqlQuery idQ("SELECT MAX(ID_COMMANDE) FROM COMMANDE");
    if(idQ.next() && !idQ.value(0).isNull()){
        QString last = idQ.value(0).toString();
        if(last.startsWith("CM")) {
            nextId = QString("CM%1").arg(last.mid(2).toInt() + 1, 3, 10, QChar('0'));
        }
    }

    QLineEdit *idEdit = new QLineEdit(nextId);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; font-weight: bold; padding: 10px; border-radius: 8px;");
    
    QLineEdit *clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("Nom du client");
    
    QLineEdit *addrEdit = new QLineEdit();
    addrEdit->setPlaceholderText("Adresse de livraison");

    QDateEdit *dateCEdit = new QDateEdit(QDate::currentDate());
    dateCEdit->setDisplayFormat("dd/MM/yyyy");
    dateCEdit->setCalendarPopup(true);

    QDateEdit *dateLEdit = new QDateEdit(QDate::currentDate().addDays(7));
    dateLEdit->setDisplayFormat("dd/MM/yyyy");
    dateLEdit->setCalendarPopup(true);

    QComboBox *etatBox = new QComboBox();
    etatBox->addItems({"En attente", "Confirmée", "En préparation", "Livrée", "Annulée"});

    QLineEdit *montantEdit = new QLineEdit();
    montantEdit->setPlaceholderText("Montant (ex: 150.50)");

    QComboBox *paimentBox = new QComboBox();
    paimentBox->addItems({"Espèces", "Carte Bancaire", "Virement", "Chèque"});

    QComboBox *empBox = new QComboBox();
    QSqlQuery empQ("SELECT ID_EMPLOYEE, NOM || ' ' || PRENOM FROM EMPLOYEE");
    while(empQ.next()){
        empBox->addItem(empQ.value(1).toString(), empQ.value(0));
    }

    mainLayout->addWidget(new QLabel("ID COMMANDE:")); mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("CLIENT:")); mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(new QLabel("ADRESSE:")); mainLayout->addWidget(addrEdit);
    mainLayout->addWidget(new QLabel("DATE COMMANDE:")); mainLayout->addWidget(dateCEdit);
    mainLayout->addWidget(new QLabel("DATE LIVRAISON:")); mainLayout->addWidget(dateLEdit);
    mainLayout->addWidget(new QLabel("ÉTAT:")); mainLayout->addWidget(etatBox);
    mainLayout->addWidget(new QLabel("MONTANT:")); mainLayout->addWidget(montantEdit);
    mainLayout->addWidget(new QLabel("PAIEMENT:")); mainLayout->addWidget(paimentBox);
    mainLayout->addWidget(new QLabel("RESPONSABLE:")); mainLayout->addWidget(empBox);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(btnSave);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };

        check(clientEdit, clientEdit->text().trimmed().length() >= 2);
        check(addrEdit, addrEdit->text().trimmed().length() >= 5);
        bool okM; double m = montantEdit->text().toDouble(&okM);
        check(montantEdit, okM && m > 0);

        btnSave->setEnabled(allValid);
    };

    connect(clientEdit, &QLineEdit::textChanged, validateForm);
    connect(addrEdit, &QLineEdit::textChanged, validateForm);
    connect(montantEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("INSERT INTO COMMANDE (ID_COMMANDE, NOMCLIENT, ADRESSE_LIVRAISON, DATE_COMMANDE, DATE_LIVRAISON, "
                  "ETAT_COMMANDE, MONTANT_TOTALE, MODE_PAIMENT, ID_EMPLOYEE) "
                  "VALUES (:id, :cl, :ad, TO_DATE(:dc, 'DD/MM/YYYY'), TO_DATE(:dl, 'DD/MM/YYYY'), :et, :mt, :mp, :em)");
        q.bindValue(":id", idEdit->text());
        q.bindValue(":cl", clientEdit->text());
        q.bindValue(":ad", addrEdit->text());
        q.bindValue(":dc", dateCEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":dl", dateLEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":et", etatBox->currentText());
        q.bindValue(":mt", montantEdit->text().toDouble());
        q.bindValue(":mp", paimentBox->currentText());
        q.bindValue(":em", empBox->currentData());

        if(q.exec()){
            QMessageBox::information(ajoutDialog, "Succès", "✅ Commande ajoutée.");
            loadSampleData();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", q.lastError().text());
        }
    });

    ajoutDialog->exec();
}

void commandes::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez une commande."); return; }

    QString id = model->index(row, 0).data().toString();

    QDialog *editDialog = new QDialog(this);
    editDialog->setWindowTitle("Modifier Commande");
    editDialog->setFixedSize(700, 850);
    editDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(editDialog);
    mainLayout->setContentsMargins(40,40,40,40);
    mainLayout->setSpacing(12);

    QLineEdit *clientEdit = new QLineEdit(model->index(row, 1).data().toString());
    QLineEdit *addrEdit = new QLineEdit(model->index(row, 2).data().toString());

    QDateEdit *dateCEdit = new QDateEdit(QDate::fromString(model->index(row, 3).data().toString(), "dd/MM/yyyy"));
    dateCEdit->setDisplayFormat("dd/MM/yyyy");
    dateCEdit->setCalendarPopup(true);

    QDateEdit *dateLEdit = new QDateEdit(QDate::fromString(model->index(row, 4).data().toString(), "dd/MM/yyyy"));
    dateLEdit->setDisplayFormat("dd/MM/yyyy");
    dateLEdit->setCalendarPopup(true);

    QComboBox *etatBox = new QComboBox();
    etatBox->addItems({"En attente", "Confirmée", "En préparation", "Livrée", "Annulée"});
    etatBox->setCurrentText(model->index(row, 5).data().toString());

    QLineEdit *montantEdit = new QLineEdit(model->index(row, 6).data().toString().replace(" €", ""));

    QComboBox *paimentBox = new QComboBox();
    paimentBox->addItems({"Espèces", "Carte Bancaire", "Virement", "Chèque"});
    paimentBox->setCurrentText(model->index(row, 7).data().toString());

    QComboBox *empBox = new QComboBox();
    QSqlQuery empQ("SELECT ID_EMPLOYEE, NOM || ' ' || PRENOM FROM EMPLOYEE");
    while(empQ.next()){
        empBox->addItem(empQ.value(1).toString(), empQ.value(0));
    }
    // Set current employee (requires fetching the ID from DB as it's not in the model directly)
    QSqlQuery curEmpQ; curEmpQ.prepare("SELECT ID_EMPLOYEE FROM COMMANDE WHERE ID_COMMANDE = :id");
    curEmpQ.bindValue(":id", id);
    if(curEmpQ.exec() && curEmpQ.next()){
        int index = empBox->findData(curEmpQ.value(0));
        if(index != -1) empBox->setCurrentIndex(index);
    }

    mainLayout->addWidget(new QLabel("CLIENT:")); mainLayout->addWidget(clientEdit);
    mainLayout->addWidget(new QLabel("ADRESSE:")); mainLayout->addWidget(addrEdit);
    mainLayout->addWidget(new QLabel("DATE COMMANDE:")); mainLayout->addWidget(dateCEdit);
    mainLayout->addWidget(new QLabel("DATE LIVRAISON:")); mainLayout->addWidget(dateLEdit);
    mainLayout->addWidget(new QLabel("ÉTAT:")); mainLayout->addWidget(etatBox);
    mainLayout->addWidget(new QLabel("MONTANT:")); mainLayout->addWidget(montantEdit);
    mainLayout->addWidget(new QLabel("PAIEMENT:")); mainLayout->addWidget(paimentBox);
    mainLayout->addWidget(new QLabel("RESPONSABLE:")); mainLayout->addWidget(empBox);

    QPushButton *btnUpdate = new QPushButton("💾 Mettre à jour");
    btnUpdate->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(btnUpdate);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };
        check(clientEdit, clientEdit->text().trimmed().length() >= 2);
        check(addrEdit, addrEdit->text().trimmed().length() >= 5);
        bool okM; double m = montantEdit->text().toDouble(&okM);
        check(montantEdit, okM && m > 0);
        btnUpdate->setEnabled(allValid);
    };

    connect(clientEdit, &QLineEdit::textChanged, validateForm);
    connect(addrEdit, &QLineEdit::textChanged, validateForm);
    connect(montantEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnUpdate, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("UPDATE COMMANDE SET NOMCLIENT=:cl, ADRESSE_LIVRAISON=:ad, DATE_COMMANDE=TO_DATE(:dc, 'DD/MM/YYYY'), "
                  "DATE_LIVRAISON=TO_DATE(:dl, 'DD/MM/YYYY'), ETAT_COMMANDE=:et, MONTANT_TOTALE=:mt, "
                  "MODE_PAIMENT=:mp, ID_EMPLOYEE=:em WHERE ID_COMMANDE=:id");
        q.bindValue(":cl", clientEdit->text());
        q.bindValue(":ad", addrEdit->text());
        q.bindValue(":dc", dateCEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":dl", dateLEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":et", etatBox->currentText());
        q.bindValue(":mt", montantEdit->text().toDouble());
        q.bindValue(":mp", paimentBox->currentText());
        q.bindValue(":em", empBox->currentData());
        q.bindValue(":id", id);

        if(q.exec()){
            QMessageBox::information(editDialog, "Succès", "✅ Mise à jour réussie.");
            loadSampleData();
            editDialog->accept();
        } else {
            QMessageBox::critical(editDialog, "Erreur", q.lastError().text());
        }
    });

    editDialog->exec();
}

void commandes::on_pushButton_3_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) return;
    QString id = model->index(row, 0).data().toString();
    if(QMessageBox::question(this, "Suppression", "Voulez-vous supprimer cette commande ?") == QMessageBox::Yes){
        QSqlQuery q;
        q.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE = :id");
        q.bindValue(":id", id);
        if(q.exec()) loadSampleData();
    }
}

void commandes::on_pushButton_4_clicked() { ui->lineEdit_5->clear(); ui->lineEdit_6->clear(); loadSampleData(); }

void commandes::on_pushButton_10_clicked() { filterTable(ui->lineEdit_5->text(), ui->lineEdit_6->text()); }

void commandes::on_lineEdit_5_textChanged(const QString &text) { filterTable(text, ui->lineEdit_6->text()); }

void commandes::on_lineEdit_6_textChanged(const QString &text) { filterTable(ui->lineEdit_5->text(), text); }

void commandes::on_pushButton_7_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter", "commandes.csv", "CSV (*.csv)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::WriteOnly)){
        QTextStream out(&f);
        out << "ID;Client;Adresse;Date C.;Date L.;Etat;Montant;Paiement;Employe\n";
        for(int r=0; r<model->rowCount(); r++){
            QStringList l;
            for(int c=0; c<model->columnCount(); c++) l << model->index(r,c).data().toString();
            out << l.join(";") << "\n";
        }
        QMessageBox::information(this, "Export", "✅ Export terminé.");
    }
}

void commandes::on_pushButton_9_clicked()
{
    int att=0, conf=0, prep=0, liv=0, ann=0;
    QSqlQuery q("SELECT ETAT_COMMANDE, COUNT(*) FROM COMMANDE GROUP BY ETAT_COMMANDE");
    while(q.next()){
        QString e = q.value(0).toString();
        int c = q.value(1).toInt();
        if(e=="En attente") att=c;
        else if(e=="Confirmée") conf=c;
        else if(e=="En préparation") prep=c;
        else if(e=="Livrée") liv=c;
        else if(e=="Annulée") ann=c;
    }
    int tot = att+conf+prep+liv+ann; if(tot==0) return;

    QDialog *d = new QDialog(this); d->setWindowTitle("Stats"); d->resize(800,500);
    d->setStyleSheet("QDialog { background-color: #f1e7dc; border-radius: 15px; }");
    QVBoxLayout *v = new QVBoxLayout(d);
    QHBoxLayout *h = new QHBoxLayout();
    
    struct SD { QString n; int c; QColor cl; };
    QList<SD> sds = {{"Attente",att,QColor(212,172,130)}, {"Conf.",conf,QColor(107,62,38)}, {"Prép.",prep,QColor(160,110,70)}, {"Livrée",liv,QColor(74,39,23)}, {"Ann.",ann,QColor(180,90,50)}};

    for(const auto &s : sds){
        if(s.c == 0) continue;
        QVBoxLayout *bc = new QVBoxLayout();
        QFrame *bar = new QFrame(); bar->setFixedWidth(80); bar->setFixedHeight(s.c * 300 / tot);
        bar->setStyleSheet(QString("background: %1; border-radius: 8px; border: 1px solid #3a1f14;").arg(s.cl.name()));
        bc->addWidget(new QLabel(QString::number(s.c)), 0, Qt::AlignCenter);
        bc->addWidget(bar, 0, Qt::AlignCenter);
        bc->addWidget(new QLabel(s.n), 0, Qt::AlignCenter);
        h->addLayout(bc);
    }
    v->addLayout(h);
    QPushButton *cl = new QPushButton("Fermer"); cl->setStyleSheet("background: #6b3e26; color: white; padding: 10px; border-radius: 10px;");
    connect(cl, &QPushButton::clicked, d, &QDialog::accept); v->addWidget(cl, 0, Qt::AlignCenter);
    d->exec();
}

// NAVIGATION
void commandes::on_pushButton_5_clicked() { hide(); (new login(this))->show(); }
void commandes::on_pushButton_11_clicked() { hide(); (new pageemployee(this))->show(); }
void commandes::on_pushButton_12_clicked() { hide(); (new fournisseurs(this))->show(); }
void commandes::on_pushButton_19_clicked() { hide(); (new pageemployee(this))->show(); }
void commandes::on_pushButton_20_clicked() { hide(); (new fournisseurs(this))->show(); }
void commandes::on_pushButton_21_clicked() { hide(); (new products(this))->show(); }
void commandes::on_pushButton_22_clicked() { hide(); (new Matieres(this))->show(); }
void commandes::on_pushButton_23_clicked() { hide(); (new pagemachine(this))->show(); }
void commandes::on_pushButton_17_clicked() { hide(); (new login(this))->show(); }
