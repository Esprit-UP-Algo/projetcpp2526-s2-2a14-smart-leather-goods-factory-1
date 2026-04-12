#include "matieres.h" // Force recompile
#include "ui_matieres.h"
#include "login.h"
#include "pageemployee.h"
#include "commandes.h"
#include "produitswindow.h"
#include "fournisseurs.h"
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

Matieres::Matieres(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Matieres)
    , m_idEmploye(idEmploye)
{
    ui->setupUi(this);
    if (ui->groupBox) ui->groupBox->hide();

    // Do NOT hide groupBox_2 if it contains the table and other components
    if (ui->groupBox_2) {
        ui->groupBox_2->setStyleSheet("QGroupBox { border: none; background: transparent; }");
    }

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

    addNavBtn("Employés", SLOT(on_pushButton_6_clicked()));
    addNavBtn("Produits", SLOT(on_pushButton_21_clicked()));
    addNavBtn("Commandes", SLOT(on_pushButton_20_clicked()));
    addNavBtn("Fournisseurs", SLOT(on_pushButton_22_clicked()));
    addNavBtn("Matières", nullptr, true);
    addNavBtn("Machines", SLOT(on_pushButton_23_clicked()));

    navLayout->addStretch();
    addNavBtn("Déconnexion", SLOT(on_pushButton_5_clicked()));
    
    sidebar->raise();
    sidebar->show();

    // Table and search widgets positioning (Luxury Leather default: clear the 240px sidebar)
    if(ui->tableWidget) ui->tableWidget->setGeometry(260, 230, 1050, 490);
    if(ui->label_8) ui->label_8->setGeometry(260, 50, 600, 60);
    // Align search labels and edits
    if(ui->label_9) ui->label_9->setGeometry(260, 160, 100, 31);
    if(ui->searchIdEdit) ui->searchIdEdit->setGeometry(360, 160, 131, 31);
    if(ui->label_10) ui->label_10->setGeometry(520, 160, 100, 31);
    if(ui->searchTypeEdit) ui->searchTypeEdit->setGeometry(620, 160, 131, 31);
    if(ui->pushButton_10) ui->pushButton_10->setGeometry(770, 160, 120, 31);
    if(ui->pushButton_7) ui->pushButton_7->setGeometry(900, 160, 120, 31);
    if(ui->pushButton_9) ui->pushButton_9->setGeometry(1030, 160, 120, 31);
    if(ui->pushButton) ui->pushButton->setGeometry(260, 730, 150, 40);
    if(ui->pushButton_2) ui->pushButton_2->setGeometry(420, 730, 150, 40);
    if(ui->pushButton_3) ui->pushButton_3->setGeometry(580, 730, 150, 40);
    if(ui->pushButton_4) ui->pushButton_4->setGeometry(740, 730, 150, 40);

    setupMatieresTable();
    loadMatieres();
    setupSearch();

}

Matieres::~Matieres() { delete ui; }

void Matieres::setupMatieresTable()
{
    ui->tableWidget->setColumnCount(10);
    QStringList headers = {"ID", "Réf", "Type Cuir", "Qualité", "Couleur", "Épaisseur (mm)", "Prix U.", "Stock", "Seuil", "Réception"};
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

void Matieres::loadMatieres()
{
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    if (!query.exec("SELECT ID_MATIERE, REF, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, TO_CHAR(DATE_RECEPTION, 'DD/MM/YYYY') "
                    "FROM SMARTLEATHER.MATIERE_PREMIERE")) {
        qDebug() << "Erreur loadMatieres:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 10; col++) {
            QString txt = query.value(col).toString();
            if (col == 6) txt += " DT";
            QTableWidgetItem *item = new QTableWidgetItem(txt);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        updateRowColors(row);
    }
}

void Matieres::updateRowColors(int row)
{
    QTableWidgetItem *stockItem = ui->tableWidget->item(row, 7);
    QTableWidgetItem *seuilItem = ui->tableWidget->item(row, 8);
    if (!stockItem || !seuilItem) return;
    
    int stock = stockItem->text().toInt();
    int seuil = seuilItem->text().toInt();

    if (stock <= seuil) {
        stockItem->setBackground(QColor(255, 200, 200));
        stockItem->setForeground(QColor(139, 0, 0));
    } else {
        stockItem->setBackground(QColor(200, 255, 200));
        stockItem->setForeground(QColor(0, 100, 0));
    }
}

void Matieres::setupSearch()
{
    connect(ui->searchTypeEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &Matieres::filterTable);
}

void Matieres::filterTable()
{
    QString f1 = ui->searchTypeEdit->text().trimmed(); // Type
    QString f2 = ui->searchIdEdit->text().trimmed(); // ID

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;
        if (!f1.isEmpty() && !ui->tableWidget->item(row, 2)->text().contains(f1, Qt::CaseInsensitive)) { match = false; }
        if (!f2.isEmpty() && match && !ui->tableWidget->item(row, 0)->text().contains(f2, Qt::CaseInsensitive)) { match = false; }
        ui->tableWidget->setRowHidden(row, !match);
    }
}

void Matieres::addMatiereToTable(const QString &id, const QString &type,
                               const QString &couleur, const QString &qualite,
                               const QString &prix, const QString &stock,
                               const QString &fournisseur)
{
    Q_UNUSED(id); Q_UNUSED(type); Q_UNUSED(couleur); Q_UNUSED(qualite);
    Q_UNUSED(prix); Q_UNUSED(stock); Q_UNUSED(fournisseur);
}

void Matieres::updateMatiereInTable(int row, const QString &type,
                                  const QString &couleur, const QString &qualite,
                                  const QString &prix, const QString &stock,
                                  const QString &fournisseur)
{
    Q_UNUSED(row); Q_UNUSED(type); Q_UNUSED(couleur); Q_UNUSED(qualite);
    Q_UNUSED(prix); Q_UNUSED(stock); Q_UNUSED(fournisseur);
}

// ═══════════════════════════════════════════════
//   AJOUTER MATIÈRE
// ═══════════════════════════════════════════════
void Matieres::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle Matière");
    dialog.setFixedSize(460, 720);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✦ NOUVELLE MATIÈRE PREMIÈRE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("ex: CUIR-001");
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Cuir Vachette", "Cuir Veau", "Daim", "Cuir Exotique", "Synthétique"});

    QComboBox *qualiteCombo = new QComboBox();
    qualiteCombo->addItems({"Premium", "Standard", "Economique"});

    QLineEdit *couleurEdit = new QLineEdit();
    couleurEdit->setPlaceholderText("ex: Noir, Marron, Beige");

    QDoubleSpinBox *epaisseurSpin = new QDoubleSpinBox();
    epaisseurSpin->setRange(0.1, 10.0); epaisseurSpin->setSingleStep(0.1); epaisseurSpin->setDecimals(1); epaisseurSpin->setSuffix(" mm");

    QDoubleSpinBox *prixSpin = new QDoubleSpinBox();
    prixSpin->setRange(0, 99999); prixSpin->setDecimals(2); prixSpin->setSuffix(" DT");

    QSpinBox *stockSpin = new QSpinBox(); stockSpin->setRange(0, 999999);
    QSpinBox *seuilSpin = new QSpinBox(); seuilSpin->setRange(0, 999999);

    QDateEdit *receptionEdit = new QDateEdit(QDate::currentDate());
    receptionEdit->setDisplayFormat("dd/MM/yyyy");
    receptionEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    
    QHBoxLayout *c1 = new QHBoxLayout();
    QVBoxLayout *c1a = new QVBoxLayout(); c1a->addWidget(new QLabel("TYPE CUIR")); c1a->addWidget(typeCombo);
    QVBoxLayout *c1b = new QVBoxLayout(); c1b->addWidget(new QLabel("QUALITÉ")); c1b->addWidget(qualiteCombo);
    c1->addLayout(c1a); c1->addLayout(c1b); mainLayout->addLayout(c1);

    mainLayout->addWidget(new QLabel("COULEUR")); mainLayout->addWidget(couleurEdit);
    
    QHBoxLayout *c2 = new QHBoxLayout();
    QVBoxLayout *c2a = new QVBoxLayout(); c2a->addWidget(new QLabel("ÉPAISSEUR")); c2a->addWidget(epaisseurSpin);
    QVBoxLayout *c2b = new QVBoxLayout(); c2b->addWidget(new QLabel("PRIX UNIT.")); c2b->addWidget(prixSpin);
    c2->addLayout(c2a); c2->addLayout(c2b); mainLayout->addLayout(c2);

    QHBoxLayout *c3 = new QHBoxLayout();
    QVBoxLayout *c3a = new QVBoxLayout(); c3a->addWidget(new QLabel("STOCK ACTUEL")); c3a->addWidget(stockSpin);
    QVBoxLayout *c3b = new QVBoxLayout(); c3b->addWidget(new QLabel("SEUIL ALERTE")); c3b->addWidget(seuilSpin);
    c3->addLayout(c3a); c3->addLayout(c3b); mainLayout->addLayout(c3);

    mainLayout->addWidget(new QLabel("DATE RÉCEPTION")); mainLayout->addWidget(receptionEdit);

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
        btnSave->setEnabled(allOk); return allOk;
    };
    btnSave->setEnabled(false);
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO SMARTLEATHER.MATIERE_PREMIERE (ID_MATIERE, REF, TYPE_CUIR, QUALITE, COULEUR, EPAISSEUR, PRIX_UNITAIRE, QUANTITE_STOCK, SEUIL_MIN, DATE_RECEPTION) "
                      "VALUES (SMARTLEATHER.SEQ_MATIERE.NEXTVAL, :ref, :type, :qualite, :couleur, :epaisseur, :prix, :stock, :seuil, :dateR)");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":couleur", couleurEdit->text().trimmed());
        query.bindValue(":epaisseur", epaisseurSpin->value());
        query.bindValue(":prix", prixSpin->value());
        query.bindValue(":stock", stockSpin->value());
        query.bindValue(":seuil", seuilSpin->value());
        query.bindValue(":dateR", receptionEdit->date());

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Matière ajoutée !");
            loadMatieres();
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
//   MODIFIER MATIÈRE
// ═══════════════════════════════════════════════
void Matieres::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une matière."); return; }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString type = ui->tableWidget->item(currentRow, 2)->text();
    QString qualite = ui->tableWidget->item(currentRow, 3)->text();
    QString couleur = ui->tableWidget->item(currentRow, 4)->text();
    QString epaisseur = ui->tableWidget->item(currentRow, 5)->text();
    QString prix = ui->tableWidget->item(currentRow, 6)->text().replace(" DT", "").trimmed();
    QString stock = ui->tableWidget->item(currentRow, 7)->text();
    QString seuil = ui->tableWidget->item(currentRow, 8)->text();
    QString dateR = ui->tableWidget->item(currentRow, 9)->text();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier Matière");
    dialog.setFixedSize(460, 720);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✎ MODIFIER MATIÈRE");
    header->setObjectName("headerLabel"); header->setAlignment(Qt::AlignCenter); mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QComboBox *typeCombo = new QComboBox(); typeCombo->addItems({"Cuir Vachette", "Cuir Veau", "Daim", "Cuir Exotique", "Synthétique"});
    typeCombo->setCurrentText(type);
    QComboBox *qualiteCombo = new QComboBox(); qualiteCombo->addItems({"Premium", "Standard", "Economique"});
    qualiteCombo->setCurrentText(qualite);
    QLineEdit *couleurEdit = new QLineEdit(couleur);
    QDoubleSpinBox *epaisseurSpin = new QDoubleSpinBox(); epaisseurSpin->setRange(0.1, 10.0); epaisseurSpin->setDecimals(1); epaisseurSpin->setValue(epaisseur.toDouble());
    QDoubleSpinBox *prixSpin = new QDoubleSpinBox(); prixSpin->setRange(0, 99999); prixSpin->setDecimals(2); prixSpin->setValue(prix.toDouble());
    QSpinBox *stockSpin = new QSpinBox(); stockSpin->setRange(0, 999999); stockSpin->setValue(stock.toInt());
    QSpinBox *seuilSpin = new QSpinBox(); seuilSpin->setRange(0, 999999); seuilSpin->setValue(seuil.toInt());
    QDateEdit *receptionEdit = new QDateEdit(QDate::fromString(dateR, "dd/MM/yyyy")); receptionEdit->setDisplayFormat("dd/MM/yyyy"); receptionEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    
    QHBoxLayout *c1 = new QHBoxLayout();
    QVBoxLayout *c1a = new QVBoxLayout(); c1a->addWidget(new QLabel("TYPE CUIR")); c1a->addWidget(typeCombo);
    QVBoxLayout *c1b = new QVBoxLayout(); c1b->addWidget(new QLabel("QUALITÉ")); c1b->addWidget(qualiteCombo);
    c1->addLayout(c1a); c1->addLayout(c1b); mainLayout->addLayout(c1);

    mainLayout->addWidget(new QLabel("COULEUR")); mainLayout->addWidget(couleurEdit);
    
    QHBoxLayout *c2 = new QHBoxLayout();
    QVBoxLayout *c2a = new QVBoxLayout(); c2a->addWidget(new QLabel("ÉPAISSEUR (mm)")); c2a->addWidget(epaisseurSpin);
    QVBoxLayout *c2b = new QVBoxLayout(); c2b->addWidget(new QLabel("PRIX UNIT. (DT)")); c2b->addWidget(prixSpin);
    c2->addLayout(c2a); c2->addLayout(c2b); mainLayout->addLayout(c2);

    QHBoxLayout *c3 = new QHBoxLayout();
    QVBoxLayout *c3a = new QVBoxLayout(); c3a->addWidget(new QLabel("STOCK ACTUEL")); c3a->addWidget(stockSpin);
    QVBoxLayout *c3b = new QVBoxLayout(); c3b->addWidget(new QLabel("SEUIL ALERTE")); c3b->addWidget(seuilSpin);
    c3->addLayout(c3a); c3->addLayout(c3b); mainLayout->addLayout(c3);

    mainLayout->addWidget(new QLabel("DATE RÉCEPTION")); mainLayout->addWidget(receptionEdit);

    mainLayout->addSpacing(10); mainLayout->addWidget(createSeparator());
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  METTRE À JOUR  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave); btnLayout->addWidget(btnCancel); mainLayout->addLayout(btnLayout);

    auto validateAll = [&]() {
        bool allOk = true;
        if (refEdit->text().trimmed().isEmpty()) { setFieldError(refEdit, refError, true, "Obligatoire"); allOk = false; } else setFieldError(refEdit, refError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject); connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE SMARTLEATHER.MATIERE_PREMIERE SET REF=:ref, TYPE_CUIR=:type, QUALITE=:qualite, COULEUR=:couleur, "
                      "EPAISSEUR=:epaisseur, PRIX_UNITAIRE=:prix, QUANTITE_STOCK=:stock, SEUIL_MIN=:seuil, DATE_RECEPTION=:dateR WHERE ID_MATIERE=:id");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":qualite", qualiteCombo->currentText());
        query.bindValue(":couleur", couleurEdit->text().trimmed());
        query.bindValue(":epaisseur", epaisseurSpin->value());
        query.bindValue(":prix", prixSpin->value());
        query.bindValue(":stock", stockSpin->value());
        query.bindValue(":seuil", seuilSpin->value());
        query.bindValue(":dateR", receptionEdit->date());
        query.bindValue(":id", id);
        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Matière mise à jour !");
            loadMatieres();
        } else {
            QString err = query.lastError().text();
            if (err.contains("ORA-00001")) {
                QMessageBox::critical(this, "Référence Existante", "Cette référence existe déjà. Veuillez en choisir une autre.");
            } else {
                QMessageBox::critical(this, "Erreur", "La mise à jour a échoué: " + err.split('\n').first());
            }
        }
    }
}

void Matieres::on_pushButton_3_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une matière."); return; }
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    if (QMessageBox::question(this, "Confirmation", "Supprimer matière " + ref + " ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTLEATHER.MATIERE_PREMIERE WHERE ID_MATIERE = :id");
        query.bindValue(":id", id);
        if (query.exec()) { QMessageBox::information(this, "Succès", "Matière supprimée."); loadMatieres(); }
        else { QMessageBox::critical(this, "Erreur", "Échec: " + query.lastError().text()); }
    }
}

void Matieres::on_pushButton_4_clicked() { loadMatieres(); }

void Matieres::on_pushButton_7_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Excel", "matieres.csv", "CSV (*.csv)");
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

void Matieres::on_pushButton_9_clicked() {
    int faible = 0, normal = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            int stock = ui->tableWidget->item(row, 7)->text().toInt();
            int seuil = ui->tableWidget->item(row, 8)->text().toInt();
            if (stock <= seuil) faible++; else normal++;
        }
    }
    QPieSeries *series = new QPieSeries();
    if(normal>0) series->append("Stock Normal", normal);
    if(faible>0) series->append("Sous le Seuil", faible);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("État des Stocks de Matières");
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog d(this); d.resize(600,400); d.setWindowTitle("Statistiques Stocks");
    QVBoxLayout *l = new QVBoxLayout(&d); l->addWidget(chartView);
    d.exec();
}

void Matieres::on_pushButton_6_clicked() { hide(); pageemployee *pl = new pageemployee(m_idEmploye, this); pl->show(); }
void Matieres::on_pushButton_21_clicked() { hide(); produitswindow *pd = new produitswindow(m_idEmploye, this); pd->show(); }
void Matieres::on_pushButton_20_clicked() { hide(); commandes *pc = new commandes(m_idEmploye, this); pc->show(); }
void Matieres::on_pushButton_22_clicked() { hide(); fournisseurs *pf = new fournisseurs(m_idEmploye, this); pf->show(); }
void Matieres::on_pushButton_23_clicked() { hide(); pagemachine *ss = new pagemachine(m_idEmploye, this); ss->show(); }
void Matieres::on_pushButton_5_clicked() { hide(); login *l = new login(); l->show(); }
void Matieres::on_pushButton_11_clicked() { on_pushButton_6_clicked(); }

