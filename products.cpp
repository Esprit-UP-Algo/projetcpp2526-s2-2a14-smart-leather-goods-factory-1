#include "products.h"
#include "ui_products.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include "commandes.h"
#include "pageemployee.h"
#include "login.h"
#include "fournisseurs.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>

products::products(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::products)
{
    ui->setupUi(this);
    model = new CustomProductModel(this);
    setupProductsTable();
    loadProducts();
    setupSearch();
}

products::~products()
{
    delete ui;
}

void products::setupSearch()
{
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &products::filterTable);
    connect(ui->searchNameEdit, &QLineEdit::textChanged, this, &products::filterTable);
}

void products::filterTable()
{
    QString idFilter = ui->searchIdEdit->text();
    QString nameFilter = ui->searchNameEdit->text();

    QString sql = "SELECT ID_PRODUIT, NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM PRODUIT WHERE 1=1";
    
    if (!idFilter.isEmpty()) {
        sql += " AND LOWER(ID_PRODUIT) LIKE LOWER('%" + idFilter + "%')";
    }
    if (!nameFilter.isEmpty()) {
        sql += " AND LOWER(NOM) LIKE LOWER('%" + nameFilter + "%')";
    }

    model->setQuery(sql);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de filtrage:" << model->lastError().text();
    }
}

void products::setupProductsTable()
{
    ui->tableWidget->setModel(model);
    
    // Set headers through the model
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Catégorie");
    model->setHeaderData(3, Qt::Horizontal, "Prix");
    model->setHeaderData(4, Qt::Horizontal, "Stock");
    model->setHeaderData(5, Qt::Horizontal, "Statut");
    model->setHeaderData(6, Qt::Horizontal, "Date création");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 120);
    ui->tableWidget->setColumnWidth(3, 110);
    ui->tableWidget->setColumnWidth(4, 110);
    ui->tableWidget->setColumnWidth(5, 120);
    ui->tableWidget->setColumnWidth(6, 110);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void products::loadProducts()
{
    model->setQuery("SELECT ID_PRODUIT, NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM PRODUIT");
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de chargement des produits:" << model->lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les produits.");
    }
}

void products::on_pushButton_6_clicked()
{
    hide();
    commandes *lg = new commandes(this);
    lg->show();
}

void products::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
    pl->show();
}

void products::on_pushButton_5_clicked()
{
    hide();
    login *lg = new login(this);
    lg->show();
}

void products::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void products::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void products::on_pushButton_22_clicked()
{
    hide();
    Matieres *pddd = new Matieres(this);
    pddd->show();
}

void products::on_pushButton_23_clicked()
{
    hide();
    pagemachine *pdedd = new pagemachine(this);
    pdedd->show();
}

void products::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Nouveau produit");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setModal(true);

    ajoutDialog->setStyleSheet(R"(
        QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }
        QLabel#headerLabel { color: #6b3e26; font-size: 22px; font-weight: bold; margin-bottom: 15px; }
        QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }
        QLineEdit, QDateTimeEdit, QComboBox {
            background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px;
            padding: 12px; color: #3a2a20; font-size: 14px; min-height: 20px;
        }
        QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }
        QPushButton {
            border-radius: 10px; padding: 15px; font-weight: bold; color: white;
            border-bottom: 3px solid rgba(0,0,0,0.2); font-size: 15px; min-width: 150px;
        }
        QPushButton#btnSave { background-color: #6f8f3d; }
        QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }
        QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("AJOUTER PRODUIT");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    // Auto-generate ID
    QString nextId = "P001";
    QSqlQuery idQuery("SELECT MAX(ID_PRODUIT) FROM PRODUIT");
    if (idQuery.next() && !idQuery.value(0).isNull()) {
        QString lastId = idQuery.value(0).toString();
        if (lastId.startsWith("P")) {
            int num = lastId.mid(1).toInt() + 1;
            nextId = QString("P%1").arg(num, 3, 10, QChar('0'));
        }
    }

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(nextId);
    idEdit->setReadOnly(true);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setStyleSheet("background-color: #e6d8cc; color: #5b2f1d; font-weight: bold;");

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Nom du produit");
    nameEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *categoryEdit = new QLineEdit();
    categoryEdit->setPlaceholderText("Catégorie");
    categoryEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *priceEdit = new QLineEdit();
    priceEdit->setPlaceholderText("Prix (ex: 29.99)");
    priceEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *stockEdit = new QLineEdit();
    stockEdit->setPlaceholderText("Quantité en stock");
    stockEdit->setAlignment(Qt::AlignCenter);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Actif", "Arrêté", "En attente"});

    QDateEdit *dateCreationEdit = new QDateEdit(QDate::currentDate());
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("🔑 Identifiant Automatique :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📝 Nom :"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(new QLabel("🏷️ Catégorie :"));
    mainLayout->addWidget(categoryEdit);
    mainLayout->addWidget(new QLabel("💰 Prix (€) :"));
    mainLayout->addWidget(priceEdit);
    mainLayout->addWidget(new QLabel("📦 Stock :"));
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("⚡ Statut :"));
    mainLayout->addWidget(statusCombo);
    mainLayout->addWidget(new QLabel("📅 Date création :"));
    mainLayout->addWidget(dateCreationEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Enregistrer");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;"); allValid = false; }
        };

        check(nameEdit, nameEdit->text().trimmed().length() >= 2);
        check(categoryEdit, categoryEdit->text().trimmed().length() >= 2);

        bool okP; double p = priceEdit->text().toDouble(&okP);
        check(priceEdit, okP && p > 0);

        bool okS; int s = stockEdit->text().toInt(&okS);
        check(stockEdit, okS && s >= 0);

        btnSave->setEnabled(allValid);
    };

    connect(nameEdit, &QLineEdit::textChanged, validateForm);
    connect(categoryEdit, &QLineEdit::textChanged, validateForm);
    connect(priceEdit, &QLineEdit::textChanged, validateForm);
    connect(stockEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, nameEdit, categoryEdit,
                                               priceEdit, stockEdit, statusCombo,
                                               dateCreationEdit, this]() {
        QSqlQuery query;
        query.prepare("INSERT INTO PRODUIT (ID_PRODUIT, NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, DATE_CREATION) "
                      "VALUES (:id, :nom, :categorie, :prix, :stock, :statut, TO_DATE(:date, 'DD/MM/YYYY'))");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":nom", nameEdit->text());
        query.bindValue(":categorie", categoryEdit->text());
        query.bindValue(":prix", priceEdit->text().toDouble());
        query.bindValue(":stock", stockEdit->text().toInt());
        query.bindValue(":statut", statusCombo->currentText());
        query.bindValue(":date", dateCreationEdit->date().toString("dd/MM/yyyy"));

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ Produit ajouté avec succès!");
            loadProducts();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur Oracle", query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void products::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentIndex().row();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un produit à modifier.");
        return;
    }

    QString id = model->index(currentRow, 0).data().toString();
    QString name = model->index(currentRow, 1).data().toString();
    QString category = model->index(currentRow, 2).data().toString();
    QString price = model->index(currentRow, 3).data().toString().replace(" €", "");
    QString stock = model->index(currentRow, 4).data().toString();
    QString status = model->index(currentRow, 5).data().toString();
    QDate creationDate = QDate::fromString(model->index(currentRow, 6).data().toString(), "dd/MM/yyyy");

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier produit");
    modifierDialog->setFixedSize(650, 800);
    modifierDialog->setModal(true);

    modifierDialog->setStyleSheet(R"(
        QDialog { background-color: #f4ede6; border: 3px dashed #c9b2a2; border-radius: 20px; }
        QLabel#headerLabel { color: #6b3e26; font-size: 22px; font-weight: bold; margin-bottom: 15px; }
        QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }
        QLineEdit, QDateTimeEdit, QComboBox {
            background-color: #fffaf5; border: 1px solid #c9b2a2; border-radius: 10px;
            padding: 12px; color: #3a2a20; font-size: 14px; min-height: 20px;
        }
        QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus { border: 2px solid #6b3e26; }
        QPushButton {
            border-radius: 10px; padding: 15px; font-weight: bold; color: white;
            border-bottom: 3px solid rgba(0,0,0,0.2); font-size: 15px; min-width: 150px;
        }
        QPushButton#btnSave { background-color: #6f8f3d; }
        QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }
        QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(modifierDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("MODIFIER PRODUIT");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(id);
    idEdit->setReadOnly(true);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setStyleSheet("background-color: #e6d8cc;");

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setText(name);
    nameEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *categoryEdit = new QLineEdit();
    categoryEdit->setText(category);
    categoryEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *priceEdit = new QLineEdit();
    priceEdit->setText(price);
    priceEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *stockEdit = new QLineEdit();
    stockEdit->setText(stock);
    stockEdit->setAlignment(Qt::AlignCenter);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Actif", "Arrêté", "En attente"});
    statusCombo->setCurrentText(status);

    QDateEdit *dateCreationEdit = new QDateEdit(creationDate);
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📝 Nom :"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(new QLabel("🏷️ Catégorie :"));
    mainLayout->addWidget(categoryEdit);
    mainLayout->addWidget(new QLabel("💰 Prix (€) :"));
    mainLayout->addWidget(priceEdit);
    mainLayout->addWidget(new QLabel("📦 Stock :"));
    mainLayout->addWidget(stockEdit);
    mainLayout->addWidget(new QLabel("⚡ Statut :"));
    mainLayout->addWidget(statusCombo);
    mainLayout->addWidget(new QLabel("📅 Date création :"));
    mainLayout->addWidget(dateCreationEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
    btnSave->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    auto validateForm = [=]() {
        bool allValid = true;
        auto check = [&](QLineEdit* e, bool cond) {
            if (cond) e->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            else { e->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;"); allValid = false; }
        };
        check(nameEdit, nameEdit->text().trimmed().length() >= 2);
        check(categoryEdit, categoryEdit->text().trimmed().length() >= 2);
        bool okP; double p = priceEdit->text().toDouble(&okP);
        check(priceEdit, okP && p > 0);
        bool okS; int s = stockEdit->text().toInt(&okS);
        check(stockEdit, okS && s >= 0);
        btnSave->setEnabled(allValid);
    };

    connect(nameEdit, &QLineEdit::textChanged, validateForm);
    connect(categoryEdit, &QLineEdit::textChanged, validateForm);
    connect(priceEdit, &QLineEdit::textChanged, validateForm);
    connect(stockEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [modifierDialog, id, nameEdit, categoryEdit,
                                               priceEdit, stockEdit, statusCombo,
                                               dateCreationEdit, this]() {
        QSqlQuery query;
        query.prepare("UPDATE PRODUIT SET NOM = :nom, CATEGORIE = :categorie, PRIX = :prix, "
                      "QUANTITE_STOCK = :stock, STATUT = :statut, DATE_CREATION = TO_DATE(:date, 'DD/MM/YYYY') "
                      "WHERE ID_PRODUIT = :id");
        query.bindValue(":nom", nameEdit->text());
        query.bindValue(":categorie", categoryEdit->text());
        query.bindValue(":prix", priceEdit->text().toDouble());
        query.bindValue(":stock", stockEdit->text().toInt());
        query.bindValue(":statut", statusCombo->currentText());
        query.bindValue(":date", dateCreationEdit->date().toString("dd/MM/yyyy"));
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ Produit mis à jour!");
            loadProducts();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur Oracle", query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
}

void products::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentIndex().row();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un produit à supprimer.");
        return;
    }

    QString id = model->index(currentRow, 0).data().toString();
    QString productName = model->index(currentRow, 1).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer le produit '%1' (ID: %2) ?")
                                 .arg(productName).arg(id),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM PRODUIT WHERE ID_PRODUIT = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Produit supprimé avec succès!");
            loadProducts();
            qDebug() << "Produit supprimé:" << id;
        } else {
            QMessageBox::critical(this, "Erreur Oracle", query.lastError().text());
        }
    }
}

void products::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNameEdit->clear();
    loadProducts();
    QMessageBox::information(this, "Actualisation", "✅ Liste des produits actualisée !");
}

void products::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Produits",
                                                    "Produits_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
                                                    "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }

    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);

    // CSV Header
    QStringList headers = {"ID", "Nom", "Catégorie", "Prix", "Stock", "Statut", "Date Création"};
    out << headers.join(";") << "\n";

    // Table Data
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList line;
        for (int col = 0; col < model->columnCount(); ++col) {
            line << model->index(row, col).data().toString();
        }
        out << line.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Succès", "✅ Fichier CSV exporté avec succès !");
}
void products::on_pushButton_9_clicked()
{
    int actif = 0, arrete = 0, attente = 0;

    QSqlQuery q("SELECT STATUT, COUNT(*) FROM PRODUIT GROUP BY STATUT");
    while(q.next()){
        QString s = q.value(0).toString();
        int count = q.value(1).toInt();
        if (s == "Actif") actif = count;
        else if (s == "Arrêté") arrete = count;
        else if (s == "En attente") attente = count;
    }

    int total = actif + arrete + attente;
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun produit en base.");
        return;
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Produits");
    statsDialog->resize(800, 600);
    statsDialog->setStyleSheet("QDialog { background-color: #f1e7dc; border: 2px dashed #b08a6b; border-radius: 20px; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *header = new QLabel(QString("📊 STATISTIQUES PRODUITS (TOTAL: %1)").arg(total));
    header->setStyleSheet("font-size: 20px; font-weight: bold; color: #5b2f1d; margin-bottom: 20px;");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QHBoxLayout *chartLayout = new QHBoxLayout();
    chartLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    struct Stat { QString label; int val; QColor color; };
    QList<Stat> stats = { {"Actif", actif, QColor(100, 200, 100)}, {"Arrêté", arrete, QColor(200, 100, 100)}, {"Attente", attente, QColor(255, 255, 100)} };

    for (const auto &s : stats) {
        if (s.val == 0) continue;
        QVBoxLayout *barCol = new QVBoxLayout();
        QLabel *lVal = new QLabel(QString::number(s.val));
        lVal->setAlignment(Qt::AlignCenter);
        
        QFrame *bar = new QFrame();
        bar->setFixedWidth(80);
        bar->setFixedHeight(qMax(20, s.val * 300 / (total == 0 ? 1 : qMax(1, qMax(qMax(actif, arrete), attente)))));
        bar->setStyleSheet(QString("background-color: %1; border: 2px solid #3a1f14; border-radius: 10px;").arg(s.color.name()));
        
        QLabel *lName = new QLabel(s.label);
        lName->setAlignment(Qt::AlignCenter);
        lName->setStyleSheet("font-weight: bold;");

        barCol->addWidget(lVal);
        barCol->addWidget(bar);
        barCol->addWidget(lName);
        chartLayout->addLayout(barCol);
    }
    mainLayout->addLayout(chartLayout);

    QPushButton *close = new QPushButton("Fermer");
    close->setStyleSheet("background-color: #7a4a2e; color: white; padding: 10px; border-radius: 10px;");
    connect(close, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(close, 0, Qt::AlignCenter);

    statsDialog->exec();
}
