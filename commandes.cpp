#include "commandes.h" // Force recompile
#include "ui_commandes.h"
#include "login.h"
#include "pageemployee.h"
#include "fournisseurs.h"
#include "produitswindow.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QDebug>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>

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


static QFrame* createSeparator() {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #d4c4b0; max-height: 1px; margin: 8px 0;");
    return line;
}
static void addShadow(QWidget* w, int blur = 20, int offsetY = 4) {
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(w);
    shadow->setBlurRadius(blur); shadow->setOffset(0, offsetY); shadow->setColor(QColor(0,0,0,40));
    w->setGraphicsEffect(shadow);
}
static void setFieldError(QLineEdit* field, QLabel* errorLabel, bool hasError, const QString& msg = "") {
    field->setProperty("error", hasError);
    field->style()->unpolish(field); field->style()->polish(field);
    errorLabel->setText(hasError ? "⚠ " + msg : "");
    errorLabel->setVisible(hasError);
}

commandes::commandes(int idEmploye, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::commandes)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    if (ui->groupBox_2) ui->groupBox_2->hide();

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

    addNavBtn("Employés", SLOT(on_pushButton_11_clicked()));
    addNavBtn("Produits", SLOT(on_pushButton_21_clicked()));
    addNavBtn("Commandes", nullptr, true);
    addNavBtn("Fournisseurs", SLOT(on_pushButton_12_clicked()));
    addNavBtn("Matières", SLOT(on_pushButton_22_clicked()));
    addNavBtn("Machines", SLOT(on_pushButton_23_clicked()));

    navLayout->addStretch();
    addNavBtn("Déconnexion", SLOT(on_pushButton_5_clicked()));
    
    sidebar->raise();
    sidebar->show();

    setupTable();
    loadCommandes();
    setupSearch();

}

commandes::~commandes()
{
    delete ui;
}

void commandes::setupTable()
{
    ui->tableWidget->setColumnCount(9);
    QStringList headers = {"ID", "Réf", "Client", "Adresse", "Date C.", "Livraison P.", "Montant", "Paiement", "État"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setVisible(true); // Force visibility
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Premium Styling for Table
    ui->tableWidget->setStyleSheet(
        "QTableWidget { background: white; border: 2px solid #c9b8a5; border-radius: 16px; gridline-color: #f5eee6; "
        "selection-background-color: #f5eee6; selection-color: #3a1f14; }"
    );

    // Premium Styling for Horizontal Header
    ui->tableWidget->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5b3020, stop:0.5 #4a2517, stop:1 #3a1a10);"
        "    color: #f5efe8; padding: 10px; border: none; font-weight: bold; font-size: 11px; letter-spacing: 1px;"
        "}"
    );
}

void commandes::loadCommandes()
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    if (!query.exec("SELECT ID_COMMANDE, REF, NOM_CLIENT, ADRESSE_LIVRAISON, TO_CHAR(DATE_COMMANDE, 'DD/MM/YYYY'), "
                    "TO_CHAR(DATE_LIVRAISON_PREVUE, 'DD/MM/YYYY'), MONTANT_TOTAL, MODE_PAIEMENT, ETAT_COMMANDE "
                    "FROM SMARTLEATHER.COMMANDE")) {
        qDebug() << "Erreur loadCommandes:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QString text = query.value(col).toString();
            if (col == 6) text += " DT";
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
    }
    updateTableColors();
}

void commandes::updateTableColors()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        QTableWidgetItem *etatItem = ui->tableWidget->item(row, 8);
        if (!etatItem) continue;
        QString etat = etatItem->text();
        if (etat == "Livrée") { etatItem->setBackground(QColor(200, 255, 200)); etatItem->setForeground(QColor(0, 100, 0)); }
        else if (etat == "Annulée") { etatItem->setBackground(QColor(255, 200, 200)); etatItem->setForeground(QColor(139, 0, 0)); }
        else if (etat == "En attente") { etatItem->setBackground(QColor(255, 255, 200)); etatItem->setForeground(QColor(128, 128, 0)); }
        else if (etat == "En cours") { etatItem->setBackground(QColor(200, 230, 255)); etatItem->setForeground(QColor(0, 0, 139)); }
    }
}

// ═══════════════════════════════════════════════
//   AJOUTER COMMANDE
// ═══════════════════════════════════════════════
void commandes::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle Commande");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✦ NOUVELLE COMMANDE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("ex: CMD-2026-001");
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QLineEdit *clientEdit = new QLineEdit();
    clientEdit->setPlaceholderText("ex: Maison du Cuir");
    QLabel *clientError = new QLabel(); clientError->setObjectName("errorLabel"); clientError->setVisible(false);

    QLineEdit *adresseEdit = new QLineEdit();
    adresseEdit->setPlaceholderText("ex: 12 Rue de Paris");
    QLabel *addrError = new QLabel(); addrError->setObjectName("errorLabel"); addrError->setVisible(false);

    QDoubleSpinBox *montantSpin = new QDoubleSpinBox();
    montantSpin->setRange(0, 9999999);
    montantSpin->setValue(0.0);
    montantSpin->setDecimals(2);
    montantSpin->setSuffix(" DT");

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"En attente", "En cours", "Livrée", "Annulée"});

    QComboBox *modeCombo = new QComboBox();
    modeCombo->addItems({"Espèces", "Virement", "Chèque", "Carte"});

    QDateEdit *dateCmdEdit = new QDateEdit(QDate::currentDate());
    dateCmdEdit->setDisplayFormat("dd/MM/yyyy");
    dateCmdEdit->setCalendarPopup(true);

    QDateEdit *dateLivEdit = new QDateEdit(QDate::currentDate().addDays(7));
    dateLivEdit->setDisplayFormat("dd/MM/yyyy");
    dateLivEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM CLIENT")); mainLayout->addWidget(clientEdit); mainLayout->addWidget(clientError);
    mainLayout->addWidget(new QLabel("ADRESSE LIVRAISON")); mainLayout->addWidget(adresseEdit); mainLayout->addWidget(addrError);
    mainLayout->addWidget(new QLabel("MONTANT TOTAL")); mainLayout->addWidget(montantSpin);
    
    QHBoxLayout *comboLayout = new QHBoxLayout();
    QVBoxLayout *eCol = new QVBoxLayout(); eCol->addWidget(new QLabel("ÉTAT")); eCol->addWidget(etatCombo);
    QVBoxLayout *mCol = new QVBoxLayout(); mCol->addWidget(new QLabel("PAIEMENT")); mCol->addWidget(modeCombo);
    comboLayout->addLayout(eCol); comboLayout->addLayout(mCol);
    mainLayout->addLayout(comboLayout);

    QHBoxLayout *dateLayout = new QHBoxLayout();
    QVBoxLayout *cCol = new QVBoxLayout(); cCol->addWidget(new QLabel("DATE COMMANDE")); cCol->addWidget(dateCmdEdit);
    QVBoxLayout *lCol = new QVBoxLayout(); lCol->addWidget(new QLabel("DATE LIVR. PRÉVUE")); lCol->addWidget(dateLivEdit);
    dateLayout->addLayout(cCol); dateLayout->addLayout(lCol);
    mainLayout->addLayout(dateLayout);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(createSeparator());

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  ENREGISTRER  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave); btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    auto validateAll = [&]() {
        bool allOk = true;
        if (refEdit->text().trimmed().isEmpty()) { setFieldError(refEdit, refError, true, "Obligatoire"); allOk = false; } else setFieldError(refEdit, refError, false);
        if (clientEdit->text().trimmed().isEmpty()) { setFieldError(clientEdit, clientError, true, "Obligatoire"); allOk = false; } else setFieldError(clientEdit, clientError, false);
        if (adresseEdit->text().trimmed().isEmpty()) { setFieldError(adresseEdit, addrError, true, "Obligatoire"); allOk = false; } else setFieldError(adresseEdit, addrError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    btnSave->setEnabled(false);
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(clientEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(adresseEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO SMARTLEATHER.COMMANDE (ID_COMMANDE, REF, NOM_CLIENT, ADRESSE_LIVRAISON, MONTANT_TOTAL, ETAT_COMMANDE, MODE_PAIEMENT, DATE_COMMANDE, DATE_LIVRAISON_PREVUE, ID_EMPLOYE) "
                      "VALUES (SMARTLEATHER.SEQ_COMMANDE.NEXTVAL, :ref, :client, :addr, :montant, :etat, :mode, :datec, :datel, :idemp)");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":client", clientEdit->text().trimmed());
        query.bindValue(":addr", adresseEdit->text().trimmed());
        query.bindValue(":montant", montantSpin->value());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":mode", modeCombo->currentText());
        query.bindValue(":datec", dateCmdEdit->date());
        query.bindValue(":datel", dateLivEdit->date());
        query.bindValue(":idemp", m_idEmploye);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Commande ajoutée !");
            loadCommandes();
        } else {
            QString err = query.lastError().text();
            if (err.contains("ORA-00001")) {
                QMessageBox::critical(this, "Référence Existante", "Cette référence existe déjà. Veuillez en choisir une autre.");
            } else {
                QMessageBox::critical(this, "Erreur", "L'ajout a échoué.\nErreur technique: " + err.split('\n').first());
            }
        }
    }
}

// ═══════════════════════════════════════════════
//   MODIFIER COMMANDE
// ═══════════════════════════════════════════════
void commandes::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString client = ui->tableWidget->item(currentRow, 2)->text();
    QString addr = ui->tableWidget->item(currentRow, 3)->text();
    QString dateCStr = ui->tableWidget->item(currentRow, 4)->text();
    QString dateLStr = ui->tableWidget->item(currentRow, 5)->text();
    QString montantStr = ui->tableWidget->item(currentRow, 6)->text().replace(" DT", "").trimmed();
    QString paiement = ui->tableWidget->item(currentRow, 7)->text();
    QString etat = ui->tableWidget->item(currentRow, 8)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier Commande");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✎ MODIFIER COMMANDE");
    header->setObjectName("headerLabel"); header->setAlignment(Qt::AlignCenter); mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);
    QLineEdit *clientEdit = new QLineEdit(client);
    QLabel *clientError = new QLabel(); clientError->setObjectName("errorLabel"); clientError->setVisible(false);
    QLineEdit *adresseEdit = new QLineEdit(addr);
    QLabel *addrError = new QLabel(); addrError->setObjectName("errorLabel"); addrError->setVisible(false);

    QDoubleSpinBox *montantSpin = new QDoubleSpinBox(); montantSpin->setRange(0, 9999999);
    montantSpin->setValue(montantStr.toDouble()); montantSpin->setDecimals(2); montantSpin->setSuffix(" DT");

    QComboBox *etatCombo = new QComboBox(); etatCombo->addItems({"En attente", "En cours", "Livrée", "Annulée"});
    etatCombo->setCurrentText(etat);
    QComboBox *modeCombo = new QComboBox(); modeCombo->addItems({"Espèces", "Virement", "Chèque", "Carte"});
    modeCombo->setCurrentText(paiement);

    QDateEdit *dateCmdEdit = new QDateEdit(QDate::fromString(dateCStr, "dd/MM/yyyy")); dateCmdEdit->setDisplayFormat("dd/MM/yyyy"); dateCmdEdit->setCalendarPopup(true);
    QDateEdit *dateLivEdit = new QDateEdit(QDate::fromString(dateLStr, "dd/MM/yyyy")); dateLivEdit->setDisplayFormat("dd/MM/yyyy"); dateLivEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM CLIENT")); mainLayout->addWidget(clientEdit); mainLayout->addWidget(clientError);
    mainLayout->addWidget(new QLabel("ADRESSE LIVRAISON")); mainLayout->addWidget(adresseEdit); mainLayout->addWidget(addrError);
    mainLayout->addWidget(new QLabel("MONTANT TOTAL")); mainLayout->addWidget(montantSpin);
    
    QHBoxLayout *coLayout = new QHBoxLayout();
    QVBoxLayout *coC1 = new QVBoxLayout(); coC1->addWidget(new QLabel("ÉTAT")); coC1->addWidget(etatCombo);
    QVBoxLayout *coC2 = new QVBoxLayout(); coC2->addWidget(new QLabel("PAIEMENT")); coC2->addWidget(modeCombo);
    coLayout->addLayout(coC1); coLayout->addLayout(coC2); mainLayout->addLayout(coLayout);

    QHBoxLayout *dtLayout = new QHBoxLayout();
    QVBoxLayout *dtC1 = new QVBoxLayout(); dtC1->addWidget(new QLabel("DATE COMMANDE")); dtC1->addWidget(dateCmdEdit);
    QVBoxLayout *dtC2 = new QVBoxLayout(); dtC2->addWidget(new QLabel("LIVRAISON")); dtC2->addWidget(dateLivEdit);
    dtLayout->addLayout(dtC1); dtLayout->addLayout(dtC2); mainLayout->addLayout(dtLayout);

    mainLayout->addSpacing(10); mainLayout->addWidget(createSeparator());
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  METTRE À JOUR  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave); btnLayout->addWidget(btnCancel); mainLayout->addLayout(btnLayout);

    auto validateAll = [&]() {
        bool allOk = true;
        if (refEdit->text().trimmed().isEmpty()) { setFieldError(refEdit, refError, true, "Obligatoire"); allOk = false; } else setFieldError(refEdit, refError, false);
        if (clientEdit->text().trimmed().isEmpty()) { setFieldError(clientEdit, clientError, true, "Obligatoire"); allOk = false; } else setFieldError(clientEdit, clientError, false);
        if (adresseEdit->text().trimmed().isEmpty()) { setFieldError(adresseEdit, addrError, true, "Obligatoire"); allOk = false; } else setFieldError(adresseEdit, addrError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(clientEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(adresseEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject); connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE SMARTLEATHER.COMMANDE SET REF=:ref, NOM_CLIENT=:client, ADRESSE_LIVRAISON=:addr, "
                      "MONTANT_TOTAL=:montant, ETAT_COMMANDE=:etat, MODE_PAIEMENT=:mode, DATE_COMMANDE=:datec, DATE_LIVRAISON_PREVUE=:datel WHERE ID_COMMANDE=:id");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":client", clientEdit->text().trimmed());
        query.bindValue(":addr", adresseEdit->text().trimmed());
        query.bindValue(":montant", montantSpin->value());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":mode", modeCombo->currentText());
        query.bindValue(":datec", dateCmdEdit->date());
        query.bindValue(":datel", dateLivEdit->date());
        query.bindValue(":id", id);
            if (query.exec()) {
                QMessageBox::information(this, "Succès", "Commande mise à jour !");
                loadCommandes();
            } else {
                QString err = query.lastError().text();
                if (err.contains("ORA-00001")) {
                    QMessageBox::critical(this, "Référence Existante", "Cette référence existe déjà. Veuillez en choisir une autre.");
                } else {
                    QMessageBox::critical(this, "Erreur", "La modification a échoué.\nErreur technique: " + err.split('\n').first());
                }
            }
    }
}

void commandes::on_pushButton_3_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une commande."); return; }
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    if (QMessageBox::question(this, "Confirmation", "Supprimer la commande " + ref + " ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTLEATHER.COMMANDE WHERE ID_COMMANDE = :id");
        query.bindValue(":id", id);
        if (query.exec()) { QMessageBox::information(this, "Succès", "Commande supprimée."); loadCommandes(); }
        else { QMessageBox::critical(this, "Erreur", "Échec: " + query.lastError().text()); }
    }
}

void commandes::on_pushButton_4_clicked() { loadCommandes(); }
void commandes::filterTable(const QString &idFilter, const QString &dateFilter) {
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;
        if (!idFilter.isEmpty()) {
            if (!ui->tableWidget->item(row, 0)->text().contains(idFilter, Qt::CaseInsensitive) &&
                !ui->tableWidget->item(row, 1)->text().contains(idFilter, Qt::CaseInsensitive) &&
                !ui->tableWidget->item(row, 2)->text().contains(idFilter, Qt::CaseInsensitive)) {
                match = false;
            }
        }
        if (!dateFilter.isEmpty() && match) {
            if (!ui->tableWidget->item(row, 4)->text().contains(dateFilter)) { match = false; }
        }
        ui->tableWidget->setRowHidden(row, !match);
    }
}
void commandes::on_pushButton_10_clicked() { filterTable(ui->lineEdit_5->text(), ui->lineEdit_6->text()); }
void commandes::on_lineEdit_5_textChanged(const QString &text) { filterTable(text, ui->lineEdit_6->text()); }
void commandes::on_lineEdit_6_textChanged(const QString &text) { filterTable(ui->lineEdit_5->text(), text); }

void commandes::on_pushButton_7_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Excel", "commandes.csv", "CSV (*.csv)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    for (int col = 0; col < ui->tableWidget->columnCount(); col++) {
        out << "\"" << ui->tableWidget->horizontalHeaderItem(col)->text() << "\"";
        if (col < ui->tableWidget->columnCount() - 1) out << ",";
    }
    out << "\n";
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->tableWidget->columnCount(); col++) {
            out << "\"" << ui->tableWidget->item(row, col)->text() << "\"";
            if (col < ui->tableWidget->columnCount() - 1) out << ",";
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "Export", "Données exportées vers " + fileName);
}
void commandes::on_pushButton_9_clicked() { showStatistics(); }
void commandes::showStatistics() {
    int enAttente = 0, enPreparation = 0, livree = 0, annulee = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString etat = ui->tableWidget->item(row, 8)->text();
            if (etat == "En attente") enAttente++;
            else if (etat == "En cours") enPreparation++;
            else if (etat == "Livrée") livree++;
            else if (etat == "Annulée") annulee++;
        }
    }
    QPieSeries *series = new QPieSeries();
    if(enAttente>0) series->append("En attente", enAttente);
    if(enPreparation>0) series->append("En cours", enPreparation);
    if(livree>0) series->append("Livrée", livree);
    if(annulee>0) series->append("Annulée", annulee);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par État");
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog d(this); d.resize(600,400); d.setWindowTitle("Statistiques");
    QVBoxLayout *l = new QVBoxLayout(&d); l->addWidget(chartView);
    d.exec();
}
bool commandes::validateData(const QString &id, const QString &client, const QString &addr, const QString &montant) {
    Q_UNUSED(id); Q_UNUSED(client); Q_UNUSED(addr); Q_UNUSED(montant);
    return true;
}
void commandes::exportToPDF() {}

void commandes::on_pushButton_5_clicked() { hide(); login *l = new login(); l->show(); }
void commandes::on_pushButton_11_clicked() { hide(); pageemployee *pl = new pageemployee(m_idEmploye, this); pl->show(); }
void commandes::on_pushButton_12_clicked() { hide(); fournisseurs *pf = new fournisseurs(m_idEmploye, this); pf->show(); }
void commandes::on_pushButton_19_clicked() { on_pushButton_11_clicked(); }
void commandes::on_pushButton_20_clicked() { on_pushButton_12_clicked(); }
void commandes::on_pushButton_21_clicked() { hide(); produitswindow *pd = new produitswindow(m_idEmploye, this); pd->show(); }
void commandes::on_pushButton_22_clicked() { hide(); Matieres *mm = new Matieres(m_idEmploye, this); mm->show(); }
void commandes::on_pushButton_23_clicked() { hide(); pagemachine *ss = new pagemachine(m_idEmploye, this); ss->show(); }
void commandes::on_pushButton_17_clicked() { on_pushButton_5_clicked(); }

void commandes::setupSearch()
{
    connect(ui->lineEdit_5, &QLineEdit::textChanged, this, [this](const QString &text){ on_lineEdit_5_textChanged(text); });
    connect(ui->lineEdit_6, &QLineEdit::textChanged, this, [this](const QString &text){ on_lineEdit_6_textChanged(text); });
}

