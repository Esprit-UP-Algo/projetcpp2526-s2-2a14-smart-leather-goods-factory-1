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

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;

        if (!idFilter.isEmpty()) {
            QString id = ui->tableWidget->item(row, 0)->text();
            if (!id.contains(idFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        if (match && !nameFilter.isEmpty()) {
            QString name = ui->tableWidget->item(row, 1)->text();
            if (!name.contains(nameFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }

        ui->tableWidget->setRowHidden(row, !match);
    }
}

void products::setupProductsTable()
{
    QStringList headers = {"ID", "Nom", "Catégorie", "Prix (€)",
                          "Stock", "Statut", "Date création"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 120);
    ui->tableWidget->setColumnWidth(3, 130);
    ui->tableWidget->setColumnWidth(4, 130);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 100);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void products::loadProducts()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID_PRODUIT, NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM PRODUIT");
    
    if (query.exec()) {
        while (query.next()) {
            QDate creationDate = QDate::fromString(query.value(6).toString(), "dd/MM/yyyy");
            
            addProductToTable(
                query.value(0).toString(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString(),
                creationDate
            );
        }
    } else {
        qDebug() << "Erreur de chargement des produits:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les produits:\n" + query.lastError().text());
    }
}

void products::addProductToTable(const QString &id, const QString &name,
                                 const QString &category, const QString &price,
                                 const QString &stock, const QString &status,
                                 const QDate &creationDate)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(name));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(category));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(price + " €"));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(stock));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(status));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(creationDate.toString("dd/MM/yyyy")));

    updateRowColors(row);
}

void products::updateRowColors(int row)
{
    QTableWidgetItem *statusItem = ui->tableWidget->item(row, 5);
    if (statusItem) {
        QString status = statusItem->text();
        if (status == "actif") {
            statusItem->setBackground(QColor(200, 255, 200));
            statusItem->setForeground(QColor(0, 100, 0));
        } else if (status == "arrété") {
            statusItem->setBackground(QColor(255, 200, 200));
            statusItem->setForeground(QColor(139, 0, 0));
        } else if (status == "En attente") {
            statusItem->setBackground(QColor(255, 255, 200));
            statusItem->setForeground(QColor(128, 128, 0));
        }
    }
}

void products::updateProductInTable(int row, const QString &name,
                                    const QString &category, const QString &price,
                                    const QString &stock, const QString &status,
                                    const QDate &creationDate)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        ui->tableWidget->item(row, 1)->setText(name);
        ui->tableWidget->item(row, 2)->setText(category);
        ui->tableWidget->item(row, 3)->setText(price + " €");
        ui->tableWidget->item(row, 4)->setText(stock);
        ui->tableWidget->item(row, 5)->setText(status);
        ui->tableWidget->item(row, 6)->setText(creationDate.toString("dd/MM/yyyy"));

        updateRowColors(row);
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
        QDialog {
            background-color: #f4ede6;
            border: 3px dashed #c9b2a2;
            border-radius: 20px;
        }
        QLabel#headerLabel {
            color: #6b3e26;
            font-size: 22px;
            font-weight: bold;
            margin-bottom: 15px;
        }
        QLabel {
            color: #3a2a20;
            font-weight: bold;
            font-size: 14px;
        }
        QLineEdit, QDateTimeEdit, QComboBox {
            background-color: #fffaf5;
            border: 1px solid #c9b2a2;
            border-radius: 10px;
            padding: 12px;
            color: #3a2a20;
            font-size: 14px;
            min-height: 20px;
        }
        QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus {
            border: 2px solid #6b3e26;
        }
        QPushButton {
            border-radius: 10px;
            padding: 15px;
            font-weight: bold;
            color: white;
            border-bottom: 3px solid rgba(0,0,0,0.2);
            font-size: 15px;
            min-width: 150px;
        }
        QPushButton#btnSave {
            background-color: #6f8f3d;
        }
        QPushButton#btnCancel {
            background-color: #b3a398;
            color: #3a2a20;
        }
        QPushButton:pressed {
            margin-top: 3px;
            border-bottom: 1px solid rgba(0,0,0,0.2);
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("AJOUTER PRODUIT");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setPlaceholderText("ID produit (ex: P001)");
    idEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Nom du produit");
    nameEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *categoryEdit = new QLineEdit();
    categoryEdit->setPlaceholderText("Catégorie");
    categoryEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *priceEdit = new QLineEdit();
    priceEdit->setPlaceholderText("Prix (€)");
    priceEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *stockEdit = new QLineEdit();
    stockEdit->setPlaceholderText("Quantité en stock");
    stockEdit->setAlignment(Qt::AlignCenter);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"En attente", "actif", "arrété"});

    QDateEdit *dateCreationEdit = new QDateEdit(QDate::currentDate());
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📝 Nom :"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(new QLabel("🏷️ Catégorie :"));
    mainLayout->addWidget(categoryEdit);
    mainLayout->addWidget(new QLabel("💰 Prix :"));
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

    connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, nameEdit, categoryEdit,
                                              priceEdit, stockEdit, statusCombo,
                                              dateCreationEdit, this]() {
        if (idEdit->text().isEmpty() || nameEdit->text().isEmpty() ||
            categoryEdit->text().isEmpty() || priceEdit->text().isEmpty() || stockEdit->text().isEmpty()) {
            QMessageBox::warning(ajoutDialog, "Champs manquants",
                               "Veuillez remplir tous les champs obligatoires.");
            return;
        }

        bool ok;
        double price = priceEdit->text().toDouble(&ok);
        if (!ok || price <= 0) {
            QMessageBox::warning(ajoutDialog, "Prix invalide",
                               "Veuillez entrer un prix valide (nombre positif).");
            return;
        }
        
        int stock = stockEdit->text().toInt(&ok);
        if (!ok || stock < 0) {
            QMessageBox::warning(ajoutDialog, "Stock invalide",
                               "Veuillez entrer un stock valide (nombre entier positif ou nul).");
            return;
        }

        QString formattedPrice = QString::number(price, 'f', 2);
        
        QSqlQuery query;
        query.prepare("INSERT INTO PRODUIT (ID_PRODUIT, NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, DATE_CREATION) "
                      "VALUES (:id, :nom, :categorie, :prix, :stock, :statut, TO_DATE(:date_creation, 'DD/MM/YYYY'))");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":nom", nameEdit->text());
        query.bindValue(":categorie", categoryEdit->text());
        query.bindValue(":prix", formattedPrice);
        query.bindValue(":stock", stock);
        query.bindValue(":statut", statusCombo->currentText());
        query.bindValue(":date_creation", dateCreationEdit->date().toString("dd/MM/yyyy"));

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ Le produit a été ajouté avec succès!");
            loadProducts();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void products::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un produit à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString name = ui->tableWidget->item(currentRow, 1)->text();
    QString category = ui->tableWidget->item(currentRow, 2)->text();
    QString price = ui->tableWidget->item(currentRow, 3)->text().replace(" €", "");
    QString stock = ui->tableWidget->item(currentRow, 4)->text();
    QString status = ui->tableWidget->item(currentRow, 5)->text();
    QDate creationDate = QDate::fromString(ui->tableWidget->item(currentRow, 6)->text(), "dd/MM/yyyy");

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier produit");
    modifierDialog->setFixedSize(650, 800);
    modifierDialog->setModal(true);

    modifierDialog->setStyleSheet(R"(
        QDialog {
            background-color: #f4ede6;
            border: 3px dashed #c9b2a2;
            border-radius: 20px;
        }
        QLabel#headerLabel {
            color: #6b3e26;
            font-size: 22px;
            font-weight: bold;
            margin-bottom: 15px;
        }
        QLabel {
            color: #3a2a20;
            font-weight: bold;
            font-size: 14px;
        }
        QLineEdit, QDateTimeEdit, QComboBox {
            background-color: #fffaf5;
            border: 1px solid #c9b2a2;
            border-radius: 10px;
            padding: 12px;
            color: #3a2a20;
            font-size: 14px;
            min-height: 20px;
        }
        QLineEdit:focus, QDateTimeEdit:focus, QComboBox:focus {
            border: 2px solid #6b3e26;
        }
        QPushButton {
            border-radius: 10px;
            padding: 15px;
            font-weight: bold;
            color: white;
            border-bottom: 3px solid rgba(0,0,0,0.2);
            font-size: 15px;
            min-width: 150px;
        }
        QPushButton#btnSave {
            background-color: #6f8f3d;
        }
        QPushButton#btnCancel {
            background-color: #b3a398;
            color: #3a2a20;
        }
        QPushButton:pressed {
            margin-top: 3px;
            border-bottom: 1px solid rgba(0,0,0,0.2);
        }
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
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

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
    statusCombo->addItems({"En attente", "actif", "arrété"});
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
    mainLayout->addWidget(new QLabel("💰 Prix :"));
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

    connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, id, nameEdit, categoryEdit,
                                              priceEdit, stockEdit, statusCombo,
                                              dateCreationEdit, this]() {
        if (nameEdit->text().isEmpty() || categoryEdit->text().isEmpty() ||
            priceEdit->text().isEmpty() || stockEdit->text().isEmpty()) {
            QMessageBox::warning(modifierDialog, "Champs manquants",
                               "Veuillez remplir tous les champs.");
            return;
        }

        bool ok;
        double parsedPrice = priceEdit->text().toDouble(&ok);
        if (!ok || parsedPrice <= 0) {
            QMessageBox::warning(modifierDialog, "Prix invalide",
                               "Veuillez entrer un prix valide (nombre positif).");
            return;
        }
        
        int parsedStock = stockEdit->text().toInt(&ok);
        if (!ok || parsedStock < 0) {
            QMessageBox::warning(modifierDialog, "Stock invalide",
                               "Veuillez entrer un stock valide (nombre entier positif ou nul).");
            return;
        }

        QString formattedPrice = QString::number(parsedPrice, 'f', 2);

        QSqlQuery query;
        query.prepare("UPDATE PRODUIT SET NOM = :nom, CATEGORIE = :categorie, PRIX = :prix, "
                      "QUANTITE_STOCK = :stock, STATUT = :statut, DATE_CREATION = TO_DATE(:date_creation, 'DD/MM/YYYY') "
                      "WHERE ID_PRODUIT = :id");
        query.bindValue(":nom", nameEdit->text());
        query.bindValue(":categorie", categoryEdit->text());
        query.bindValue(":prix", formattedPrice);
        query.bindValue(":stock", parsedStock);
        query.bindValue(":statut", statusCombo->currentText());
        query.bindValue(":date_creation", dateCreationEdit->date().toString("dd/MM/yyyy"));
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ Le produit a été mis à jour avec succès!");
            loadProducts();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
    modifierDialog->deleteLater();
}

void products::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner un produit à supprimer.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString productName = ui->tableWidget->item(currentRow, 1)->text();

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
            qDebug() << "Produit supprimé:" << id << "- Nom:" << productName;
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
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
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                    "produits_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
                                                    "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }

    QTextStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out.setGenerateByteOrderMark(true);

    QStringList headers = {"ID", "Nom", "Catégorie", "Date commande",
                           "Date livraison", "Statut", "Prix"};
    for (int i = 0; i < headers.size(); ++i) {
        if (i > 0) out << ";";
        out << headers[i];
    }
    out << "\n";

    int visibleRows = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (ui->tableWidget->isRowHidden(row)) continue;

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            if (col > 0) out << ";";
            QString cell = ui->tableWidget->item(row, col)->text();
            if (cell.contains(';') || cell.contains('"') || cell.contains('\n')) {
                cell.replace("\"", "\"\"");
                cell = "\"" + cell + "\"";
            }
            out << cell;
        }
        out << "\n";
        visibleRows++;
    }

    file.close();
    QMessageBox::information(this, "Succès",
                             QString("✅ Fichier CSV exporté avec succès !\n%1 ligne(s) exportée(s).")
                             .arg(visibleRows));
}
void products::on_pushButton_9_clicked()
{
    int actif = 0, arrete = 0, attente = 0;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString status = ui->tableWidget->item(row, 5)->text();
            if (status == "actif") actif++;
            else if (status == "arrété") arrete++;
            else if (status == "En attente") attente++;
        }
    }

    int total = actif + arrete + attente;
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun produit à afficher.");
        return;
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des produits");
    statsDialog->resize(800, 600);
    statsDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f1e7dc;"
        "   border: 2px dashed #b08a6b;"
        "   border-radius: 20px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QFrame *headerFrame = new QFrame();
    headerFrame->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6b3e26, stop:1 #4a2717);"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   padding: 10px;"
        "}"
    );
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES PRODUITS");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *totalLabel = new QLabel(QString("Total: %1").arg(total));
    totalLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #fffaf5;"
        "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
        "border: 1px solid #c49a6c;"
    );
    headerLayout->addWidget(totalLabel);

    mainLayout->addWidget(headerFrame);

    QFrame *chartFrame = new QFrame();
    chartFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #fffaf5;"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 20px;"
        "   padding: 20px;"
        "}"
    );
    QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
    chartLayout->setSpacing(20);
    chartLayout->setAlignment(Qt::AlignBottom);

    int maxCount = qMax(qMax(actif, arrete), attente);
    maxCount = qMax(maxCount, 1);
    const int maxBarHeight = 200;

    struct StatusData { QString name; int count; QColor color; QString icon; };
    QList<StatusData> statuses = {
        {"Actif", actif, QColor(100, 200, 100), "✅"},
        {"Arrêté", arrete, QColor(200, 100, 100), "⛔"},
        {"En attente", attente, QColor(255, 255, 100), "⏳"}
    };

    for (const StatusData &sd : statuses) {
        if (sd.count == 0) continue;
        double percent = 100.0 * sd.count / total;
        int barHeight = (sd.count * maxBarHeight) / maxCount;
        if (barHeight < 10) barHeight = 10;

        QWidget *container = new QWidget();
        QVBoxLayout *barLayout = new QVBoxLayout(container);
        barLayout->setSpacing(8);
        barLayout->setAlignment(Qt::AlignHCenter);

        QLabel *countLabel = new QLabel(QString::number(sd.count));
        countLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3a2a20;");
        countLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(countLabel);

        QLabel *percentLabel = new QLabel(QString("%1%").arg(percent, 0, 'f', 1));
        percentLabel->setStyleSheet("font-size: 14px; color: #5b2f1d; font-weight: bold;");
        percentLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(percentLabel);

        QFrame *bar = new QFrame();
        bar->setFixedWidth(80);
        bar->setFixedHeight(barHeight);
        bar->setStyleSheet(QString(R"(
            QFrame {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 %1,
                                            stop:1 %2);
                border: 2px solid #3a1f14;
                border-radius: 12px;
                border-bottom: 4px solid #2a150e;
            }
        )").arg(sd.color.lighter(110).name()).arg(sd.color.name()));
        barLayout->addWidget(bar, 0, Qt::AlignCenter);

        QLabel *nameLabel = new QLabel(sd.icon + " " + sd.name);
        nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3a2a20;");
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setWordWrap(true);
        barLayout->addWidget(nameLabel);

        barLayout->addStretch();
        chartLayout->addWidget(container);
    }

    mainLayout->addWidget(chartFrame);

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a4a2e, stop:1 #5b2f1d);"
        "   border: 2px solid #3a1f14;"
        "   border-radius: 20px;"
        "   padding: 12px 40px;"
        "   font-weight: bold;"
        "   font-size: 16px;"
        "   color: #fffaf5;"
        "   border-bottom: 4px solid #2a150e;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5a3a, stop:1 #6b3e26);"
        "}"
        "QPushButton:pressed {"
        "   margin-top: 2px;"
        "   border-bottom: 2px solid #2a150e;"
        "}"
    );
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    statsDialog->exec();
    delete statsDialog;
}
