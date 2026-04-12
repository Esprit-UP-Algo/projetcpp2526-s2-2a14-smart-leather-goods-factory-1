#include "fournisseurs.h" // Force recompile
#include "ui_fournisseurs.h"
#include "login.h"
#include "pageemployee.h"
#include "commandes.h"
#include "produitswindow.h"
#include "matieres.h"
#include "pagemachine.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
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

fournisseurs::fournisseurs(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::fournisseurs)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    if (ui->groupBox) ui->groupBox->hide();

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

    addNavBtn("Employés", SLOT(on_pushButton_15_clicked()));
    addNavBtn("Produits", SLOT(on_pushButton_21_clicked()));
    addNavBtn("Commandes", SLOT(on_pushButton_20_clicked()));
    addNavBtn("Fournisseurs", nullptr, true);
    addNavBtn("Matières", SLOT(on_pushButton_22_clicked()));
    addNavBtn("Machines", SLOT(on_pushButton_23_clicked()));

    navLayout->addStretch();
    addNavBtn("Déconnexion", SLOT(on_pushButton_16_clicked()));
    
    sidebar->raise();
    sidebar->show();

    setupFournisseursTable();
    loadFournisseurs();
    setupSearch();

}

fournisseurs::~fournisseurs() { delete ui; }

void fournisseurs::setupFournisseursTable()
{
    ui->tableWidget->setColumnCount(9);
    QStringList headers = {"ID", "Réf", "Nom", "Matière", "Téléphone", "Adresse", "Délai", "Qualité", "Statut"};
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

void fournisseurs::loadFournisseurs()
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    if (!query.exec("SELECT ID_FOURNISSEUR, REF, NOM, TYPE_MATIERE, TEL, ADRESSE, TO_CHAR(DELAI_LIVRAISON, 'DD/MM/YYYY'), QUALITE, STATUT "
                    "FROM SMARTLEATHER.FOURNISSEUR")) {
        qDebug() << "Erreur loadFournisseurs:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        updateRowColors(row);
    }
}

void fournisseurs::addFournisseurToTable(const QString &id, const QString &nom,
                                         const QString &typeMatiere, const QString &telephone,
                                         const QString &adresse, const QString &delaiLivraison,
                                         const QString &qualite, const QString &statut)
{
    Q_UNUSED(id); Q_UNUSED(nom); Q_UNUSED(typeMatiere); Q_UNUSED(telephone);
    Q_UNUSED(adresse); Q_UNUSED(delaiLivraison); Q_UNUSED(qualite); Q_UNUSED(statut);
}
void fournisseurs::updateFournisseurInTable(int row, const QString &nom,
                                            const QString &typeMatiere, const QString &telephone,
                                            const QString &adresse, const QString &delaiLivraison,
                                            const QString &qualite, const QString &statut)
{
    Q_UNUSED(row); Q_UNUSED(nom); Q_UNUSED(typeMatiere); Q_UNUSED(telephone);
    Q_UNUSED(adresse); Q_UNUSED(delaiLivraison); Q_UNUSED(qualite); Q_UNUSED(statut);
}

void fournisseurs::updateRowColors(int row)
{
    QTableWidgetItem *statutItem = ui->tableWidget->item(row, 8);
    if (!statutItem) return;
    QString statut = statutItem->text();
    if (statut == "Actif") { statutItem->setBackground(QColor(200, 255, 200)); statutItem->setForeground(QColor(0, 100, 0)); }
    else if (statut == "Inactif") { statutItem->setBackground(QColor(255, 200, 200)); statutItem->setForeground(QColor(139, 0, 0)); }
}

void fournisseurs::setupSearch()
{
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &fournisseurs::filterTable);
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &fournisseurs::filterTable);
}

void fournisseurs::filterTable()
{
    QString filterNom = ui->searchNomEdit->text().trimmed();
    QString filterId = ui->searchIdEdit->text().trimmed();

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;
        
        if (!filterNom.isEmpty() && !ui->tableWidget->item(row, 2)->text().contains(filterNom, Qt::CaseInsensitive)) { match = false; }
        if (!filterId.isEmpty() && match && !ui->tableWidget->item(row, 1)->text().contains(filterId, Qt::CaseInsensitive)) { match = false; }
        
        ui->tableWidget->setRowHidden(row, !match);
    }
}

// ═══════════════════════════════════════════════
//   AJOUTER FOURNISSEUR
// ═══════════════════════════════════════════════
void fournisseurs::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouveau Fournisseur");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✦ NOUVEAU FOURNISSEUR");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("ex: FOU-001");
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("ex: Cuirs & Co");
    QLabel *nomError = new QLabel(); nomError->setObjectName("errorLabel"); nomError->setVisible(false);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir Vachette", "Cuir Veau", "Daim", "Cuir Exotique", "Synthétique"});

    QLineEdit *telEdit = new QLineEdit();
    telEdit->setPlaceholderText("ex: 98123456");
    telEdit->setMaxLength(8);
    QLabel *telError = new QLabel(); telError->setObjectName("errorLabel"); telError->setVisible(false);

    QLineEdit *adresseEdit = new QLineEdit();
    adresseEdit->setPlaceholderText("ex: 15 Rue de l'Artisan");
    QLabel *addrError = new QLabel(); addrError->setObjectName("errorLabel"); addrError->setVisible(false);

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Premium", "Standard", "Economique"});
    
    QComboBox *statutCombo = new QComboBox();
    statutCombo->addItems({"Actif", "Inactif"});

    QDateEdit *delaiEdit = new QDateEdit(QDate::currentDate().addDays(14));
    delaiEdit->setDisplayFormat("dd/MM/yyyy");
    delaiEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM FOURNISSEUR")); mainLayout->addWidget(nomEdit); mainLayout->addWidget(nomError);
    mainLayout->addWidget(new QLabel("TYPE MATIÈRE PRINCIPALE")); mainLayout->addWidget(typeCombo);
    
    QHBoxLayout *c1Layout = new QHBoxLayout();
    QVBoxLayout *colTel = new QVBoxLayout(); colTel->addWidget(new QLabel("TÉLÉPHONE")); colTel->addWidget(telEdit); colTel->addWidget(telError);
    QVBoxLayout *colQual = new QVBoxLayout(); colQual->addWidget(new QLabel("QUALITÉ")); colQual->addWidget(qualiteCombo);
    c1Layout->addLayout(colTel); c1Layout->addLayout(colQual);
    mainLayout->addLayout(c1Layout);

    mainLayout->addWidget(new QLabel("ADRESSE")); mainLayout->addWidget(adresseEdit); mainLayout->addWidget(addrError);

    QHBoxLayout *c2Layout = new QHBoxLayout();
    QVBoxLayout *colDel = new QVBoxLayout(); colDel->addWidget(new QLabel("DÉLAI LIVRAISON")); colDel->addWidget(delaiEdit);
    QVBoxLayout *colStat = new QVBoxLayout(); colStat->addWidget(new QLabel("STATUT")); colStat->addWidget(statutCombo);
    c2Layout->addLayout(colDel); c2Layout->addLayout(colStat);
    mainLayout->addLayout(c2Layout);

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
        if (nomEdit->text().trimmed().isEmpty()) { setFieldError(nomEdit, nomError, true, "Obligatoire"); allOk = false; } else setFieldError(nomEdit, nomError, false);
        if (telEdit->text().trimmed().isEmpty() || telEdit->text().length() < 8) { setFieldError(telEdit, telError, true, "8 chiffres min"); allOk = false; } else setFieldError(telEdit, telError, false);
        if (adresseEdit->text().trimmed().isEmpty()) { setFieldError(adresseEdit, addrError, true, "Obligatoire"); allOk = false; } else setFieldError(adresseEdit, addrError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    btnSave->setEnabled(false);
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(nomEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(telEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(adresseEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO SMARTLEATHER.FOURNISSEUR (ID_FOURNISSEUR, REF, NOM, TYPE_MATIERE, TEL, ADRESSE, DELAI_LIVRAISON, QUALITE, STATUT) "
                      "VALUES (SMARTLEATHER.SEQ_FOURNISSEUR.NEXTVAL, :ref, :nom, :type, :tel, :addr, :delai, :qualite, :statut)");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":nom", nomEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":tel", telEdit->text().toInt());
        query.bindValue(":addr", adresseEdit->text().trimmed());
        query.bindValue(":delai", delaiEdit->date());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":statut", statutCombo->currentText());

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Fournisseur ajouté !");
            loadFournisseurs();
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
//   MODIFIER FOURNISSEUR
// ═══════════════════════════════════════════════
void fournisseurs::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un fournisseur à modifier.");
        return;
    }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString tel = ui->tableWidget->item(currentRow, 4)->text();
    QString addr = ui->tableWidget->item(currentRow, 5)->text();
    QString delai = ui->tableWidget->item(currentRow, 6)->text();
    QString qual = ui->tableWidget->item(currentRow, 7)->text();
    QString statut = ui->tableWidget->item(currentRow, 8)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier Fournisseur");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✎ MODIFIER FOURNISSEUR");
    header->setObjectName("headerLabel"); header->setAlignment(Qt::AlignCenter); mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QLineEdit *nomEdit = new QLineEdit(nom);
    QLabel *nomError = new QLabel(); nomError->setObjectName("errorLabel"); nomError->setVisible(false);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir Vachette", "Cuir Veau", "Daim", "Cuir Exotique", "Synthétique"});
    typeCombo->setCurrentText(type);

    QLineEdit *telEdit = new QLineEdit(tel); telEdit->setMaxLength(8);
    QLabel *telError = new QLabel(); telError->setObjectName("errorLabel"); telError->setVisible(false);

    QLineEdit *adresseEdit = new QLineEdit(addr);
    QLabel *addrError = new QLabel(); addrError->setObjectName("errorLabel"); addrError->setVisible(false);

    QComboBox *qualiteCombo = new QComboBox(); qualiteCombo->addItems({"Premium", "Standard", "Economique"});
    qualiteCombo->setCurrentText(qual);
    
    QComboBox *statutCombo = new QComboBox(); statutCombo->addItems({"Actif", "Inactif"});
    statutCombo->setCurrentText(statut);

    QDateEdit *delaiEdit = new QDateEdit(QDate::fromString(delai, "dd/MM/yyyy"));
    delaiEdit->setDisplayFormat("dd/MM/yyyy"); delaiEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM FOURNISSEUR")); mainLayout->addWidget(nomEdit); mainLayout->addWidget(nomError);
    mainLayout->addWidget(new QLabel("TYPE MATIÈRE PRINCIPALE")); mainLayout->addWidget(typeCombo);
    
    QHBoxLayout *c1Layout = new QHBoxLayout();
    QVBoxLayout *colTel = new QVBoxLayout(); colTel->addWidget(new QLabel("TÉLÉPHONE")); colTel->addWidget(telEdit); colTel->addWidget(telError);
    QVBoxLayout *colQual = new QVBoxLayout(); colQual->addWidget(new QLabel("QUALITÉ")); colQual->addWidget(qualiteCombo);
    c1Layout->addLayout(colTel); c1Layout->addLayout(colQual); mainLayout->addLayout(c1Layout);

    mainLayout->addWidget(new QLabel("ADRESSE")); mainLayout->addWidget(adresseEdit); mainLayout->addWidget(addrError);

    QHBoxLayout *c2Layout = new QHBoxLayout();
    QVBoxLayout *colDel = new QVBoxLayout(); colDel->addWidget(new QLabel("DÉLAI LIVRAISON")); colDel->addWidget(delaiEdit);
    QVBoxLayout *colStat = new QVBoxLayout(); colStat->addWidget(new QLabel("STATUT")); colStat->addWidget(statutCombo);
    c2Layout->addLayout(colDel); c2Layout->addLayout(colStat); mainLayout->addLayout(c2Layout);

    mainLayout->addSpacing(10); mainLayout->addWidget(createSeparator());
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  METTRE À JOUR  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave); btnLayout->addWidget(btnCancel); mainLayout->addLayout(btnLayout);

    auto validateAll = [&]() {
        bool allOk = true;
        if (refEdit->text().trimmed().isEmpty()) { setFieldError(refEdit, refError, true, "Obligatoire"); allOk = false; } else setFieldError(refEdit, refError, false);
        if (nomEdit->text().trimmed().isEmpty()) { setFieldError(nomEdit, nomError, true, "Obligatoire"); allOk = false; } else setFieldError(nomEdit, nomError, false);
        if (telEdit->text().trimmed().isEmpty() || telEdit->text().length() < 8) { setFieldError(telEdit, telError, true, "8 chiffres min"); allOk = false; } else setFieldError(telEdit, telError, false);
        if (adresseEdit->text().trimmed().isEmpty()) { setFieldError(adresseEdit, addrError, true, "Obligatoire"); allOk = false; } else setFieldError(adresseEdit, addrError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(nomEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(telEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(adresseEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject); connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE SMARTLEATHER.FOURNISSEUR SET REF=:ref, NOM=:nom, TYPE_MATIERE=:type, TEL=:tel, "
                      "ADRESSE=:addr, DELAI_LIVRAISON=:delai, QUALITE=:qualite, STATUT=:statut WHERE ID_FOURNISSEUR=:id");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":nom", nomEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":tel", telEdit->text().toInt());
        query.bindValue(":addr", adresseEdit->text().trimmed());
        query.bindValue(":delai", delaiEdit->date());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":statut", statutCombo->currentText());
        query.bindValue(":id", id);
            if (query.exec()) {
                QMessageBox::information(this, "Succès", "Fournisseur mis à jour !");
                loadFournisseurs();
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

void fournisseurs::on_pushButton_3_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un fournisseur."); return; }
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    if (QMessageBox::question(this, "Confirmation", "Supprimer fournisseur " + ref + " ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTLEATHER.FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
        query.bindValue(":id", id);
        if (query.exec()) { QMessageBox::information(this, "Succès", "Fournisseur supprimé."); loadFournisseurs(); }
        else { QMessageBox::critical(this, "Erreur", "Échec: " + query.lastError().text()); }
    }
}

void fournisseurs::on_pushButton_4_clicked() { loadFournisseurs(); }

void fournisseurs::on_pushButton_7_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Excel", "fournisseurs.csv", "CSV (*.csv)");
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
    QMessageBox::information(this, "Export", "Données exportées avec succès !");
}

void fournisseurs::on_pushButton_9_clicked() {
    int actif = 0, inactif = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString etat = ui->tableWidget->item(row, 8)->text();
            if (etat == "Actif") actif++; else if (etat == "Inactif") inactif++;
        }
    }
    QPieSeries *series = new QPieSeries();
    if(actif>0) series->append("Actif", actif);
    if(inactif>0) series->append("Inactif", inactif);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statut Fournisseurs");
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog d(this); d.resize(600,400); d.setWindowTitle("Statistiques");
    QVBoxLayout *l = new QVBoxLayout(&d); l->addWidget(chartView);
    d.exec();
}

void fournisseurs::on_pushButton_15_clicked() { hide(); pageemployee *pl = new pageemployee(m_idEmploye, this); pl->show(); }
void fournisseurs::on_pushButton_20_clicked() { hide(); commandes *pc = new commandes(m_idEmploye, this); pc->show(); }
void fournisseurs::on_pushButton_21_clicked() { hide(); produitswindow *pd = new produitswindow(m_idEmploye, this); pd->show(); }
void fournisseurs::on_pushButton_22_clicked() { hide(); Matieres *mm = new Matieres(m_idEmploye, this); mm->show(); }
void fournisseurs::on_pushButton_23_clicked() { hide(); pagemachine *ss = new pagemachine(m_idEmploye, this); ss->show(); }
void fournisseurs::on_pushButton_16_clicked() { hide(); login *l = new login(); l->show(); }
void fournisseurs::on_pushButton_17_clicked() { on_pushButton_15_clicked(); }
