#include "matieres.h"
#include "ui_matieres.h"
#include <QDateEdit>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDateTime>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "pageemployee.h"
#include "login.h"
#include "fournisseurs.h"
#include "products.h"
#include "pagemachine.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

Matieres::Matieres(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Matieres)
{
    ui->setupUi(this);
    model = new CustomMatiereModel(this);
    setupMatieresTable();
    loadMatieres();
    setupSearch();
}

Matieres::~Matieres()
{
    delete ui;
}

void Matieres::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
    connect(ui->searchTypeEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
}

void Matieres::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString typeFilter = ui->searchTypeEdit->text();

    QString sql = "SELECT ID_MATIERE, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, TO_CHAR(DATE_RECEPTION, 'DD/MM/YYYY') FROM MATIEREPREMIER WHERE 1=1";
    
    if (!idFilter.isEmpty()) {
        sql += " AND LOWER(ID_MATIERE) LIKE LOWER('%" + idFilter + "%')";
    }
    if (!typeFilter.isEmpty()) {
        sql += " AND LOWER(TYPE_CUIR) LIKE LOWER('%" + typeFilter + "%')";
    }

    model->setQuery(sql);
}

void Matieres::on_pushButton_6_clicked() { hide(); (new Matieres(this))->show(); }
void Matieres::on_pushButton_11_clicked() { hide(); (new pageemployee(this))->show(); }
void Matieres::on_pushButton_5_clicked() { hide(); (new login(this))->show(); }
void Matieres::on_pushButton_20_clicked() { hide(); (new fournisseurs(this))->show(); }
void Matieres::on_pushButton_21_clicked() { hide(); (new products(this))->show(); }
void Matieres::on_pushButton_22_clicked() { hide(); (new Matieres(this))->show(); }
void Matieres::on_pushButton_23_clicked() { hide(); (new pagemachine(this))->show(); }

void Matieres::setupMatieresTable()
{
    ui->tableWidget->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Qualité");
    model->setHeaderData(3, Qt::Horizontal, "Couleur");
    model->setHeaderData(4, Qt::Horizontal, "Épaisseur");
    model->setHeaderData(5, Qt::Horizontal, "Prix Unit.");
    model->setHeaderData(6, Qt::Horizontal, "Stock");
    model->setHeaderData(7, Qt::Horizontal, "Seuil");
    model->setHeaderData(8, Qt::Horizontal, "Date Réc.");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 90);
    ui->tableWidget->setColumnWidth(3, 90);
    ui->tableWidget->setColumnWidth(4, 90);
    ui->tableWidget->setColumnWidth(5, 85);
    ui->tableWidget->setColumnWidth(6, 80);
    ui->tableWidget->setColumnWidth(7, 80);
    ui->tableWidget->setColumnWidth(8, 100);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void Matieres::loadMatieres()
{
    model->setQuery("SELECT ID_MATIERE, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, TO_CHAR(DATE_RECEPTION, 'DD/MM/YYYY') FROM MATIEREPREMIER");
}

void Matieres::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouvelle matière");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    
    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40,40,40,40); mainLayout->setSpacing(12);

    QString nextId = "MT001";
    QSqlQuery idQ("SELECT MAX(ID_MATIERE) FROM MATIEREPREMIER");
    if(idQ.next() && !idQ.value(0).isNull()){
        nextId = QString("MT%1").arg(idQ.value(0).toString().mid(2).toInt() + 1, 3, 10, QChar('0'));
    }

    QLineEdit *idEdit = new QLineEdit(nextId);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; font-weight: bold; padding: 10px;");

    QLineEdit *typeEdit = new QLineEdit();
    QLineEdit *couleurEdit = new QLineEdit();
    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    QLineEdit *prixEdit = new QLineEdit();
    QLineEdit *stockEdit = new QLineEdit();
    QLineEdit *epaisseurEdit = new QLineEdit();
    QLineEdit *seuilEdit = new QLineEdit();
    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy"); dateEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("ID:")); mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("TYPE:")); mainLayout->addWidget(typeEdit);
    mainLayout->addWidget(new QLabel("QUALITÉ:")); mainLayout->addWidget(qualiteCombo);
    mainLayout->addWidget(new QLabel("COULEUR:")); mainLayout->addWidget(couleurEdit);
    mainLayout->addWidget(new QLabel("ÉPAISSEUR:")); mainLayout->addWidget(epaisseurEdit);
    mainLayout->addWidget(new QLabel("PRIX UNITAIRE:")); mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(new QLabel("STOCK ACTUEL:")); mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("SEUIL ALERTE:")); mainLayout->addWidget(seuilEdit);
    mainLayout->addWidget(new QLabel("DATE RÉCEPTION:")); mainLayout->addWidget(dateEdit);

    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(btnSave);

    auto validate = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };
        check(typeEdit, typeEdit->text().trimmed().length() >= 2);
        check(couleurEdit, couleurEdit->text().trimmed().length() >= 2);
        bool okP; double p = prixEdit->text().toDouble(&okP); check(prixEdit, okP && p > 0);
        bool okS; int s = stockEdit->text().toInt(&okS); check(stockEdit, okS && s >= 0);
        bool okE; double ep = epaisseurEdit->text().toDouble(&okE); check(epaisseurEdit, okE && ep > 0);
        bool okSL; int sl = seuilEdit->text().toInt(&okSL); check(seuilEdit, okSL && sl >= 0);
        btnSave->setEnabled(allValid);
    };

    connect(typeEdit, &QLineEdit::textChanged, validate);
    connect(couleurEdit, &QLineEdit::textChanged, validate);
    connect(prixEdit, &QLineEdit::textChanged, validate);
    connect(stockEdit, &QLineEdit::textChanged, validate);
    connect(epaisseurEdit, &QLineEdit::textChanged, validate);
    connect(seuilEdit, &QLineEdit::textChanged, validate);
    validate();

    connect(btnSave, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("INSERT INTO MATIEREPREMIER (ID_MATIERE, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, DATE_RECEPTION) "
                  "VALUES (:id, :t, :q, :c, :e, :p, :s, :sm, TO_DATE(:d, 'DD/MM/YYYY'))");
        q.bindValue(":id", idEdit->text());
        q.bindValue(":t", typeEdit->text());
        q.bindValue(":q", qualiteCombo->currentText());
        q.bindValue(":c", couleurEdit->text());
        q.bindValue(":e", epaisseurEdit->text().toDouble());
        q.bindValue(":p", prixEdit->text().toDouble());
        q.bindValue(":s", stockEdit->text().toInt());
        q.bindValue(":sm", seuilEdit->text().toInt());
        q.bindValue(":d", dateEdit->date().toString("dd/MM/yyyy"));
        if(q.exec()){ QMessageBox::information(ajoutDialog, "Succès", "✅ Ajouté!"); loadMatieres(); ajoutDialog->accept(); }
        else QMessageBox::critical(ajoutDialog, "Erreur", q.lastError().text());
    });
    ajoutDialog->exec();
}

void Matieres::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this, "Sélection", "Sélectionnez une matière."); return; }
    QString id = model->index(row, 0).data().toString();

    QDialog *editDialog = new QDialog(this);
    editDialog->setWindowTitle("Modifier Matière");
    editDialog->setFixedSize(650, 800);
    editDialog->setStyleSheet("QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }");
    QVBoxLayout *layout = new QVBoxLayout(editDialog);
    layout->setContentsMargins(40,40,40,40); layout->setSpacing(12);

    QLineEdit *typeEdit = new QLineEdit(model->index(row, 1).data().toString());
    QComboBox *qualiteCombo = new QComboBox(); qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
    qualiteCombo->setCurrentText(model->index(row, 2).data().toString());
    QLineEdit *couleurEdit = new QLineEdit(model->index(row, 3).data().toString());
    QLineEdit *epaisseurEdit = new QLineEdit(model->index(row, 4).data().toString());
    QLineEdit *prixEdit = new QLineEdit(model->index(row, 5).data().toString().replace(" €", ""));
    QLineEdit *stockEdit = new QLineEdit(model->index(row, 6).data().toString());
    QLineEdit *seuilEdit = new QLineEdit(model->index(row, 7).data().toString());
    QDateEdit *dateEdit = new QDateEdit(QDate::fromString(model->index(row, 8).data().toString(), "dd/MM/yyyy"));
    dateEdit->setDisplayFormat("dd/MM/yyyy"); dateEdit->setCalendarPopup(true);

    layout->addWidget(new QLabel("TYPE:")); layout->addWidget(typeEdit);
    layout->addWidget(new QLabel("QUALITÉ:")); layout->addWidget(qualiteCombo);
    layout->addWidget(new QLabel("COULEUR:")); layout->addWidget(couleurEdit);
    layout->addWidget(new QLabel("ÉPAISSEUR:")); layout->addWidget(epaisseurEdit);
    layout->addWidget(new QLabel("PRIX UNITAIRE:")); layout->addWidget(prixEdit);
    layout->addWidget(new QLabel("STOCK ACTUEL:")); layout->addWidget(stockEdit);
    layout->addWidget(new QLabel("SEUIL ALERTE:")); layout->addWidget(seuilEdit);
    layout->addWidget(new QLabel("DATE RÉCEPTION:")); layout->addWidget(dateEdit);

    QPushButton *btnUpdate = new QPushButton("💾 Mettre à jour");
    btnUpdate->setStyleSheet("background-color: #6f8f3d; color: white; padding: 15px; border-radius: 10px; font-weight: bold; margin-top: 10px;");
    layout->addWidget(btnUpdate);

    auto validate = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 10px; border-radius: 8px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 10px; border-radius: 8px;"); allValid = false; }
        };
        check(typeEdit, typeEdit->text().trimmed().length() >= 2);
        check(couleurEdit, couleurEdit->text().trimmed().length() >= 2);
        bool okP; double p = prixEdit->text().toDouble(&okP); check(prixEdit, okP && p > 0);
        bool okS; int s = stockEdit->text().toInt(&okS); check(stockEdit, okS && s >= 0);
        bool okE; double ep = epaisseurEdit->text().toDouble(&okE); check(epaisseurEdit, okE && ep > 0);
        bool okSL; int sl = seuilEdit->text().toInt(&okSL); check(seuilEdit, okSL && sl >= 0);
        btnUpdate->setEnabled(allValid);
    };

    connect(typeEdit, &QLineEdit::textChanged, validate);
    connect(couleurEdit, &QLineEdit::textChanged, validate);
    connect(prixEdit, &QLineEdit::textChanged, validate);
    connect(stockEdit, &QLineEdit::textChanged, validate);
    connect(epaisseurEdit, &QLineEdit::textChanged, validate);
    connect(seuilEdit, &QLineEdit::textChanged, validate);
    validate();

    connect(btnUpdate, &QPushButton::clicked, [=](){
        QSqlQuery q;
        q.prepare("UPDATE MATIEREPREMIER SET TYPE_CUIR=:t, QUALITE=:q, COULEUR=:c, EPAISSEUR=:e, "
                  "PRIX_UNITAIRE=:p, QUANTITE_STOCK=:s, SEUIL_MIN=:sm, DATE_RECEPTION=TO_DATE(:d, 'DD/MM/YYYY') "
                  "WHERE ID_MATIERE=:id");
        q.bindValue(":t", typeEdit->text()); q.bindValue(":q", qualiteCombo->currentText());
        q.bindValue(":c", couleurEdit->text()); q.bindValue(":e", epaisseurEdit->text().toDouble());
        q.bindValue(":p", prixEdit->text().toDouble()); q.bindValue(":s", stockEdit->text().toInt());
        q.bindValue(":sm", seuilEdit->text().toInt()); q.bindValue(":d", dateEdit->date().toString("dd/MM/yyyy"));
        q.bindValue(":id", id);
        if(q.exec()){ QMessageBox::information(editDialog, "Succès", "✅ Mis à jour!"); loadMatieres(); editDialog->accept(); }
        else QMessageBox::critical(editDialog, "Erreur", q.lastError().text());
    });
    editDialog->exec();
}

void Matieres::on_pushButton_3_clicked()
{
    int row = ui->tableWidget->currentIndex().row();
    if (row < 0) return;
    QString id = model->index(row, 0).data().toString();
    if(QMessageBox::question(this, "Suppression", "Supprimer cette matière?") == QMessageBox::Yes){
        QSqlQuery q; q.prepare("DELETE FROM MATIEREPREMIER WHERE ID_MATIERE = :id");
        q.bindValue(":id", id);
        if(q.exec()) loadMatieres();
    }
}

void Matieres::on_pushButton_4_clicked() { ui->searchIdEdit->clear(); ui->searchTypeEdit->clear(); loadMatieres(); }

void Matieres::on_pushButton_7_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Exporter", "matieres.csv", "CSV (*.csv)");
    if(path.isEmpty()) return;
    QFile f(path);
    if(f.open(QIODevice::WriteOnly)){
        QTextStream out(&f);
        out << "ID;Type;Qualite;Couleur;Epaisseur;Prix;Stock;Seuil;Date\n";
        for(int r=0; r<model->rowCount(); r++){
            QStringList l;
            for(int c=0; c<model->columnCount(); c++) l << model->index(r,c).data().toString();
            out << l.join(";") << "\n";
        }
        QMessageBox::information(this, "Succès", "✅ Exporté!");
    }
}

void Matieres::on_pushButton_9_clicked()
{
    int qA = 0, qB = 0, qC = 0, stockAlerte = 0;
    QSqlQuery q("SELECT QUALITE, QUANTITE_STOCK, SEUIL_MIN FROM MATIEREPREMIER");
    while(q.next()){
        QString qual = q.value(0).toString();
        int stock = q.value(1).toInt();
        int seuil = q.value(2).toInt();
        if (qual.contains("A")) qA++; else if (qual.contains("B")) qB++; else if (qual.contains("C")) qC++;
        if (stock < seuil) stockAlerte++;
    }
    int total = qA + qB + qC; if (total == 0) return;

    QDialog *statsDialog = new QDialog(this); statsDialog->setWindowTitle("Stats"); statsDialog->resize(800, 600);
    statsDialog->setStyleSheet("QDialog { background-color: #f1e7dc; border-radius: 20px; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    
    struct Stat { QString label; int val; QColor color; };
    QList<Stat> stats = { {"Qualité A", qA, QColor(100, 200, 100)}, {"Qualité B", qB, QColor(255, 255, 100)}, {"Qualité C", qC, QColor(200, 100, 100)} };

    QHBoxLayout *chartLayout = new QHBoxLayout();
    for (const auto &s : stats) {
        if (s.val == 0) continue;
        QVBoxLayout *barCol = new QVBoxLayout();
        QFrame *bar = new QFrame(); bar->setFixedWidth(90); bar->setFixedHeight(s.val * 300 / total);
        bar->setStyleSheet(QString("background-color: %1; border-radius: 10px; border: 1px solid #3a1f14;").arg(s.color.name()));
        barCol->addWidget(new QLabel(QString::number(s.val)), 0, Qt::AlignCenter);
        barCol->addWidget(bar, 0, Qt::AlignCenter);
        barCol->addWidget(new QLabel(s.label), 0, Qt::AlignCenter);
        chartLayout->addLayout(barCol);
    }
    mainLayout->addLayout(chartLayout);
    QPushButton *close = new QPushButton("Fermer"); connect(close, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(close, 0, Qt::AlignCenter);
    statsDialog->exec();
}
