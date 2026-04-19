#include "pagemachine.h" // Force recompile
#include "ui_pagemachine.h"
#include "login.h"
#include "pageemployee.h"
#include "commandes.h"
#include "produitswindow.h"
#include "fournisseurs.h"
#include "matieres.h"
#include "smtp.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDateTime>
#include <QDialog>
#include <QSslSocket>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QDebug>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>
#include <QPrinter>
#include <QPainter>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QTextCharFormat>

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
QLineEdit, QSpinBox, QComboBox {
    background-color: rgba(255, 255, 255, 0.85);
    border: 2px solid #d4c4b0;
    border-radius: 12px;
    padding: 10px 14px;
    color: #3a2a20;
    font-size: 13px;
    selection-background-color: #c9a87c;
}
QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
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

pagemachine::pagemachine(int idEmploye, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pagemachine)
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

    addNavBtn("Employés", SLOT(on_pushButton_6_clicked()));
    addNavBtn("Produits", SLOT(on_pushButton_21_clicked()));
    addNavBtn("Commandes", SLOT(on_pushButton_20_clicked()));
    addNavBtn("Fournisseurs", SLOT(on_pushButton_22_clicked()));
    addNavBtn("Matières", SLOT(on_pushButton_23_clicked()));
    addNavBtn("Machines", nullptr, true);

    navLayout->addStretch();
    addNavBtn("Déconnexion", SLOT(on_pushButton_11_clicked()));
    
    sidebar->raise();
    sidebar->show();
 
    setupMachinesTable();
    loadMachines();
    
    // Check SSL Support for Email feature
    bool supportsSsl = QSslSocket::supportsSsl();
    qDebug() << "======= DEBUG SYSTEM =======";
    qDebug() << "Supports SSL:" << supportsSsl;
    qDebug() << "SSL Version:" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << "============================";

    if (!supportsSsl) {
        QMessageBox::critical(this, "SSL Manquant", 
            "Votre système ne possède pas les bibliothèques OpenSSL nécessaires (libeay32.dll / ssleay32.dll).\n"
            "L'envoi d'emails sera IMPOSSIBLE tant qu'elles ne sont pas installées.");
    }

    // Customize search UI for State filter
    if (ui->label_9) ui->label_9->setText("REF MACHINE :");
    if (ui->label_10) ui->label_10->setText("ÉTAT :");
    
    if (ui->searchNomEdit) {
        ui->searchNomEdit->hide();
        QComboBox *etatSearch = new QComboBox(this);
        etatSearch->setObjectName("searchEtatCombo");
        etatSearch->setGeometry(ui->searchNomEdit->geometry());
        etatSearch->addItems({"Tous", "Active", "Inactive", "En maintenance", "En panne"});
        etatSearch->show();
        connect(etatSearch, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &pagemachine::filterTable);
    }
    
    // Connect original UI buttons for refined features
    connect(ui->pushButton_10, &QPushButton::clicked, this, &pagemachine::filterTable); // Recherche
    ui->pushButton_7->setText("Exporter PDF"); // Repurpose the main Exporter button

    setupSearch();

    // Start background maintenance reminder timer (checks every 60 seconds)
    m_reminderTimer = new QTimer(this);
    connect(m_reminderTimer, &QTimer::timeout, this, &pagemachine::checkUpcomingMaintenance);
    // Initial check right away, then every minute
    checkUpcomingMaintenance();
    m_reminderTimer->start(60000);
}

pagemachine::~pagemachine() { delete ui; }

void pagemachine::setupMachinesTable()
{
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);          // Niveau Charge
    
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);

    // Labels last to override any defaults
    QStringList headers = {"ID", "Réf", "Nom", "Type", "État", "Capacité", "Fréquence", "Charge (%)"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setVisible(true);

    // Styling the Table Grid and Background
    ui->tableWidget->setStyleSheet(
        "QTableWidget { "
        "    background-color: #ffffff; "
        "    gridline-color: #d4c4b0; "
        "    border: 1px solid #d4c4b0; "
        "    alternate-background-color: #faf6f1; "
        "    selection-background-color: #e8ddd0; "
        "    selection-color: #3a1f14; "
        "    border-radius: 8px;"
        "}"
    );

    // Premium Styling for Horizontal Header (Column Titles)
    ui->tableWidget->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background: #5b3020;"
        "    color: #f5efe8; "
        "    padding: 8px; "
        "    border: 1px solid #4a2517; "
        "    font-weight: bold; "
        "    font-size: 11px; "
        "}"
    );
}

void pagemachine::loadMachines()
{
    ui->tableWidget->setSortingEnabled(false); // Disable sorting during refill
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    if (!query.exec("SELECT ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE "
                    "FROM SMARTLEATHER.MACHINE")) {
        qDebug() << "Erreur loadMachines:" << query.lastError().text();
        ui->tableWidget->setSortingEnabled(true);
        return;
    }

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        
        qDebug() << "SQL Trace Row" << row << "(ID=" << query.value(0).toString() << "):";
        for (int col = 0; col < 8; col++) {
            QString txt = query.value(col).toString();
            qDebug() << "  Col" << col << "=" << txt;
            QTableWidgetItem *item = new QTableWidgetItem(txt);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        updateRowColors(row);
    }
    ui->tableWidget->setSortingEnabled(true); // Re-enable sorting
    ui->tableWidget->sortByColumn(1, Qt::AscendingOrder); // Default sort by REF
}

void pagemachine::updateRowColors(int row)
{
    QTableWidgetItem *etatItem = ui->tableWidget->item(row, 4);
    if (!etatItem) return;
    QString etat = etatItem->text();
    if (etat == "En maintenance" || etat == "En panne") { 
        etatItem->setBackground(QColor(255, 230, 230)); 
        etatItem->setForeground(QColor(139, 0, 0)); 
        etatItem->setFont(QFont("Segoe UI", -1, QFont::Bold));
    }
    else if (etat == "Active") { 
        etatItem->setBackground(QColor(230, 255, 230)); 
        etatItem->setForeground(QColor(0, 100, 0)); 
    }
    else if (etat == "Inactive") { 
        etatItem->setBackground(QColor(245, 245, 245)); 
        etatItem->setForeground(QColor(100, 100, 100)); 
    }
}

void pagemachine::setupSearch()
{
    connect(ui->searchNomEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
    connect(ui->searchIdEdit, &QLineEdit::textChanged, this, &pagemachine::filterTable);
}

void pagemachine::filterTable()
{
    QString fRef = ui->searchIdEdit->text().trimmed();
    QComboBox *etatSearch = findChild<QComboBox*>("searchEtatCombo");
    QString fEtat = (etatSearch && etatSearch->currentIndex() > 0) ? etatSearch->currentText() : "";

    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool match = true;
        if (!fRef.isEmpty() && !ui->tableWidget->item(row, 1)->text().contains(fRef, Qt::CaseInsensitive)) { match = false; }
        if (match && !fEtat.isEmpty() && ui->tableWidget->item(row, 4)->text() != fEtat) { match = false; }
        ui->tableWidget->setRowHidden(row, !match);
    }
}

void pagemachine::addMachineToTable(const QString &id, const QString &nom,
                                   const QString &type, const QString &etat,
                                   const QString &capacite, const QString &frequence,
                                   const QString &niveauCharge)
{
    Q_UNUSED(id); Q_UNUSED(nom); Q_UNUSED(type); Q_UNUSED(etat);
    Q_UNUSED(capacite); Q_UNUSED(frequence); Q_UNUSED(niveauCharge);
}

void pagemachine::updateMachineInTable(int row, const QString &nom,
                                      const QString &type, const QString &etat,
                                      const QString &capacite, const QString &frequence,
                                      const QString &niveauCharge)
{
    Q_UNUSED(row); Q_UNUSED(nom); Q_UNUSED(type); Q_UNUSED(etat);
    Q_UNUSED(capacite); Q_UNUSED(frequence); Q_UNUSED(niveauCharge);
}

// ═══════════════════════════════════════════════
//   AJOUTER MACHINE
// ═══════════════════════════════════════════════
void pagemachine::on_pushButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Nouvelle Machine");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✦ NOUVELLE MACHINE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit();
    refEdit->setPlaceholderText("ex: MAC-2026-001");
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);

    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("ex: Presse Hydra 500");
    QLabel *nomError = new QLabel(); nomError->setObjectName("errorLabel"); nomError->setVisible(false);

    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"Presse", "Découpeuse", "Couseuse", "Teinture", "Séchage"});

    QComboBox *etatCombo = new QComboBox();
    etatCombo->addItems({"Active", "Inactive", "En maintenance", "En panne"});

    QSpinBox *capaciteSpin = new QSpinBox(); capaciteSpin->setRange(1, 10000);
    QSpinBox *frequenceSpin = new QSpinBox(); frequenceSpin->setRange(1, 1000);
    QSpinBox *chargeSpin = new QSpinBox(); chargeSpin->setRange(0, 100); chargeSpin->setSuffix(" %");

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM MACHINE")); mainLayout->addWidget(nomEdit); mainLayout->addWidget(nomError);
    
    QHBoxLayout *c1 = new QHBoxLayout();
    QVBoxLayout *c1a = new QVBoxLayout(); c1a->addWidget(new QLabel("TYPE")); c1a->addWidget(typeCombo);
    QVBoxLayout *c1b = new QVBoxLayout(); c1b->addWidget(new QLabel("ÉTAT")); c1b->addWidget(etatCombo);
    c1->addLayout(c1a); c1->addLayout(c1b); mainLayout->addLayout(c1);

    QHBoxLayout *c2 = new QHBoxLayout();
    QVBoxLayout *c2a = new QVBoxLayout(); c2a->addWidget(new QLabel("CAPACITÉ")); c2a->addWidget(capaciteSpin);
    QVBoxLayout *c2b = new QVBoxLayout(); c2b->addWidget(new QLabel("FRÉQUENCE (Hz)")); c2b->addWidget(frequenceSpin);
    c2->addLayout(c2a); c2->addLayout(c2b); mainLayout->addLayout(c2);

    mainLayout->addWidget(new QLabel("NIVEAU DE CHARGE")); mainLayout->addWidget(chargeSpin);

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
        QString valNom = nomEdit->text().trimmed();
        if (valNom.isEmpty()) { 
            setFieldError(nomEdit, nomError, true, "Obligatoire"); allOk = false; 
        } else if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s']+$").match(valNom).hasMatch()) {
            setFieldError(nomEdit, nomError, true, "Lettres uniquement"); allOk = false;
        } else setFieldError(nomEdit, nomError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    btnSave->setEnabled(false);
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(nomEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO SMARTLEATHER.MACHINE (ID_MACHINE, REF, NOM, TYPE, ETAT, CAPACITE, FREQUENCE, NIVEAU_DE_CHARGE, ID_EMPLOYE) "
                      "VALUES (SMARTLEATHER.SEQ_MACHINE.NEXTVAL, :ref, :nom, :type, :etat, :cap, :freq, :charge, :idemp)");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":nom", nomEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":cap", capaciteSpin->value());
        query.bindValue(":freq", frequenceSpin->value());
        query.bindValue(":charge", chargeSpin->value());
        query.bindValue(":idemp", m_idEmploye);

        if (query.exec()) { 
            QMessageBox::information(this, "Succès", "Machine ajoutée !"); 
            
            // Advanced Feature: Email Alert if "En panne"
            if (etatCombo->currentText() == "En panne") {
                QString admEmail = "eyadkhil@aiesec.net"; 
                Smtp *smtp = new Smtp("dkhileya5@gmail.com", "bfnedrbmyguthysx");
                QString subject = "⚠️ ALERTE : Machine " + refEdit->text().trimmed() + " en panne";
                QString body = "La machine " + nomEdit->text().trimmed() + " (Réf: " + refEdit->text().trimmed() + ") vient d'être signalée EN PANNE.\n\nVeuillez intervenir rapidement.";
                
                connect(smtp, &Smtp::status, [this](const QString &msg){
                    QMessageBox::information(this, "Email", msg);
                });
                connect(smtp, &Smtp::error, [this](const QString &err){
                    QMessageBox::critical(this, "Erreur Email", err);
                });

                smtp->sendMail("eyadkhil5@gmail.com", admEmail, subject, body);
                
                QMessageBox::information(this, "Mail Envoyé", "Alerte : Un email contenant la référence et le nom de la machine a été envoyé à l'administration.");
            }
            
            loadMachines(); 
        }
        else {
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
//   MODIFIER MACHINE
// ═══════════════════════════════════════════════
void pagemachine::on_pushButton_2_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une machine."); return; }

    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString etat = ui->tableWidget->item(currentRow, 4)->text();
    int cap = ui->tableWidget->item(currentRow, 5)->text().toInt();
    int freq = ui->tableWidget->item(currentRow, 6)->text().toInt();
    int charge = ui->tableWidget->item(currentRow, 7)->text().replace(" %", "").toInt();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier Machine");
    dialog.setFixedSize(440, 680);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✎ MODIFIER MACHINE");
    header->setObjectName("headerLabel"); header->setAlignment(Qt::AlignCenter); mainLayout->addWidget(header);
    mainLayout->addWidget(createSeparator());

    QLineEdit *refEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);
    QLineEdit *nomEdit = new QLineEdit(nom);
    QLabel *nomError = new QLabel(); nomError->setObjectName("errorLabel"); nomError->setVisible(false);

    QComboBox *typeCombo = new QComboBox(); typeCombo->addItems({"Presse", "Découpeuse", "Couseuse", "Teinture", "Séchage"});
    typeCombo->setCurrentText(type);
    QComboBox *etatCombo = new QComboBox(); etatCombo->addItems({"Active", "Inactive", "En maintenance", "En panne"});
    etatCombo->setCurrentText(etat);

    QSpinBox *capaciteSpin = new QSpinBox(); capaciteSpin->setRange(1, 10000); capaciteSpin->setValue(cap);
    QSpinBox *frequenceSpin = new QSpinBox(); frequenceSpin->setRange(1, 1000); frequenceSpin->setValue(freq);
    QSpinBox *chargeSpin = new QSpinBox(); chargeSpin->setRange(0, 100); chargeSpin->setValue(charge); chargeSpin->setSuffix(" %");

    mainLayout->addWidget(new QLabel("RÉFÉRENCE")); mainLayout->addWidget(refEdit); mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("NOM MACHINE")); mainLayout->addWidget(nomEdit); mainLayout->addWidget(nomError);
    
    QHBoxLayout *c1 = new QHBoxLayout();
    QVBoxLayout *c1a = new QVBoxLayout(); c1a->addWidget(new QLabel("TYPE")); c1a->addWidget(typeCombo);
    QVBoxLayout *c1b = new QVBoxLayout(); c1b->addWidget(new QLabel("ÉTAT")); c1b->addWidget(etatCombo);
    c1->addLayout(c1a); c1->addLayout(c1b); mainLayout->addLayout(c1);

    QHBoxLayout *c2 = new QHBoxLayout();
    QVBoxLayout *c2a = new QVBoxLayout(); c2a->addWidget(new QLabel("CAPACITÉ")); c2a->addWidget(capaciteSpin);
    QVBoxLayout *c2b = new QVBoxLayout(); c2b->addWidget(new QLabel("FRÉQUENCE (Hz)")); c2b->addWidget(frequenceSpin);
    c2->addLayout(c2a); c2->addLayout(c2b); mainLayout->addLayout(c2);

    mainLayout->addWidget(new QLabel("NIVEAU DE CHARGE")); mainLayout->addWidget(chargeSpin);

    mainLayout->addSpacing(10); mainLayout->addWidget(createSeparator());
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("  ✓  METTRE À JOUR  "); btnSave->setObjectName("btnSave"); addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER"); btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave); btnLayout->addWidget(btnCancel); mainLayout->addLayout(btnLayout);

    auto validateAll = [&]() {
        bool allOk = true;
        if (refEdit->text().trimmed().isEmpty()) { setFieldError(refEdit, refError, true, "Obligatoire"); allOk = false; } else setFieldError(refEdit, refError, false);
        QString valNom = nomEdit->text().trimmed();
        if (valNom.isEmpty()) { 
            setFieldError(nomEdit, nomError, true, "Obligatoire"); allOk = false; 
        } else if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s']+$").match(valNom).hasMatch()) {
            setFieldError(nomEdit, nomError, true, "Lettres uniquement"); allOk = false;
        } else setFieldError(nomEdit, nomError, false);
        btnSave->setEnabled(allOk); return allOk;
    };
    QObject::connect(refEdit, &QLineEdit::textChanged, validateAll);
    QObject::connect(nomEdit, &QLineEdit::textChanged, validateAll);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject); connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("UPDATE SMARTLEATHER.MACHINE SET REF=:ref, NOM=:nom, TYPE=:type, ETAT=:etat, "
                      "CAPACITE=:cap, FREQUENCE=:freq, NIVEAU_DE_CHARGE=:charge WHERE ID_MACHINE=:id");
        query.bindValue(":ref", refEdit->text().trimmed());
        query.bindValue(":nom", nomEdit->text().trimmed());
        query.bindValue(":type", typeCombo->currentText());
        query.bindValue(":etat", etatCombo->currentText());
        query.bindValue(":cap", capaciteSpin->value());
        query.bindValue(":freq", frequenceSpin->value());
        query.bindValue(":charge", chargeSpin->value());
        query.bindValue(":id", id);

        if (query.exec()) { 
            QMessageBox::information(this, "Succès", "Machine modifiée !"); 
            
            // Advanced Feature: Email Alert if "En panne"
            if (etatCombo->currentText() == "En panne") {
                QString admEmail = "eyadkhil@aiesec.net"; 
                Smtp *smtp = new Smtp("dkhileya5@gmail.com", "bfnedrbmyguthysx");
                QString subject = "⚠️ ALERTE : Machine " + refEdit->text().trimmed() + " en panne";
                QString body = "La machine " + nomEdit->text().trimmed() + " (Réf: " + refEdit->text().trimmed() + ") vient d'être signalée EN PANNE.\n\nVeuillez intervenir rapidement.";
                
                connect(smtp, &Smtp::status, [this](const QString &msg){
                    QMessageBox::information(this, "Email", msg);
                });
                connect(smtp, &Smtp::error, [this](const QString &err){
                    QMessageBox::critical(this, "Erreur Email", err);
                });

                smtp->sendMail("eyadkhil5@gmail.com", admEmail, subject, body);
                
                QMessageBox::information(this, "Mail Envoyé", "Alerte : Un email contenant la référence et le nom de la machine a été envoyé à l'administration.");
            }
            
            loadMachines(); 
        }
        else {
            QString err = query.lastError().text();
            if (err.contains("ORA-00001")) {
                QMessageBox::critical(this, "Référence Existante", "Cette référence existe déjà. Veuillez en choisir une autre.");
            } else {
                QMessageBox::critical(this, "Erreur", "La modification a échoué.\nErreur technique: " + err.split('\n').first());
            }
        }
    }
}

void pagemachine::on_pushButton_3_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) { QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une machine."); return; }
    QString id = ui->tableWidget->item(currentRow, 0)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    if (QMessageBox::question(this, "Confirmation", "Supprimer machine " + nom + " ?") == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM SMARTLEATHER.MACHINE WHERE ID_MACHINE = :id");
        query.bindValue(":id", id);
        if (query.exec()) { QMessageBox::information(this, "Succès", "Machine supprimée."); loadMachines(); }
        else { QMessageBox::critical(this, "Erreur", "Échec: " + query.lastError().text()); }
    }
}

void pagemachine::on_pushButton_4_clicked() { loadMachines(); }

void pagemachine::on_pushButton_7_clicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection Requise", "Veuillez sélectionner une machine.");
        return;
    }

    QString ref = ui->tableWidget->item(currentRow, 1)->text();
    QString nom = ui->tableWidget->item(currentRow, 2)->text();
    QString type = ui->tableWidget->item(currentRow, 3)->text();
    QString etat = ui->tableWidget->item(currentRow, 4)->text();
    QString cap = ui->tableWidget->item(currentRow, 5)->text();
    QString freq = ui->tableWidget->item(currentRow, 6)->text();
    QString charge = ui->tableWidget->item(currentRow, 7)->text();

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "Diagnostic_" + ref + ".pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::ScreenResolution); // Fixed at 96 DPI for reliability
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    if (!painter.isActive()) return;

    int x = 50;
    int y = 50;

    // Content
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(x, y, "FICHE DE DIAGNOSTIC MACHINE");
    y += 50;

    painter.setFont(QFont("Arial", 11, QFont::Normal));
    painter.drawText(x, y, "Date: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    y += 40;

    auto drawRow = [&](const QString& label, const QString& value, int &currY) {
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(x, currY, label + ":");
        painter.setFont(QFont("Arial", 10, QFont::Normal));
        painter.drawText(x + 180, currY, value);
        currY += 30;
    };

    drawRow("Reference", ref, y);
    drawRow("Nom", nom, y);
    drawRow("Type", type, y);
    drawRow("Etat", etat, y);
    drawRow("Capacite", cap, y);
    drawRow("Frequence", freq, y);
    drawRow("Niveau Charge", charge, y);

    y += 40;
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(x, y, "Observations:");
    y += 20;
    painter.drawRect(x, y, 400, 100); // Simple box for notes

    painter.end();
    QMessageBox::information(this, "Succès", "Document généré !");
}

void pagemachine::on_pushButton_9_clicked() {
    int op = 0, maint = 0, arret = 0, panne = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        if (!ui->tableWidget->isRowHidden(row)) {
            QString e = ui->tableWidget->item(row, 4)->text();
            if (e == "Active") op++; 
            else if (e == "En maintenance") maint++; 
            else if (e == "En panne") panne++;
            else arret++;
        }
    }
    QPieSeries *series = new QPieSeries();
    if(op>0) { QPieSlice *s = series->append("Active", op); s->setBrush(QColor(110, 155, 58)); }
    if(maint>0) { QPieSlice *s = series->append("Maintenance", maint); s->setBrush(QColor(230, 150, 50)); }
    if(panne>0) { QPieSlice *s = series->append("En Panne", panne); s->setBrush(QColor(192, 57, 43)); s->setExploded(); }
    if(arret>0) { QPieSlice *s = series->append("Inactive", arret); s->setBrush(QColor(127, 140, 141)); }

    QChart *chart = new QChart(); 
    chart->addSeries(series); 
    chart->setTitle("📊 État du Parc Machines");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart); 
    chartView->setRenderHint(QPainter::Antialiasing);
    
    QDialog d(this); 
    d.resize(800, 550); 
    d.setWindowTitle("Statistiques Machines");
    d.setStyleSheet("background-color: #faf6f1;");
    QVBoxLayout *l = new QVBoxLayout(&d); 
    l->addWidget(chartView); 
    d.exec();
}


// ═══════════════════════════════════════════════
//   AGENDA DE MAINTENANCE PRÉVENTIVE
// ═══════════════════════════════════════════════
void pagemachine::on_btnAgenda_clicked()
{
    QDialog agenda(this);
    agenda.setWindowTitle("Agenda de Maintenance");
    agenda.setFixedSize(550, 480);
    agenda.setStyleSheet(QString(DIALOG_BASE_STYLE) + 
        "QCalendarWidget QWidget { alternate-background-color: #f0e8de; }"
        "QCalendarWidget QToolButton { color: #5b3a28; font-weight: bold; font-size: 14px; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(&agenda);
    
    QLabel *header = new QLabel("🗓️ AGENDA DE MAINTENANCE PRÉVENTIVE");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLabel *infoLabel = new QLabel("ℹ️ Aujourd'hui : <b>" + QDate::currentDate().toString("dd/MM/yyyy") + 
        "</b> &nbsp;&nbsp;|&nbsp;&nbsp; <i>Les dates antérieures à aujourd'hui sont verrouillées.</i>");
    infoLabel->setStyleSheet("color: #8b6f5a; font-size: 12px; margin-bottom: 5px;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);

    QCalendarWidget *calendar = new QCalendarWidget();
    calendar->setGridVisible(true);
    calendar->setMinimumHeight(400);
    calendar->setMinimumDate(QDate::currentDate()); // Block past dates
    mainLayout->addWidget(calendar);

    QString agendaFile = QApplication::applicationDirPath() + "/agenda_maintenance.txt";

    // Lambda to highlight calendar dates based on file records
    auto highlightCalendar = [&]() {
        QTextCharFormat fmtDefault;
        calendar->setDateTextFormat(QDate(), fmtDefault);

        QFile file(agendaFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
        
        QTextStream in(&file);
        QSet<QDate> highlightedDates;

        while (!in.atEnd()) {
            QStringList parts = in.readLine().split("|");
            if (parts.size() >= 4) {
                QDate d = QDate::fromString(parts[0], "yyyy-MM-dd");
                if (d >= QDate::currentDate()) {
                    highlightedDates.insert(d);
                }
            }
        }
        file.close();

        QTextCharFormat fmtHighlight;
        fmtHighlight.setBackground(QColor("#c9a87c"));
        fmtHighlight.setForeground(Qt::white);
        for (const QDate &d : highlightedDates) {
            calendar->setDateTextFormat(d, fmtHighlight);
        }
    };

    // Triggered when user explicitly CLICKS a specific date
    connect(calendar, &QCalendarWidget::clicked, [&](const QDate &date) {
        // Only open popup for today or future dates (safety net)
        if (date < QDate::currentDate()) return;

        // Block weekends (6 = Saturday, 7 = Sunday)
        if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
            QMessageBox::warning(&agenda, "Action Refusée", "Vous ne pouvez pas planifier d'intervention pendant le week-end.");
            return;
        }

        QDialog dayDialog(&agenda);
        dayDialog.setWindowTitle("Interventions - " + date.toString("dd/MM/yyyy"));
        dayDialog.setFixedSize(550, 450);
        dayDialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);
        
        QVBoxLayout *dayLayout = new QVBoxLayout(&dayDialog);
        
        // Top: Table of scheduled tasks for clicked date
        QTableWidget *taskTable = new QTableWidget();
        taskTable->setColumnCount(3);
        taskTable->setHorizontalHeaderLabels({"Heure", "Machine", "Intervention"});
        taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        taskTable->verticalHeader()->setVisible(false);
        taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        taskTable->setStyleSheet("background-color: #ffffff; border: 1px solid #d4c4b0; border-radius: 8px;");
        dayLayout->addWidget(new QLabel("Interventions programmées pour le " + date.toString("dd/MM/yyyy") + " :"));
        dayLayout->addWidget(taskTable);

        // Bottom: Add task form
        QFrame *addFrame = new QFrame();
        addFrame->setStyleSheet("background-color: rgba(255, 255, 255, 0.6); border: 1px solid #d4c4b0; border-radius: 8px; margin-top: 10px;");
        QGridLayout *addLayout = new QGridLayout(addFrame);
        addLayout->setHorizontalSpacing(15);
        addLayout->setVerticalSpacing(10);
        
        QComboBox *machineCombo = new QComboBox();
        machineCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        machineCombo->setMinimumHeight(40);
        QSqlQuery query;
        if (query.exec("SELECT REF, NOM FROM SMARTLEATHER.MACHINE")) {
            while (query.next()) machineCombo->addItem(query.value(0).toString() + " - " + query.value(1).toString());
        }
        if (machineCombo->count() == 0) machineCombo->addItem("-- Aucune machine trouvée --");

        QTime defaultTime = QTime::currentTime();
        if (defaultTime < QTime(8,0)) defaultTime = QTime(8,0);
        if (defaultTime > QTime(18,0)) defaultTime = QTime(18,0);
        
        QTimeEdit *timeEdit = new QTimeEdit(defaultTime);
        timeEdit->setMinimumHeight(40);
        timeEdit->setMinimumTime(QTime(8, 0));  // 8 AM
        timeEdit->setMaximumTime(QTime(18, 0)); // 6 PM
        
        QLineEdit *descEdit = new QLineEdit();
        descEdit->setPlaceholderText("Ex: Lubrification, Nettoyage...");
        descEdit->setMinimumHeight(40);
        
        addLayout->addWidget(new QLabel("Machine:"), 0, 0);
        addLayout->addWidget(machineCombo, 0, 1);
        addLayout->addWidget(new QLabel("Heure:"), 0, 2);
        addLayout->addWidget(timeEdit, 0, 3);
        addLayout->addWidget(new QLabel("Cause / Action:"), 1, 0);
        addLayout->addWidget(descEdit, 1, 1, 1, 2);

        QPushButton *btnAdd = new QPushButton(" ✓ Planifier");
        btnAdd->setStyleSheet("QPushButton { background-color: #6d9b3a; border: none; border-radius: 10px; color: white; font-weight: bold; font-size: 14px; } QPushButton:hover { background-color: #7dab4a; }");
        btnAdd->setMinimumHeight(45);
        addLayout->addWidget(btnAdd, 1, 3);
        
        dayLayout->addWidget(addFrame);

        auto loadDayTasks = [&]() {
            taskTable->setRowCount(0);
            QFile file(agendaFile);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
            QString targetDate = date.toString("yyyy-MM-dd");
            QTextStream in(&file);
            while (!in.atEnd()) {
                QStringList parts = in.readLine().split("|");
                if (parts.size() >= 4 && parts[0] == targetDate) {
                    int row = taskTable->rowCount();
                    taskTable->insertRow(row);
                    taskTable->setItem(row, 0, new QTableWidgetItem(parts[1]));
                    taskTable->setItem(row, 1, new QTableWidgetItem(parts[2]));
                    taskTable->setItem(row, 2, new QTableWidgetItem(parts[3]));
                }
            }
            file.close();
        };

        connect(btnAdd, &QPushButton::clicked, [&]() {
            QString actionText = descEdit->text().trimmed();
            
            // Input Validation
            if (actionText.isEmpty()) { 
                QMessageBox::warning(&dayDialog, "Erreur", "Veuillez entrer une cause/action."); 
                return; 
            }
            if (!QRegularExpression("^[A-Za-zÀ-ÿ\\s']+$").match(actionText).hasMatch()) { 
                QMessageBox::warning(&dayDialog, "Contrôle Saisie", "Seules les lettres et les espaces sont autorisés pour l'action."); 
                return; 
            }
            if (machineCombo->currentText().contains("--")) { 
                QMessageBox::warning(&dayDialog, "Erreur", "Veuillez sélectionner une machine valide."); 
                return; 
            }

            // Save to file
            QFile file(agendaFile);
            if (file.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream out(&file);
                out << date.toString("yyyy-MM-dd") << "|" << timeEdit->time().toString("HH:mm") << "|" << machineCombo->currentText() << "|" << actionText << "\n";
                file.close();
                QMessageBox::information(&dayDialog, "Succès", "Planifié pour le " + date.toString("dd/MM/yyyy") + " !");
                descEdit->clear();
                loadDayTasks(); // Refresh table in modal
            } else {
                QMessageBox::critical(&dayDialog, "Erreur", "Sauvegarde impossible : " + file.errorString());
            }
        });

        loadDayTasks();
        dayDialog.exec();
        
        // After modal is closed, refresh main calendar highlights
        highlightCalendar();
    });

    highlightCalendar();
    agenda.exec();
}


void pagemachine::on_pushButton_6_clicked() { hide(); pageemployee *pl = new pageemployee(m_idEmploye, this); pl->show(); }
void pagemachine::on_pushButton_21_clicked() { hide(); produitswindow *pd = new produitswindow(m_idEmploye, this); pd->show(); }
void pagemachine::on_pushButton_20_clicked() { hide(); commandes *pc = new commandes(m_idEmploye, this); pc->show(); }
void pagemachine::on_pushButton_22_clicked() { hide(); fournisseurs *pf = new fournisseurs(m_idEmploye, this); pf->show(); }
void pagemachine::on_pushButton_23_clicked() { hide(); Matieres *mm = new Matieres(m_idEmploye, this); mm->show(); }
void pagemachine::on_pushButton_11_clicked() { hide(); login *l = new login(); l->show(); }


// ═══════════════════════════════════════════════
//   VÉRIFICATION BACKGROUND MAINTENANCE (TIMER)
// ═══════════════════════════════════════════════
void pagemachine::checkUpcomingMaintenance()
{
    QString agendaFile = QApplication::applicationDirPath() + "/agenda_maintenance.txt";
    QFile file(agendaFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    QDate today = QDate::currentDate();
    QTime now = QTime::currentTime();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 4) { // date|time|machine|action
            QDate d = QDate::fromString(parts[0], "yyyy-MM-dd");
            QTime t = QTime::fromString(parts[1], "HH:mm");

            if (d == today) {
                int secsDiff = now.secsTo(t);
                // Si l'intervention est dans moins de 2 heures (7200 sec) et n'est pas encore passée
                if (secsDiff > 0 && secsDiff <= 7200) {
                    if (!m_notifiedTasks.contains(line)) {
                        m_notifiedTasks.insert(line);

                        // Envoi de l'email automatique de rappel
                        QString admEmail = "eyadkhil@aiesec.net"; 
                        Smtp *smtp = new Smtp("dkhileya5@gmail.com", "bfnedrbmyguthysx");
                        QString subject = "⏰ RAPPEL: Maintenance dans moins de 2 heures";
                        QString body = "Bonjour,\n\n"
                                       "Ceci est un rappel automatique de notre agenda de maintenance.\n"
                                       "Une intervention approche à grands pas !\n\n"
                                       "- Date : Aujourd'hui\n"
                                       "- Heure prévue : " + parts[1] + "\n"
                                       "- Machine : " + parts[2] + "\n"
                                       "- Action/Cause : " + parts[3] + "\n\n"
                                       "Veuillez prendre les dispositions nécessaires.\n\n"
                                       "Cordialement,\n"
                                       "Smart Leather Factory ERP";
                        
                        connect(smtp, &Smtp::status, smtp, &QObject::deleteLater);
                        connect(smtp, &Smtp::error, smtp, &QObject::deleteLater);

                        smtp->sendMail("eyadkhil5@gmail.com", admEmail, subject, body);
                    }
                }
            }
        }
    }
    file.close();
}

