#include "pagemachine.h"
#include "ui_pagemachine.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include "commandes.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "products.h"
#include "matieres.h"
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include "smtp.h"


pagemachine::pagemachine(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagemachine)
{
    ui->setupUi(this);
    model = new CustomSqlModel(this);
    setupMachinesTable();
    loadMachines();
    setupSearch();
}

pagemachine::~pagemachine()
{
    delete ui;
}

void pagemachine::setupSearch()
{
    ui->searchIdEdit->setPlaceholderText("Chercher par État...");
    ui->searchNomEdit->setPlaceholderText("Chercher par Réf...");
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
}

void pagemachine::filterTable()
{
    QString etatFilter = ui->searchIdEdit->text();
    QString refFilter = ui->searchNomEdit->text();

    QString sql = "SELECT ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE FROM MACHINE WHERE 1=1";
    
    if (!etatFilter.isEmpty()) {
        sql += " AND LOWER(ETAT) LIKE LOWER('%" + etatFilter + "%')";
    }
    if (!refFilter.isEmpty()) {
        sql += " AND LOWER(REF) LIKE LOWER('%" + refFilter + "%')";
    }

    model->setQuery(sql);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de filtrage: " << model->lastError().text();
    }
}

void pagemachine::setupMachinesTable()
{
    ui->tableWidget->setModel(model);
    
    // Set headers through the model
    model->setHeaderData(0, Qt::Horizontal, "ID Machine");
    model->setHeaderData(1, Qt::Horizontal, "Référence");
    model->setHeaderData(2, Qt::Horizontal, "Nom Machine");
    model->setHeaderData(3, Qt::Horizontal, "Type Machine");
    model->setHeaderData(4, Qt::Horizontal, "État Machine");
    model->setHeaderData(5, Qt::Horizontal, "Capacité");
    model->setHeaderData(6, Qt::Horizontal, "Fréquence");
    model->setHeaderData(7, Qt::Horizontal, "Niveau de Charge");

    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 130);
    ui->tableWidget->setColumnWidth(3, 130);
    ui->tableWidget->setColumnWidth(4, 110);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setColumnWidth(6, 100);
    ui->tableWidget->setColumnWidth(7, 120);

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
}

void pagemachine::loadMachines()
{
    model->setQuery("SELECT ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE FROM MACHINE");
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de chargement des machines: " << model->lastError().text();
        QMessageBox::warning(this, "Erreur Base de données", "Impossible de charger les machines.");
    }
}

void pagemachine::on_pushButton_6_clicked()
{
    hide();
    commandes *lg = new commandes(this);
    lg->show();
}

void pagemachine::on_pushButton_11_clicked()
{
    hide();
    pageemployee *pl = new pageemployee(this);
    pl->show();
}

void pagemachine::on_pushButton_20_clicked()
{
    hide();
    fournisseurs *pf = new fournisseurs(this);
    pf->show();
}

void pagemachine::on_pushButton_21_clicked()
{
    hide();
    products *pd = new products(this);
    pd->show();
}

void pagemachine::on_pushButton_22_clicked()
{
    hide();
    Matieres *mm = new Matieres(this);
    mm->show();
}

void pagemachine::on_pushButton_23_clicked()
{
    hide();
    pagemachine *ss = new pagemachine(this);
    ss->show();
}

void pagemachine::on_pushButton_clicked()
{
    QDialog *ajoutDialog = new QDialog(this);
    ajoutDialog->setWindowTitle("Ajouter une machine");
    ajoutDialog->setFixedSize(650, 800);
    ajoutDialog->setModal(true);

    ajoutDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   margin-bottom: 15px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QLineEdit, QComboBox, QSpinBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 15px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "   font-size: 15px;"
        "   min-width: 150px;"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(ajoutDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("AJOUTER UNE MACHINE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

    QString newId = "M001";
    QSqlQuery queryId("SELECT ID_MACHINE FROM MACHINE");
    int maxNum = 0;
    while (queryId.next()) {
        QString currentId = queryId.value(0).toString();
        if (currentId.startsWith("M", Qt::CaseInsensitive)) {
            int num = currentId.mid(1).toInt();
            if (num > maxNum) {
                maxNum = num;
            }
        }
    }
    newId = QString("M%1").arg(maxNum + 1, 3, 10, QChar('0'));
    idEdit->setText(newId);

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("Référence");
    refEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom de la machine");
    nomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});

    QLineEdit *capaciteEdit = new QLineEdit();
    capaciteEdit->setPlaceholderText("Capacité (ex: 1000 unités/h)");
    capaciteEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *frequenceEdit = new QLineEdit();
    frequenceEdit->setPlaceholderText("Fréquence (ex: 50 Hz)");
    frequenceEdit->setAlignment(Qt::AlignCenter);

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setValue(50);
    chargeSpinBox->setSuffix(" %");
    chargeSpinBox->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel("🔑 ID Machine :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📌 Référence :"));
    mainLayout->addWidget(refEdit);
    mainLayout->addWidget(new QLabel("🏭 Nom Machine :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("⚙️ Type Machine :"));
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(new QLabel("📊 État Machine :"));
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(new QLabel("📦 Capacité :"));
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(new QLabel("📈 Fréquence :"));
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(new QLabel("🔋 Niveau de Charge :"));
    mainLayout->addWidget(chargeSpinBox);

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
        auto checkField = [&](QLineEdit* edit, bool condition) {
            if (condition) {
                edit->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            } else {
                edit->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;");
                allValid = false;
            }
        };

        checkField(refEdit, refEdit->text().trimmed().length() >= 2);
        checkField(nomEdit, nomEdit->text().trimmed().length() >= 2);
        bool okCap; int cap = capaciteEdit->text().toInt(&okCap);
        checkField(capaciteEdit, okCap && cap > 0);
        bool okFreq; int freq = frequenceEdit->text().toInt(&okFreq);
        checkField(frequenceEdit, okFreq && freq > 0);

        btnSave->setEnabled(allValid);
    };

    connect(refEdit, &QLineEdit::textChanged, validateForm);
    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(capaciteEdit, &QLineEdit::textChanged, validateForm);
    connect(frequenceEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnSave, &QPushButton::clicked, [=]() {
        QSqlQuery query;
        query.prepare("INSERT INTO MACHINE (ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE) "
                      "VALUES (:id, :ref, :nom, :type, :etat, :capacite, :frequence, :charge)");
        query.bindValue(":id", idEdit->text());
        query.bindValue(":ref", refEdit->text());
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":capacite", capaciteEdit->text());
        query.bindValue(":frequence", frequenceEdit->text());
        query.bindValue(":charge", QString::number(chargeSpinBox->value()));

        if (query.exec()) {
            QMessageBox::information(ajoutDialog, "Succès", "✅ La machine a été ajoutée avec succès!");
            loadMachines();
            ajoutDialog->accept();
        } else {
            QMessageBox::critical(ajoutDialog, "Erreur", query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
    ajoutDialog->exec();
    ajoutDialog->deleteLater();
}

void pagemachine::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentIndex().row();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine à modifier.");
        return;
    }

    QString id = model->index(currentRow, 0).data().toString();
    QString ref = model->index(currentRow, 1).data().toString();
    QString nom = model->index(currentRow, 2).data().toString();
    QString type = model->index(currentRow, 3).data().toString();
    QString etat = model->index(currentRow, 4).data().toString();
    QString capacite = model->index(currentRow, 5).data().toString();
    QString frequence = model->index(currentRow, 6).data().toString();
    QString niveauCharge = model->index(currentRow, 7).data().toString().replace("%", "");

    QDialog *modifierDialog = new QDialog(this);
    modifierDialog->setWindowTitle("Modifier machine");
    modifierDialog->setFixedSize(650, 800);
    modifierDialog->setModal(true);

    modifierDialog->setStyleSheet(
        "QDialog {"
        "   background-color: #f4ede6;"
        "   border: 3px dashed #c9b2a2;"
        "   border-radius: 20px;"
        "}"
        "QLabel#headerLabel {"
        "   color: #6b3e26;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   margin-bottom: 15px;"
        "}"
        "QLabel { color: #3a2a20; font-weight: bold; font-size: 14px; }"
        "QLineEdit, QComboBox, QSpinBox {"
        "   background-color: #fffaf5;"
        "   border: 1px solid #c9b2a2;"
        "   border-radius: 10px;"
        "   padding: 12px;"
        "   color: #3a2a20;"
        "   font-size: 14px;"
        "   min-height: 20px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus { border: 2px solid #6b3e26; }"
        "QPushButton {"
        "   border-radius: 10px; padding: 15px; font-weight: bold; color: white;"
        "   border-bottom: 3px solid rgba(0,0,0,0.2);"
        "   font-size: 15px;"
        "   min-width: 150px;"
        "}"
        "QPushButton#btnSave { background-color: #6f8f3d; }"
        "QPushButton#btnCancel { background-color: #b3a398; color: #3a2a20; }"
        "QPushButton:pressed { margin-top: 3px; border-bottom: 1px solid rgba(0,0,0,0.2); }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(modifierDialog);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(15);

    QLabel *header = new QLabel("MODIFIER MACHINE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(id);
    idEdit->setAlignment(Qt::AlignCenter);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setText(ref);
    refEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setText(nom);
    nomEdit->setAlignment(Qt::AlignCenter);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Fraiseuse", "Tour", "Perceuse", "Presse", "Scie", "Rectifieuse", "Imprimante 3D", "Autre"});
    typeCombo->setCurrentText(type);

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Fonctionnelle", "Maintenance", "En panne", "Arrêtée"});
    etatCombo->setCurrentText(etat);

    QLineEdit *capaciteEdit = new QLineEdit();
    capaciteEdit->setText(capacite);
    capaciteEdit->setAlignment(Qt::AlignCenter);

    QLineEdit *frequenceEdit = new QLineEdit();
    frequenceEdit->setText(frequence);
    frequenceEdit->setAlignment(Qt::AlignCenter);

    QSpinBox *chargeSpinBox = new QSpinBox();
    chargeSpinBox->setRange(0, 100);
    chargeSpinBox->setValue(niveauCharge.toInt());
    chargeSpinBox->setSuffix(" %");
    chargeSpinBox->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel("🔑 ID Machine :"));
    mainLayout->addWidget(idEdit);
    mainLayout->addWidget(new QLabel("📌 Référence :"));
    mainLayout->addWidget(refEdit);
    mainLayout->addWidget(new QLabel("🏭 Nom Machine :"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(new QLabel("⚙️ Type Machine :"));
    mainLayout->addWidget(typeCombo);
    mainLayout->addWidget(new QLabel("📊 État Machine :"));
    mainLayout->addWidget(etatCombo);
    mainLayout->addWidget(new QLabel("📦 Capacité :"));
    mainLayout->addWidget(capaciteEdit);
    mainLayout->addWidget(new QLabel("📈 Fréquence :"));
    mainLayout->addWidget(frequenceEdit);
    mainLayout->addWidget(new QLabel("🔋 Niveau de Charge :"));
    mainLayout->addWidget(chargeSpinBox);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    QPushButton *btnUpdate = new QPushButton("💾 Mettre à jour");
    btnUpdate->setObjectName("btnSave");
    QPushButton *btnCancel = new QPushButton("❌ Annuler");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnUpdate);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    auto validateForm = [=]() {
        bool allValid = true;
        auto checkField = [&](QLineEdit* edit, bool condition) {
            if (condition) {
                edit->setStyleSheet("border: 2px solid #6f8f3d; background-color: #fafffa; padding: 12px; border-radius: 10px;");
            } else {
                edit->setStyleSheet("border: 2px solid #c0392b; background-color: #fff5f5; padding: 12px; border-radius: 10px;");
                allValid = false;
            }
        };
        checkField(refEdit, refEdit->text().trimmed().length() >= 2);
        checkField(nomEdit, nomEdit->text().trimmed().length() >= 2);
        bool okCap; int cap = capaciteEdit->text().toInt(&okCap);
        checkField(capaciteEdit, okCap && cap > 0);
        bool okFreq; int freq = frequenceEdit->text().toInt(&okFreq);
        checkField(frequenceEdit, okFreq && freq > 0);

        btnUpdate->setEnabled(allValid);
    };

    connect(refEdit, &QLineEdit::textChanged, validateForm);
    connect(nomEdit, &QLineEdit::textChanged, validateForm);
    connect(capaciteEdit, &QLineEdit::textChanged, validateForm);
    connect(frequenceEdit, &QLineEdit::textChanged, validateForm);
    validateForm();

    connect(btnUpdate, &QPushButton::clicked, [=]() {
        QSqlQuery query;
        query.prepare("UPDATE MACHINE SET REF = :ref, NOM = :nom, TYPE = :type, ETAT = :etat, "
                      "CAPACITE = :capacite, FREQUENCE = :frequence, NIVEAU_DE_CHARGE = :charge "
                      "WHERE ID_MACHINE = :id");
        query.bindValue(":ref", refEdit->text());
        query.bindValue(":nom", nomEdit->text());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":capacite", capaciteEdit->text());
        query.bindValue(":frequence", frequenceEdit->text());
        query.bindValue(":charge", QString::number(chargeSpinBox->value()));
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(modifierDialog, "Succès", "✅ La machine a été mise à jour avec succès!");
            loadMachines();
            modifierDialog->accept();
        } else {
            QMessageBox::critical(modifierDialog, "Erreur", "Erreur lors de la mise à jour:\n" + query.lastError().text());
        }
    });

    connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
    modifierDialog->exec();
    modifierDialog->deleteLater();
}

void pagemachine::on_pushButton_3_clicked()
{
    int currentRow = ui->tableWidget->currentIndex().row();

    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine à supprimer.");
        return;
    }

    QString id = model->index(currentRow, 0).data().toString();
    QString nom = model->index(currentRow, 2).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation de suppression",
                                 QString("Voulez-vous vraiment supprimer la machine '%1' (ID: %2) ?")
                                 .arg(nom).arg(id),
                                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM MACHINE WHERE ID_MACHINE = :id");
        query.bindValue(":id", id);
        
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "✅ Machine supprimée avec succès!");
            loadMachines();
            qDebug() << "Machine supprimée:" << id << "-" << nom;
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        }
    }
}

void pagemachine::on_pushButton_4_clicked()
{
    ui->searchIdEdit->clear();
    ui->searchNomEdit->clear();
    loadMachines();
    QMessageBox::information(this, "Actualisation", "✅ Liste des machines actualisée !");
}

void pagemachine::on_pushButton_7_clicked()
{
    int currentRow = ui->tableWidget->currentIndex().row();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection requise",
                           "Veuillez sélectionner une machine dans le tableau pour générer la demande de maintenance.");
        return;
    }

    QString id = model->index(currentRow, 0).data().toString();
    QString ref = model->index(currentRow, 1).data().toString();
    QString nom = model->index(currentRow, 2).data().toString();
    QString type = model->index(currentRow, 3).data().toString();
    QString etat = model->index(currentRow, 4).data().toString();

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter Demande de Maintenance",
                                                    "Demande_Maintenance_" + ref + ".pdf",
                                                    "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument doc;
    QString html = "<h1>Demande de Maintenance / Diagnostique</h1>"
                   "<hr>"
                   "<h3>Détails de la machine</h3>"
                   "<table border='1' cellspacing='0' cellpadding='5' width='100%'>"
                   "<tr><th align='left'>ID Machine</th><td>" + id + "</td></tr>"
                   "<tr><th align='left'>Référence</th><td>" + ref + "</td></tr>"
                   "<tr><th align='left'>Nom de la Machine</th><td>" + nom + "</td></tr>"
                   "<tr><th align='left'>Type</th><td>" + type + "</td></tr>"
                   "<tr><th align='left'>État Actuel</th><td style='color:red;'><b>" + etat + "</b></td></tr>"
                   "</table>"
                   "<br><br>"
                   "<h3>Description du problème (Diagnostique)</h3>"
                   "<p><i>Remplir manuellement ou suite à l'inspection:</i></p>"
                   "<div style='border: 1px solid black; height: 300px;'></div>"
                   "<br><br>"
                   "<p align='right'><b>Date : </b>" + QDate::currentDate().toString("dd/MM/yyyy") + "</p>"
                   "<p align='right'><b>Signature : </b>___________________</p>";
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "✅ Demande de maintenance générée avec succès !");
}

void pagemachine::on_pushButton_9_clicked()
{
    int fonctionnelle = 0, maintenance = 0, panne = 0, arretee = 0;

    QSqlQuery q;
    q.exec("SELECT ETAT, COUNT(*) FROM MACHINE GROUP BY ETAT");
    while(q.next()){
        QString e = q.value(0).toString();
        int count = q.value(1).toInt();
        if (e == "Fonctionnelle") fonctionnelle = count;
        else if (e == "Maintenance") maintenance = count;
        else if (e == "En panne") panne = count;
        else if (e == "Arrêtée") arretee = count;
    }

    int totalEtat = fonctionnelle + maintenance + panne + arretee;

    if (totalEtat == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune machine à afficher.");
        return;
    }

    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des machines (Par État)");
    statsDialog->resize(900, 650);
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

    QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES MACHINES (PAR ÉTAT)");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *totalLabel = new QLabel(QString("Total machines: %1").arg(totalEtat));
    totalLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #fffaf5;"
        "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
        "border: 1px solid #c49a6c;"
    );
    headerLayout->addWidget(totalLabel);

    mainLayout->addWidget(headerFrame);

    QWidget *etatTab = new QWidget();
    QVBoxLayout *etatLayout = new QVBoxLayout(etatTab);

    QFrame *chartFrame = new QFrame();
    chartFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #fffaf5;"
        "   border: 2px solid #b08a6b;"
        "   border-radius: 15px;"
        "   padding: 10px;"
        "}"
    );
    QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
    chartLayout->setSpacing(20);
    chartLayout->setAlignment(Qt::AlignBottom);

    int maxCount = qMax(qMax(qMax(fonctionnelle, maintenance), panne), arretee);
    maxCount = qMax(maxCount, 1);
    const int maxBarHeight = 300;

    struct EtatData { QString name; int count; QColor color; QString icon; };
    QList<EtatData> etats = {
        {"Fonctionnelle", fonctionnelle, QColor(100, 200, 100), "✅"},
        {"Maintenance", maintenance, QColor(255, 255, 100), "🔧"},
        {"En panne", panne, QColor(200, 100, 100), "⚠️"},
        {"Arrêtée", arretee, QColor(150, 150, 150), "⏹️"}
    };

    for (const EtatData &ed : etats) {
        if (ed.count == 0) continue;
        double percent = 100.0 * ed.count / totalEtat;
        int barHeight = (ed.count * maxBarHeight) / maxCount;
        if (barHeight < 10) barHeight = 10;

        QWidget *container = new QWidget();
        QVBoxLayout *barLayout = new QVBoxLayout(container);
        barLayout->setSpacing(8);
        barLayout->setAlignment(Qt::AlignHCenter);

        QLabel *countLabel = new QLabel(QString::number(ed.count));
        countLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #3a2a20;");
        countLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(countLabel);

        QLabel *percentLabel = new QLabel(QString("%1%").arg(percent, 0, 'f', 1));
        percentLabel->setStyleSheet("font-size: 16px; color: #5b2f1d; font-weight: bold;");
        percentLabel->setAlignment(Qt::AlignCenter);
        barLayout->addWidget(percentLabel);

        QFrame *bar = new QFrame();
        bar->setFixedWidth(100);
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
        )").arg(ed.color.lighter(110).name()).arg(ed.color.name()));
        barLayout->addWidget(bar, 0, Qt::AlignCenter);

        QLabel *nameLabel = new QLabel(ed.icon + " " + ed.name);
        nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #3a2a20;");
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setWordWrap(true);
        barLayout->addWidget(nameLabel);

        barLayout->addStretch();
        chartLayout->addWidget(container);
    }

    etatLayout->addWidget(chartFrame);
    mainLayout->addWidget(etatTab);

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
