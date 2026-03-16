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

    #include "commandes.h"
    #include "pageemployee.h"
    #include "login.h"
    #include "products.h"
    #include "matieres.h"
    #include "pagemachine.h"
    #include <QFile>
    #include <QTextStream>


fournisseurs::fournisseurs(int idEmploye, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::fournisseurs),
    m_idEmploye(idEmploye)
{
    ui->setupUi(this);
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

        for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
            bool match = true;

            if (!idFilter.isEmpty()) {
                QString id = ui->tableWidget->item(row, 0)->text();
                if (!id.contains(idFilter, Qt::CaseInsensitive)) {
                    match = false;
                }
            }

            if (match && !nomFilter.isEmpty()) {
                QString nom = ui->tableWidget->item(row, 1)->text();
                if (!nom.contains(nomFilter, Qt::CaseInsensitive)) {
                    match = false;
                }
            }

            ui->tableWidget->setRowHidden(row, !match);
        }
    }

    void fournisseurs::setupFournisseursTable()
    {
        QStringList headers = {"ID", "Nom", "Type matière", "Téléphone",
                              "Adresse", "Délai livraison", "Qualité", "Statut"};
        ui->tableWidget->setColumnCount(headers.size());
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        ui->tableWidget->setColumnWidth(0, 80);
        ui->tableWidget->setColumnWidth(1, 150);
        ui->tableWidget->setColumnWidth(2, 120);
        ui->tableWidget->setColumnWidth(3, 120);
        ui->tableWidget->setColumnWidth(4, 200);
        ui->tableWidget->setColumnWidth(5, 120);
        ui->tableWidget->setColumnWidth(6, 100);
        ui->tableWidget->setColumnWidth(7, 80);

        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableWidget->setAlternatingRowColors(true);
    }

    void fournisseurs::loadFournisseurs()
    {
        ui->tableWidget->setRowCount(0);

        QList<QStringList> sampleData = {
            {"F001", "Cuirs Premium", "Cuir", "0123456789", "15 Rue des Tanneurs, Paris", "15/06/2024", "Qualité A", "Actif"},
            {"F002", "Tissus Lyon", "Tissu", "0478654321", "8 Avenue de la République, Lyon", "20/06/2024", "Qualité B", "Actif"},
            {"F003", "Métaux Industries", "Métal", "0491325678", "23 Boulevard Industriel, Marseille", "25/06/2024", "Qualité A", "Actif"},
            {"F004", "Plastiques Modernes", "Plastique", "0387654321", "5 Rue de la Plasturgie, Lille", "30/06/2024", "Qualité C", "Inactif"},
            {"F005", "Bois & Cie", "Bois", "0256789123", "12 Place du Bois, Bordeaux", "10/07/2024", "Qualité B", "Actif"}
        };

        for (const QStringList &rowData : sampleData) {
            addFournisseurToTable(rowData[0], rowData[1], rowData[2], rowData[3],
                                 rowData[4], rowData[5], rowData[6], rowData[7]);
        }
    }

    void fournisseurs::addFournisseurToTable(const QString &id, const QString &nom,
                                             const QString &typeMatiere, const QString &telephone,
                                             const QString &adresse, const QString &delaiLivraison,
                                             const QString &qualite, const QString &statut)
    {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(nom));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(typeMatiere));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(telephone));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(adresse));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(delaiLivraison));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(qualite));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(statut));

        updateRowColors(row);
    }

    void fournisseurs::updateRowColors(int row)
    {
        QTableWidgetItem *statutItem = ui->tableWidget->item(row, 7);
        if (statutItem) {
            QString statut = statutItem->text();
            if (statut == "Actif") {
                statutItem->setBackground(QColor(200, 255, 200));
                statutItem->setForeground(QColor(0, 100, 0));
            } else if (statut == "Inactif") {
                statutItem->setBackground(QColor(255, 200, 200));
                statutItem->setForeground(QColor(139, 0, 0));
            }
        }

        QTableWidgetItem *qualiteItem = ui->tableWidget->item(row, 6);
        if (qualiteItem) {
            QString qualite = qualiteItem->text();
            if (qualite.contains("A")) {
                qualiteItem->setBackground(QColor(200, 230, 255));
            } else if (qualite.contains("B")) {
                qualiteItem->setBackground(QColor(255, 255, 200));
            } else if (qualite.contains("C")) {
                qualiteItem->setBackground(QColor(255, 200, 230));
            }
        }
    }

    void fournisseurs::updateFournisseurInTable(int row, const QString &nom,
                                                const QString &typeMatiere, const QString &telephone,
                                                const QString &adresse, const QString &delaiLivraison,
                                                const QString &qualite, const QString &statut)
    {
        if (row >= 0 && row < ui->tableWidget->rowCount()) {
            ui->tableWidget->item(row, 1)->setText(nom);
            ui->tableWidget->item(row, 2)->setText(typeMatiere);
            ui->tableWidget->item(row, 3)->setText(telephone);
            ui->tableWidget->item(row, 4)->setText(adresse);
            ui->tableWidget->item(row, 5)->setText(delaiLivraison);
            ui->tableWidget->item(row, 6)->setText(qualite);
            ui->tableWidget->item(row, 7)->setText(statut);

            updateRowColors(row);
        }
    }


    void fournisseurs::on_pushButton_17_clicked()
    {
        hide();
        pageemployee *pl = new pageemployee(m_idEmploye, this);
        pl->show();
    }

    void fournisseurs::on_pushButton_15_clicked()
    {
        hide();
        login *lg = new login(this);
        lg->show();
    }

    void fournisseurs::on_pushButton_20_clicked()
    {
        hide();
        fournisseurs *fr = new fournisseurs(m_idEmploye, this);
        fr->show();
    }

    void fournisseurs::on_pushButton_16_clicked()
    {
        hide();
        commandes *lg = new commandes(m_idEmploye, this);
        lg->show();
    }

    void fournisseurs::on_pushButton_21_clicked()
    {
        hide();
        products *pd = new products(m_idEmploye, this);
        pd->show();
    }

    void fournisseurs::on_pushButton_22_clicked()
    {
        hide();
        Matieres *pddd = new Matieres(m_idEmploye, this);
        pddd->show();
    }

    void fournisseurs::on_pushButton_23_clicked()
    {
        hide();
        pagemachine *ss = new pagemachine(m_idEmploye, this);
        ss->show();
    }

    void fournisseurs::on_pushButton_clicked()
    {
        QDialog *ajoutDialog = new QDialog(this);
        ajoutDialog->setWindowTitle("Ajouter un fournisseur");
        ajoutDialog->setFixedSize(700, 850);
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
            "QLineEdit, QComboBox, QDateEdit {"
            "   background-color: #fffaf5;"
            "   border: 1px solid #c9b2a2;"
            "   border-radius: 10px;"
            "   padding: 12px;"
            "   color: #3a2a20;"
            "   font-size: 14px;"
            "   min-height: 20px;"
            "}"
            "QLineEdit:focus, QComboBox:focus, QDateEdit:focus { border: 2px solid #6b3e26; }"
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

        QLabel *header = new QLabel("AJOUTER UN FOURNISSEUR");
        header->setObjectName("headerLabel");
        header->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(header);

        QLineEdit *idEdit = new QLineEdit();
        idEdit->setPlaceholderText("ID fournisseur (ex: F001)");
        idEdit->setAlignment(Qt::AlignCenter);

        QLineEdit *nomEdit = new QLineEdit();
        nomEdit->setPlaceholderText("Nom du fournisseur");
        nomEdit->setAlignment(Qt::AlignCenter);

        QComboBox *typeCombo = new QComboBox();
        typeCombo->addItems({"Cuir", "Tissu", "Métal", "Plastique", "Bois", "Verre"});

        QLineEdit *telephoneEdit = new QLineEdit();
        telephoneEdit->setPlaceholderText("Téléphone");
        telephoneEdit->setAlignment(Qt::AlignCenter);

        QLineEdit *adresseEdit = new QLineEdit();
        adresseEdit->setPlaceholderText("Adresse complète");
        adresseEdit->setAlignment(Qt::AlignCenter);

        QDateEdit *delaiDate = new QDateEdit(QDate::currentDate().addDays(15));
        delaiDate->setDisplayFormat("dd/MM/yyyy");
        delaiDate->setCalendarPopup(true);

        QComboBox *qualiteCombo = new QComboBox();
        qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});

        QComboBox *statutCombo = new QComboBox();
        statutCombo->addItems({"Actif", "Inactif"});

        mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
        mainLayout->addWidget(idEdit);
        mainLayout->addWidget(new QLabel("🏢 Nom du fournisseur :"));
        mainLayout->addWidget(nomEdit);
        mainLayout->addWidget(new QLabel("📦 Type de matière :"));
        mainLayout->addWidget(typeCombo);
        mainLayout->addWidget(new QLabel("📞 Téléphone :"));
        mainLayout->addWidget(telephoneEdit);
        mainLayout->addWidget(new QLabel("📍 Adresse :"));
        mainLayout->addWidget(adresseEdit);
        mainLayout->addWidget(new QLabel("📅 Délai de livraison :"));
        mainLayout->addWidget(delaiDate);
        mainLayout->addWidget(new QLabel("⭐ Qualité matière :"));
        mainLayout->addWidget(qualiteCombo);
        mainLayout->addWidget(new QLabel("⚡ Statut :"));
        mainLayout->addWidget(statutCombo);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->setSpacing(20);
        QPushButton *btnSave = new QPushButton("💾 Enregistrer");
        btnSave->setObjectName("btnSave");
        QPushButton *btnCancel = new QPushButton("❌ Annuler");
        btnCancel->setObjectName("btnCancel");
        btnLayout->addWidget(btnSave);
        btnLayout->addWidget(btnCancel);
        mainLayout->addLayout(btnLayout);

        connect(btnSave, &QPushButton::clicked, [ajoutDialog, idEdit, nomEdit, typeCombo,
                                                  telephoneEdit, adresseEdit, delaiDate,
                                                  qualiteCombo, statutCombo, this]() {
            if (idEdit->text().isEmpty() || nomEdit->text().isEmpty() ||
                telephoneEdit->text().isEmpty() || adresseEdit->text().isEmpty()) {
                QMessageBox::warning(ajoutDialog, "Champs manquants",
                                   "Veuillez remplir tous les champs obligatoires.");
                return;
            }

            QString telephone = telephoneEdit->text();
            bool phoneValid = true;
            for (QChar c : telephone) {
                if (!c.isDigit() && c != ' ' && c != '+' && c != '-') {
                    phoneValid = false;
                    break;
                }
            }

            if (!phoneValid) {
                QMessageBox::warning(ajoutDialog, "Téléphone invalide",
                                   "Veuillez entrer un numéro de téléphone valide.");
                return;
            }

            QString delaiFormatted = delaiDate->date().toString("dd/MM/yyyy");

            addFournisseurToTable(idEdit->text(),
                                 nomEdit->text(),
                                 typeCombo->currentText(),
                                 telephoneEdit->text(),
                                 adresseEdit->text(),
                                 delaiFormatted,
                                 qualiteCombo->currentText(),
                                 statutCombo->currentText());

            QMessageBox::information(ajoutDialog, "Succès",
                                   "✅ Le fournisseur a été ajouté avec succès!");

            ajoutDialog->accept();
        });

        connect(btnCancel, &QPushButton::clicked, ajoutDialog, &QDialog::reject);
        ajoutDialog->exec();
        ajoutDialog->deleteLater();
    }

    void fournisseurs::on_pushButton_2_clicked()
    {
        int currentRow = ui->tableWidget->currentRow();

        if (currentRow < 0) {
            QMessageBox::warning(this, "Sélection requise",
                               "Veuillez sélectionner un fournisseur à modifier.");
            return;
        }

        QString id = ui->tableWidget->item(currentRow, 0)->text();
        QString nom = ui->tableWidget->item(currentRow, 1)->text();
        QString typeMatiere = ui->tableWidget->item(currentRow, 2)->text();
        QString telephone = ui->tableWidget->item(currentRow, 3)->text();
        QString adresse = ui->tableWidget->item(currentRow, 4)->text();
        QString delai = ui->tableWidget->item(currentRow, 5)->text();
        QString qualite = ui->tableWidget->item(currentRow, 6)->text();
        QString statut = ui->tableWidget->item(currentRow, 7)->text();

        QDialog *modifierDialog = new QDialog(this);
        modifierDialog->setWindowTitle("Modifier fournisseur");
        modifierDialog->setFixedSize(700, 850);
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
            "QLineEdit, QComboBox, QDateEdit {"
            "   background-color: #fffaf5;"
            "   border: 1px solid #c9b2a2;"
            "   border-radius: 10px;"
            "   padding: 12px;"
            "   color: #3a2a20;"
            "   font-size: 14px;"
            "   min-height: 20px;"
            "}"
            "QLineEdit:focus, QComboBox:focus, QDateEdit:focus { border: 2px solid #6b3e26; }"
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

        QLabel *header = new QLabel("MODIFIER FOURNISSEUR");
        header->setObjectName("headerLabel");
        header->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(header);

        QLineEdit *idEdit = new QLineEdit();
        idEdit->setText(id);
        idEdit->setAlignment(Qt::AlignCenter);
        idEdit->setReadOnly(true);
        idEdit->setStyleSheet("background-color: #e6d8cc; padding: 12px; font-size: 14px;");

        QLineEdit *nomEdit = new QLineEdit();
        nomEdit->setText(nom);
        nomEdit->setAlignment(Qt::AlignCenter);

        QComboBox *typeCombo = new QComboBox();
        typeCombo->addItems({"Cuir", "Tissu", "Métal", "Plastique", "Bois", "Verre"});
        typeCombo->setCurrentText(typeMatiere);

        QLineEdit *telephoneEdit = new QLineEdit();
        telephoneEdit->setText(telephone);
        telephoneEdit->setAlignment(Qt::AlignCenter);

        QLineEdit *adresseEdit = new QLineEdit();
        adresseEdit->setText(adresse);
        adresseEdit->setAlignment(Qt::AlignCenter);

        QDateEdit *delaiDate = new QDateEdit(QDate::fromString(delai, "dd/MM/yyyy"));
        delaiDate->setDisplayFormat("dd/MM/yyyy");
        delaiDate->setCalendarPopup(true);

        QComboBox *qualiteCombo = new QComboBox();
        qualiteCombo->addItems({"Qualité A", "Qualité B", "Qualité C"});
        qualiteCombo->setCurrentText(qualite);

        QComboBox *statutCombo = new QComboBox();
        statutCombo->addItems({"Actif", "Inactif"});
        statutCombo->setCurrentText(statut);

        mainLayout->addWidget(new QLabel("🔑 Identifiant :"));
        mainLayout->addWidget(idEdit);
        mainLayout->addWidget(new QLabel("🏢 Nom du fournisseur :"));
        mainLayout->addWidget(nomEdit);
        mainLayout->addWidget(new QLabel("📦 Type de matière :"));
        mainLayout->addWidget(typeCombo);
        mainLayout->addWidget(new QLabel("📞 Téléphone :"));
        mainLayout->addWidget(telephoneEdit);
        mainLayout->addWidget(new QLabel("📍 Adresse :"));
        mainLayout->addWidget(adresseEdit);
        mainLayout->addWidget(new QLabel("📅 Délai de livraison :"));
        mainLayout->addWidget(delaiDate);
        mainLayout->addWidget(new QLabel("⭐ Qualité matière :"));
        mainLayout->addWidget(qualiteCombo);
        mainLayout->addWidget(new QLabel("⚡ Statut :"));
        mainLayout->addWidget(statutCombo);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->setSpacing(20);
        QPushButton *btnSave = new QPushButton("💾 Mettre à jour");
        btnSave->setObjectName("btnSave");
        QPushButton *btnCancel = new QPushButton("❌ Annuler");
        btnCancel->setObjectName("btnCancel");
        btnLayout->addWidget(btnSave);
        btnLayout->addWidget(btnCancel);
        mainLayout->addLayout(btnLayout);

        connect(btnSave, &QPushButton::clicked, [modifierDialog, currentRow, nomEdit, typeCombo,
                                                  telephoneEdit, adresseEdit, delaiDate,
                                                  qualiteCombo, statutCombo, this]() {
            if (nomEdit->text().isEmpty() || telephoneEdit->text().isEmpty() ||
                adresseEdit->text().isEmpty()) {
                QMessageBox::warning(modifierDialog, "Champs manquants",
                                   "Veuillez remplir tous les champs.");
                return;
            }

            QString telephone = telephoneEdit->text();
            bool phoneValid = true;
            for (QChar c : telephone) {
                if (!c.isDigit() && c != ' ' && c != '+' && c != '-') {
                    phoneValid = false;
                    break;
                }
            }

            if (!phoneValid) {
                QMessageBox::warning(modifierDialog, "Téléphone invalide",
                                   "Veuillez entrer un numéro de téléphone valide.");
                return;
            }

            QString delaiFormatted = delaiDate->date().toString("dd/MM/yyyy");

            updateFournisseurInTable(currentRow,
                                    nomEdit->text(),
                                    typeCombo->currentText(),
                                    telephoneEdit->text(),
                                    adresseEdit->text(),
                                    delaiFormatted,
                                    qualiteCombo->currentText(),
                                    statutCombo->currentText());

            QMessageBox::information(modifierDialog, "Succès",
                                   "✅ Le fournisseur a été mis à jour avec succès!");
            modifierDialog->accept();
        });

        connect(btnCancel, &QPushButton::clicked, modifierDialog, &QDialog::reject);
        modifierDialog->exec();
        modifierDialog->deleteLater();
    }

    void fournisseurs::on_pushButton_3_clicked()
    {
        int currentRow = ui->tableWidget->currentRow();

        if (currentRow < 0) {
            QMessageBox::warning(this, "Sélection requise",
                               "Veuillez sélectionner un fournisseur à supprimer.");
            return;
        }

        QString id = ui->tableWidget->item(currentRow, 0)->text();
        QString nom = ui->tableWidget->item(currentRow, 1)->text();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation de suppression",
                                     QString("Voulez-vous vraiment supprimer le fournisseur '%1' - %2 ?")
                                     .arg(id).arg(nom),
                                     QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            ui->tableWidget->removeRow(currentRow);
            QMessageBox::information(this, "Succès", "✅ Fournisseur supprimé avec succès!");
            qDebug() << "Fournisseur supprimé:" << id << "- Nom:" << nom;
        }
    }

    void fournisseurs::on_pushButton_4_clicked()
    {
        ui->searchIdEdit->clear();
        ui->searchNomEdit->clear();
        loadFournisseurs();
        QMessageBox::information(this, "Actualisation", "✅ Liste des fournisseurs actualisée !");
    }

    void fournisseurs::on_pushButton_7_clicked()
    {
        QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel",
                                                        "fournisseurs_" + QDate::currentDate().toString("yyyyMMdd") + ".csv",
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

        QStringList headers = {"ID", "Nom", "Type matière", "Téléphone",
                               "Adresse", "Délai livraison", "Qualité", "Statut"};
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
    // Replace your current on_pushButton_9_clicked() with this:

    void fournisseurs::on_pushButton_9_clicked()
    {
        // 1. Count data
        int actif = 0, inactif = 0;
        int qualiteA = 0, qualiteB = 0, qualiteC = 0;

        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            if (!ui->tableWidget->isRowHidden(row)) {
                QString statut = ui->tableWidget->item(row, 7)->text(); // column 7 = Statut
                QString qualite = ui->tableWidget->item(row, 6)->text(); // column 6 = Qualité

                if (statut == "Actif") actif++;
                else if (statut == "Inactif") inactif++;

                if (qualite == "Qualité A") qualiteA++;
                else if (qualite == "Qualité B") qualiteB++;
                else if (qualite == "Qualité C") qualiteC++;
            }
        }

        int totalStatut = actif + inactif;
        int totalQualite = qualiteA + qualiteB + qualiteC;

        if (totalStatut == 0 && totalQualite == 0) {
            QMessageBox::information(this, "Statistiques", "Aucun fournisseur à afficher.");
            return;
        }

        // 2. Create dialog with leather theme
        QDialog *statsDialog = new QDialog(this);
        statsDialog->setWindowTitle("Statistiques des fournisseurs");
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

        // ----- Header (tooled leather) -----
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

        QLabel *titleLabel = new QLabel("📊 STATISTIQUES DES FOURNISSEURS");
        titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fffaf5;");
        headerLayout->addWidget(titleLabel);
        headerLayout->addStretch();

        // Total (we'll update per tab later, but a generic total can be shown)
        QLabel *totalLabel = new QLabel(QString("Total fournisseurs: %1").arg(totalStatut));
        totalLabel->setStyleSheet(
            "font-size: 20px; font-weight: bold; color: #fffaf5;"
            "background-color: #8b5a2b; padding: 8px 20px; border-radius: 30px;"
            "border: 1px solid #c49a6c;"
        );
        headerLayout->addWidget(totalLabel);

        mainLayout->addWidget(headerFrame);

        // ----- Tab widget -----
        QTabWidget *tabWidget = new QTabWidget();
        tabWidget->setStyleSheet(
            "QTabWidget::pane {"
            "   border: 2px solid #b08a6b;"
            "   border-radius: 15px;"
            "   background-color: #fffaf5;"
            "   padding: 10px;"
            "}"
            "QTabBar::tab {"
            "   background-color: #e9dccf;"
            "   border: 1px solid #b08a6b;"
            "   border-radius: 10px;"
            "   padding: 8px 16px;"
            "   margin-right: 5px;"
            "   font-weight: bold;"
            "   color: #3a2a20;"
            "}"
            "QTabBar::tab:selected {"
            "   background-color: #6b3e26;"
            "   color: #fffaf5;"
            "}"
        );

        // ----- Tab 1: Par statut -----
        if (totalStatut > 0) {
            QWidget *statutTab = new QWidget();
            QVBoxLayout *statutLayout = new QVBoxLayout(statutTab);

            QFrame *chartFrame = new QFrame();
            chartFrame->setStyleSheet(
                "QFrame {"
                "   background-color: #fffaf5;"
                "   border: none;"
                "   padding: 10px;"
                "}"
            );
            QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
            chartLayout->setSpacing(20);
            chartLayout->setAlignment(Qt::AlignBottom);

            int maxCount = qMax(actif, inactif);
            maxCount = qMax(maxCount, 1);
            const int maxBarHeight = 200;

            struct StatusData { QString name; int count; QColor color; QString icon; };
            QList<StatusData> statuses = {
                {"Actif", actif, QColor(100, 200, 100), "✅"},
                {"Inactif", inactif, QColor(200, 100, 100), "❌"}
            };

            for (const StatusData &sd : statuses) {
                if (sd.count == 0) continue;
                double percent = 100.0 * sd.count / totalStatut;
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
                barLayout->addWidget(nameLabel);

                barLayout->addStretch();
                chartLayout->addWidget(container);
            }

            statutLayout->addWidget(chartFrame);
            tabWidget->addTab(statutTab, "Par statut");
        }

        // ----- Tab 2: Par qualité -----
        if (totalQualite > 0) {
            QWidget *qualiteTab = new QWidget();
            QVBoxLayout *qualiteLayout = new QVBoxLayout(qualiteTab);

            QFrame *chartFrame = new QFrame();
            chartFrame->setStyleSheet(
                "QFrame {"
                "   background-color: #fffaf5;"
                "   border: none;"
                "   padding: 10px;"
                "}"
            );
            QHBoxLayout *chartLayout = new QHBoxLayout(chartFrame);
            chartLayout->setSpacing(20);
            chartLayout->setAlignment(Qt::AlignBottom);

            int maxCount = qMax(qMax(qualiteA, qualiteB), qualiteC);
            maxCount = qMax(maxCount, 1);
            const int maxBarHeight = 200;

            struct QualData { QString name; int count; QColor color; QString icon; };
            QList<QualData> qualites = {
                {"Qualité A", qualiteA, QColor(100, 150, 255), "⭐"},
                {"Qualité B", qualiteB, QColor(255, 255, 100), "⭐"},
                {"Qualité C", qualiteC, QColor(255, 100, 150), "⭐"}
            };

            for (const QualData &qd : qualites) {
                if (qd.count == 0) continue;
                double percent = 100.0 * qd.count / totalQualite;
                int barHeight = (qd.count * maxBarHeight) / maxCount;
                if (barHeight < 10) barHeight = 10;

                QWidget *container = new QWidget();
                QVBoxLayout *barLayout = new QVBoxLayout(container);
                barLayout->setSpacing(8);
                barLayout->setAlignment(Qt::AlignHCenter);

                QLabel *countLabel = new QLabel(QString::number(qd.count));
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
                )").arg(qd.color.lighter(110).name()).arg(qd.color.name()));
                barLayout->addWidget(bar, 0, Qt::AlignCenter);

                QLabel *nameLabel = new QLabel(qd.icon + " " + qd.name);
                nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3a2a20;");
                nameLabel->setAlignment(Qt::AlignCenter);
                barLayout->addWidget(nameLabel);

                barLayout->addStretch();
                chartLayout->addWidget(container);
            }

            qualiteLayout->addWidget(chartFrame);
            tabWidget->addTab(qualiteTab, "Par qualité");
        }

        mainLayout->addWidget(tabWidget);

        // ----- Close button (leather patch) -----
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
