#include "produitswindow.h"
#include "ui_produitswindow.h"
#include "connection.h"
#include "ajout_produit.h"
#include "modifier_produit.h"
#include "pageemployee.h"
#include "commandes.h"
#include "fournisseurs.h"
#include "matieres.h"
#include "pagemachine.h"
#include "login.h"
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QGraphicsDropShadowEffect>
#include <QFrame>
#include <QDoubleValidator>
#include <QSqlError>
#include <QDebug>
#include <QColor>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QHeaderView>
#include <QAbstractItemView>

// ═══════════════════════════════════════════════════
//   Styles communs pour les dialogues Ajout/Modifier
// ═══════════════════════════════════════════════════
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
QLineEdit, QSpinBox {
    background-color: rgba(255, 255, 255, 0.85);
    border: 2px solid #d4c4b0;
    border-radius: 12px;
    padding: 10px 14px;
    color: #3a2a20;
    font-size: 13px;
    selection-background-color: #c9a87c;
}
QLineEdit:focus, QSpinBox:focus {
    border: 2px solid #8b6f5a;
    background-color: white;
}
QLineEdit[error="true"] {
    border: 2px solid #e74c3c;
    background-color: #fdf2f2;
}
QLineEdit[error="true"]:focus {
    border: 2px solid #c0392b;
    background-color: #fdf2f2;
}
QDateTimeEdit {
    background-color: rgba(255, 255, 255, 0.85);
    border: 2px solid #d4c4b0;
    border-radius: 12px;
    padding: 10px 14px;
    color: #3a2a20;
    font-size: 13px;
}
QDateTimeEdit:focus {
    border: 2px solid #8b6f5a;
    background-color: white;
}
QComboBox {
    background-color: rgba(255, 255, 255, 0.85);
    border: 2px solid #d4c4b0;
    border-radius: 12px;
    padding: 10px 14px;
    color: #3a2a20;
    font-size: 13px;
}
QComboBox:focus {
    border: 2px solid #8b6f5a;
    background-color: white;
}
QComboBox::drop-down {
    border: none;
    padding-right: 10px;
}
QComboBox QAbstractItemView {
    background-color: #faf6f1;
    border: 2px solid #d4c4b0;
    border-radius: 8px;
    selection-background-color: #c9a87c;
    selection-color: #2a1a12;
    padding: 4px;
    color: #3a2a20;
}

/* ── CALENDAR POPUP ── */
QCalendarWidget {
    background-color: #faf6f1;
    color: #3a2a20;
}
QCalendarWidget QWidget#qt_calendar_navigationbar {
    background-color: #5b3020;
    color: #f5efe8;
    border-radius: 6px;
    min-height: 36px;
}
QCalendarWidget QToolButton {
    color: #f5efe8;
    background: transparent;
    font-weight: bold;
    font-size: 13px;
    padding: 4px 8px;
    border-radius: 4px;
}
QCalendarWidget QToolButton:hover {
    background-color: #7b5040;
}
QCalendarWidget QSpinBox {
    background-color: #4a2517;
    color: #f5efe8;
    border: none;
    border-radius: 4px;
    padding: 2px 6px;
    font-size: 13px;
    font-weight: bold;
}
QCalendarWidget QAbstractItemView {
    background-color: #faf6f1;
    color: #3a2a20;
    selection-background-color: #c9a87c;
    selection-color: #2a1a12;
    alternate-background-color: #f0e8de;
    font-size: 12px;
    border: none;
    outline: none;
}
QCalendarWidget QAbstractItemView:enabled {
    color: #3a2a20;
}
QCalendarWidget QAbstractItemView:disabled {
    color: #b0a090;
}
QCalendarWidget QMenu {
    background-color: #faf6f1;
    color: #3a2a20;
    border: 1px solid #d4c4b0;
}
QCalendarWidget QMenu::item:selected {
    background-color: #c9a87c;
    color: #2a1a12;
}

/* ── MESSAGE BOX ── */
QMessageBox {
    background-color: #faf6f1;
    color: #3a2a20;
}
QMessageBox QLabel {
    color: #3a2a20;
    font-size: 13px;
}
QMessageBox QPushButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #6b4530, stop:1 #4a2a18);
    border: none;
    border-radius: 8px;
    padding: 8px 24px;
    font-weight: 700;
    color: #f5efe8;
    min-width: 80px;
}
QMessageBox QPushButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
        stop:0 #7d5840, stop:1 #5b3a28);
}
)";

static const char* BTN_SAVE_GREEN = R"(
QPushButton#btnSave {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #6d9b3a, stop:1 #8fb85a);
    border: none; border-radius: 14px; padding: 12px 28px;
    font-weight: 700; font-size: 13px; color: white; letter-spacing: 1px;
}
QPushButton#btnSave:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7dab4a, stop:1 #9fc86a); }
QPushButton#btnSave:pressed { background: #5a8a2a; }
QPushButton#btnSave:disabled { background: #b0b0b0; color: #e0e0e0; }
QPushButton#btnCancel {
    background: transparent; border: 2px solid #c9b8a5; border-radius: 14px;
    padding: 12px 28px; font-weight: 600; font-size: 13px; color: #8b7a6a; letter-spacing: 1px;
}
QPushButton#btnCancel:hover { background: rgba(0,0,0,0.04); border-color: #a0907e; color: #5b4a3a; }
)";

static const char* BTN_SAVE_AMBER = R"(
QPushButton#btnSave {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
        stop:0 #c47a2c, stop:1 #e09a4c);
    border: none; border-radius: 14px; padding: 12px 28px;
    font-weight: 700; font-size: 13px; color: white; letter-spacing: 1px;
}
QPushButton#btnSave:hover { background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #d48a3c, stop:1 #f0aa5c); }
QPushButton#btnSave:pressed { background: #b06a1c; }
QPushButton#btnSave:disabled { background: #b0b0b0; color: #e0e0e0; }
QPushButton#btnCancel {
    background: transparent; border: 2px solid #c9b8a5; border-radius: 14px;
    padding: 12px 28px; font-weight: 600; font-size: 13px; color: #8b7a6a; letter-spacing: 1px;
}
QPushButton#btnCancel:hover { background: rgba(0,0,0,0.04); border-color: #a0907e; color: #5b4a3a; }
)";

// ── Helpers ──
static QFrame* createSeparator() {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #d4c4b0; max-height: 1px; margin: 8px 0;");
    return line;
}

static void addShadow(QWidget* w, int blur = 20, int offsetY = 4) {
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(w);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, offsetY);
    shadow->setColor(QColor(0, 0, 0, 40));
    w->setGraphicsEffect(shadow);
}

// ── Validation helper: set error property and refresh style ──
static void setFieldError(QLineEdit* field, QLabel* errorLabel, bool hasError, const QString& msg = "") {
    field->setProperty("error", hasError);
    field->style()->unpolish(field);
    field->style()->polish(field);
    errorLabel->setText(hasError ? "⚠ " + msg : "");
    errorLabel->setVisible(hasError);
}

// ═══════════════════════════════════════════════
produitswindow::produitswindow(int idEmployeConnecte, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::produitswindow)
    , m_idEmployeConnecte(idEmployeConnecte)
{
    ui->setupUi(this);
    ui->tableView->setModel(P.afficher());
    
    // Add navigation menu programmatically
    setupNavigationMenu();

    // Activer l'édition par double-clic dans le tableau
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    // --- Ajustement Design des Colonnes ---
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // ID
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Prix
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // Statut complet visible
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents); // Date ajustée

    // Premium Styling for Table
    ui->tableView->setStyleSheet(
        "QTableView { background: white; border: 2px solid #c9b8a5; border-radius: 16px; gridline-color: #f5eee6; "
        "selection-background-color: #f5eee6; selection-color: #3a1f14; }"
    );

    // Premium Styling for Horizontal Header
    ui->tableView->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5b3020, stop:0.5 #4a2517, stop:1 #3a1a10);"
        "    color: #f5efe8; padding: 10px; border: none; font-weight: bold; font-size: 11px; letter-spacing: 1px;"
        "}"
    );
}

produitswindow::~produitswindow()
{
    delete ui;
}

// ═══════════════════════════════════════════════
//        ✚ AJOUTER (sans champ ID)
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Nouveau produit");
    dialog.setFixedSize(440, 650);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✦ NOUVEAU PRODUIT");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLabel *subHeader = new QLabel("Remplissez les informations ci-dessous");
    subHeader->setObjectName("subHeaderLabel");
    subHeader->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subHeader);
    mainLayout->addWidget(createSeparator());

    // ── Champs avec erreurs ──
    QLineEdit *nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("ex: Sac en cuir");
    QLabel *nomError = new QLabel();
    nomError->setObjectName("errorLabel");
    nomError->setVisible(false);

    QLineEdit *referenceEdit = new QLineEdit();
    referenceEdit->setPlaceholderText("ex: REF-2026-001");
    QLabel *refError = new QLabel();
    refError->setObjectName("errorLabel");
    refError->setVisible(false);

    QLineEdit *categorieEdit = new QLineEdit();
    categorieEdit->setPlaceholderText("ex: Sac, Ceinture...");
    QLabel *catError = new QLabel();
    catError->setObjectName("errorLabel");
    catError->setVisible(false);

    QLineEdit *prixEdit = new QLineEdit();
    prixEdit->setPlaceholderText("ex: 149.90");
    QLabel *prixError = new QLabel();
    prixError->setObjectName("errorLabel");
    prixError->setVisible(false);

    QSpinBox *stockSpin = new QSpinBox();
    stockSpin->setRange(0, 999999);
    stockSpin->setValue(0);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Disponible", "Rupture", "Obsolète"});

    QDateTimeEdit *dateCreationEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("NOM PRODUIT"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(nomError);
    mainLayout->addWidget(new QLabel("RÉFÉRENCE"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("CATÉGORIE"));
    mainLayout->addWidget(categorieEdit);
    mainLayout->addWidget(catError);
    mainLayout->addWidget(new QLabel("PRIX (DT)"));
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(prixError);
    mainLayout->addWidget(new QLabel("QUANTITÉ STOCK"));
    mainLayout->addWidget(stockSpin);

    QHBoxLayout *statDateLayout = new QHBoxLayout();
    QVBoxLayout *statCol = new QVBoxLayout();
    statCol->addWidget(new QLabel("STATUT"));
    statCol->addWidget(statusCombo);
    QVBoxLayout *dateCol = new QVBoxLayout();
    dateCol->addWidget(new QLabel("DATE CRÉATION"));
    dateCol->addWidget(dateCreationEdit);
    statDateLayout->addLayout(statCol);
    statDateLayout->addSpacing(12);
    statDateLayout->addLayout(dateCol);
    mainLayout->addLayout(statDateLayout);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(createSeparator());

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    QPushButton *btnSave = new QPushButton("  ✓  ENREGISTRER  ");
    btnSave->setObjectName("btnSave");
    addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // ── Validation en temps réel ──
    auto validateAll = [&]() {
        bool allOk = true;
        // Nom
        if (nomEdit->text().trimmed().isEmpty()) {
            setFieldError(nomEdit, nomError, true, "Le nom est obligatoire");
            allOk = false;
        } else {
            setFieldError(nomEdit, nomError, false);
        }
        // Référence
        if (referenceEdit->text().trimmed().isEmpty()) {
            setFieldError(referenceEdit, refError, true, "La référence est obligatoire");
            allOk = false;
        } else {
            setFieldError(referenceEdit, refError, false);
        }
        // Catégorie
        if (categorieEdit->text().trimmed().isEmpty()) {
            setFieldError(categorieEdit, catError, true, "La catégorie est obligatoire");
            allOk = false;
        } else {
            setFieldError(categorieEdit, catError, false);
        }
        // Prix
        bool ok;
        float p = prixEdit->text().toFloat(&ok);
        if (prixEdit->text().trimmed().isEmpty()) {
            setFieldError(prixEdit, prixError, true, "Le prix est obligatoire");
            allOk = false;
        } else if (!ok || p <= 0) {
            setFieldError(prixEdit, prixError, true, "Nombre > 0 requis");
            allOk = false;
        } else {
            setFieldError(prixEdit, prixError, false);
        }
        btnSave->setEnabled(allOk);
        return allOk;
    };

    btnSave->setEnabled(false); // Désactivé par défaut
    QObject::connect(nomEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(referenceEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(categorieEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(prixEdit, &QLineEdit::textChanged, [&]() { validateAll(); });

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        Produit p("", nomEdit->text(), referenceEdit->text(), categorieEdit->text(),
                  prixEdit->text().toFloat(),
                  stockSpin->value(),
                  statusCombo->currentText(), dateCreationEdit->date());

        if (p.ajouter()) {
            QMessageBox::information(this, "Succès", "Produit ajouté avec succès !");
            ui->tableView->setModel(P.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "L'ajout a échoué.");
        }
    }
}

// ═══════════════════════════════════════════════
//        ✎ MODIFIER
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_2_clicked() {
    int currentRow = ui->tableView->currentIndex().row();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un produit à modifier.");
        return;
    }

    QAbstractItemModel *model = ui->tableView->model();
    QString id = model->data(model->index(currentRow, 0)).toString();
    QString nom = model->data(model->index(currentRow, 1)).toString();
    QString ref = model->data(model->index(currentRow, 2)).toString();
    QString cat = model->data(model->index(currentRow, 3)).toString();
    float prix = model->data(model->index(currentRow, 4)).toFloat();
    int stock = model->data(model->index(currentRow, 5)).toInt();
    QString statut = model->data(model->index(currentRow, 6)).toString();
    QDate dCreation = QDate::fromString(model->data(model->index(currentRow, 7)).toString(), "dd/MM/yyyy");

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier le produit #" + id);
    dialog.setFixedSize(440, 700);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(4);

    QLabel *header = new QLabel("✎ MODIFIER PRODUIT");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    QLabel *subHeader = new QLabel("Modification du produit ID: " + id);
    subHeader->setObjectName("subHeaderLabel");
    subHeader->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subHeader);
    mainLayout->addWidget(createSeparator());

    QLineEdit *idEdit = new QLineEdit(id);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("QLineEdit { background-color: #ece4da; color: #9a8a7a; border: 2px dashed #d4c4b0; border-radius: 12px; padding: 10px 14px; }");
    mainLayout->addWidget(new QLabel("ID PRODUIT"));
    mainLayout->addWidget(idEdit);

    QLineEdit *nomEdit = new QLineEdit(nom);
    QLabel *nomError = new QLabel(); nomError->setObjectName("errorLabel"); nomError->setVisible(false);
    QLineEdit *referenceEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);
    QLineEdit *categorieEdit = new QLineEdit(cat);
    QLabel *catError = new QLabel(); catError->setObjectName("errorLabel"); catError->setVisible(false);
    QLineEdit *prixEdit = new QLineEdit(QString::number(prix));
    QLabel *prixError = new QLabel(); prixError->setObjectName("errorLabel"); prixError->setVisible(false);

    QSpinBox *stockSpin = new QSpinBox();
    stockSpin->setRange(0, 999999);
    stockSpin->setValue(stock);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Disponible", "Rupture", "Obsolète"});
    statusCombo->setCurrentText(statut);

    QDateTimeEdit *dateCreationEdit = new QDateTimeEdit(QDateTime(dCreation, QTime()));
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("NOM PRODUIT"));
    mainLayout->addWidget(nomEdit);
    mainLayout->addWidget(nomError);
    mainLayout->addWidget(new QLabel("RÉFÉRENCE"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("CATÉGORIE"));
    mainLayout->addWidget(categorieEdit);
    mainLayout->addWidget(catError);
    mainLayout->addWidget(new QLabel("PRIX (DT)"));
    mainLayout->addWidget(prixEdit);
    mainLayout->addWidget(prixError);
    mainLayout->addWidget(new QLabel("QUANTITÉ STOCK"));
    mainLayout->addWidget(stockSpin);

    QHBoxLayout *statDateLayout = new QHBoxLayout();
    QVBoxLayout *statCol = new QVBoxLayout();
    statCol->addWidget(new QLabel("STATUT"));
    statCol->addWidget(statusCombo);
    QVBoxLayout *dateCol = new QVBoxLayout();
    dateCol->addWidget(new QLabel("DATE CRÉATION"));
    dateCol->addWidget(dateCreationEdit);
    statDateLayout->addLayout(statCol);
    statDateLayout->addSpacing(12);
    statDateLayout->addLayout(dateCol);
    mainLayout->addLayout(statDateLayout);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(createSeparator());

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);
    QPushButton *btnSave = new QPushButton("  ✓  METTRE À JOUR  ");
    btnSave->setObjectName("btnSave");
    addShadow(btnSave, 15, 3);
    QPushButton *btnCancel = new QPushButton("ANNULER");
    btnCancel->setObjectName("btnCancel");
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    // ── Validation en temps réel ──
    auto validateAll = [&]() {
        bool allOk = true;
        if (nomEdit->text().trimmed().isEmpty()) {
            setFieldError(nomEdit, nomError, true, "Le nom est obligatoire");
            allOk = false;
        } else { setFieldError(nomEdit, nomError, false); }
        if (referenceEdit->text().trimmed().isEmpty()) {
            setFieldError(referenceEdit, refError, true, "La référence est obligatoire");
            allOk = false;
        } else { setFieldError(referenceEdit, refError, false); }
        if (categorieEdit->text().trimmed().isEmpty()) {
            setFieldError(categorieEdit, catError, true, "La catégorie est obligatoire");
            allOk = false;
        } else { setFieldError(categorieEdit, catError, false); }
        bool ok;
        float p = prixEdit->text().toFloat(&ok);
        if (prixEdit->text().trimmed().isEmpty()) {
            setFieldError(prixEdit, prixError, true, "Le prix est obligatoire");
            allOk = false;
        } else if (!ok || p <= 0) {
            setFieldError(prixEdit, prixError, true, "Nombre > 0 requis");
            allOk = false;
        } else { setFieldError(prixEdit, prixError, false); }
        btnSave->setEnabled(allOk);
        return allOk;
    };

    validateAll(); // check initial values
    QObject::connect(nomEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(referenceEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(categorieEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(prixEdit, &QLineEdit::textChanged, [&]() { validateAll(); });

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        Produit p(id, nomEdit->text(), referenceEdit->text(), categorieEdit->text(),
                  prixEdit->text().toFloat(),
                  stockSpin->value(),
                  statusCombo->currentText(), dateCreationEdit->date());

        if (p.modifier()) {
            QMessageBox::information(this, "Succès", "Produit mis à jour !");
            ui->tableView->setModel(P.afficher());
        } else {
            // Get the last error from the database connection
            QString err = QSqlDatabase::database().lastError().text();
            if (err.contains("ORA-00001")) {
                QMessageBox::critical(this, "Référence Existante", "Cette référence existe déjà. Veuillez en choisir une autre.");
            } else {
                QMessageBox::critical(this, "Erreur", "La modification a échoué !");
            }
        }
    }
}

// ═══════════════════════════════════════════════
//        ✕ SUPPRIMER
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_3_clicked() {
    int currentRow = ui->tableView->currentIndex().row();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un produit à supprimer.");
        return;
    }

    QString id = ui->tableView->model()->data(ui->tableView->model()->index(currentRow, 0)).toString();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                        "Êtes-vous sûr de vouloir supprimer le produit (ID: " + id + ") ?",
                        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (P.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Produit supprimé !");
            ui->tableView->setModel(P.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué !");
        }
    }
}

// ═══════════════════════════════════════════════
//        ↻ ACTUALISER
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_4_clicked() {
    ui->tableView->setModel(P.afficher());
    QMessageBox::information(this, "Actualisation", "La table a été mise à jour !");
}

// ═══════════════════════════════════════════════
//        📄 EXPORTER PDF
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_7_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en PDF", "", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    QAbstractItemModel *model = ui->tableView->model();
    if (!model) { QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter."); return; }

    QString html = "<div style='text-align:center; font-family:Segoe UI, Arial, sans-serif;'>";
    html += "<h1 style='color:#3a1f14; font-size:32px; letter-spacing:4px; margin-bottom:5px; margin-top:10px;'>SMART LEATHER</h1>";
    html += "<h3 style='color:#8b6f5a; font-size:16px; margin-top:0px; font-weight: normal; letter-spacing:2px;'>COLLECTION OFFICIELLE</h3>";
    html += "<hr style='border:1px solid #c9a87c; width:50%; margin-bottom:30px;'/></div>";

    html += "<table width='100%' cellspacing='15' cellpadding='15' style='font-family:Segoe UI, Arial;'>";
    
    for (int r = 0; r < model->rowCount(); r++) {
        if (r % 2 == 0) html += "<tr>"; // Début de ligne (2 produits par ligne)

        QString nom = model->data(model->index(r, 1)).toString();
        QString ref = model->data(model->index(r, 2)).toString();
        QString cat = model->data(model->index(r, 3)).toString();
        QString prix = model->data(model->index(r, 4)).toString();
        QString stock = model->data(model->index(r, 5)).toString();
        QString statut = model->data(model->index(r, 6)).toString();
        QString dateC = model->data(model->index(r, 7)).toString();

        QString statutColor = (statut.toLower().contains("rupture")) ? "#b04a3a" : "#5a8a2a";

        html += "<td width='50%' style='background-color:#faf6f1; border:1px solid #d4c4b0; padding:15px; border-radius:8px;'>";
        html += "  <h2 style='color:#4a2517; font-size:18px; margin:0px; text-transform:uppercase;'>" + nom + "</h2>";
        html += "  <p style='color:#8b6f5a; font-size:11px; margin-top:2px; margin-bottom:12px;'>Réf : <b style='color:#3a1f14;'>" + ref + "</b> &nbsp;|&nbsp; Catégorie : <b>" + cat + "</b></p>";
        html += "  <p style='color:#3a2a20; font-size:22px; font-weight:bold; margin-top:0px; margin-bottom:5px; color:#c07a2c'>" + prix + " DT <span style='font-size:12px; color:#8b6f5a;'>(" + stock + " en stock)</span></p>";
        html += "  <p style='font-size:11px; margin:0px; font-weight:bold; color:" + statutColor + ";'>" + statut + " <span style='font-weight:normal; color:#8b6f5a;'>— Ajouté le " + dateC + "</span></p>";
        html += "</td>";

        if (r % 2 == 1 || r == model->rowCount() - 1) {
            // Si c'est le dernier produit et qu'il est seul sur la ligne, ajouter une cellule vide pour l'alignement
            if (r == model->rowCount() - 1 && r % 2 == 0) html += "<td width='50%'></td>";
            html += "</tr>"; 
        }
    }
    
    html += "</table>";
    html += "<div style='text-align:center; margin-top:40px; color:#8b6f5a; font-size:10px;'><p>© Smart Leather Goods Factory — Généré automatiquement</p></div>";


    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);
    QMessageBox::information(this, "Succès", "Catalogue exporté avec succès en PDF !");
}

// ═══════════════════════════════════════════════
//        📈 STATISTIQUES
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_9_clicked()
{
    QMap<QString, int> statsCat = P.statistiquesParCategorie();
    QMap<QString, int> statsStatut = P.statistiquesParStatut();

    if (statsCat.isEmpty() && statsStatut.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucune donnée disponible pour afficher les statistiques.");
        return;
    }

    // --- 1. Graphique Catégories ---
    QPieSeries *seriesCat = new QPieSeries();
    QList<QColor> colorsCat = {QColor("#5b3020"), QColor("#c9a87c"), QColor("#7aaa4a"), QColor("#b06a1c"), QColor("#4a2a18")};
    int colorIndex = 0;
    
    for (auto it = statsCat.cbegin(); it != statsCat.cend(); ++it) {
        QString label = it.key() + " (" + QString::number(it.value()) + ")";
        QPieSlice *slice = seriesCat->append(label, it.value());
        slice->setBrush(colorsCat[colorIndex % colorsCat.size()]);
        slice->setLabelVisible(true);
        slice->setLabelColor(QColor("#3a2a20"));
        slice->setLabelFont(QFont("Segoe UI", 10, QFont::Bold));
        colorIndex++;
    }

    QChart *chartCat = new QChart();
    chartCat->addSeries(seriesCat);
    chartCat->setTitle("Répartition par Catégorie");
    chartCat->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chartCat->setTitleBrush(QBrush(QColor("#3a1a10")));
    chartCat->setAnimationOptions(QChart::AllAnimations);
    chartCat->setBackgroundBrush(QBrush(Qt::transparent));
    chartCat->legend()->setAlignment(Qt::AlignBottom);
    chartCat->legend()->setFont(QFont("Segoe UI", 10));

    QChartView *chartViewCat = new QChartView(chartCat);
    chartViewCat->setRenderHint(QPainter::Antialiasing);
    chartViewCat->setStyleSheet("background-color: transparent; border: none;");

    // --- 2. Graphique Statut d'Inventaire ---
    QPieSeries *seriesStatut = new QPieSeries();
    for (auto it = statsStatut.cbegin(); it != statsStatut.cend(); ++it) {
        QString label = it.key() + " (" + QString::number(it.value()) + ")";
        QPieSlice *slice = seriesStatut->append(label, it.value());
        
        // Couleur intelligente selon le statut
        QString statutKey = it.key().toLower();
        if (statutKey.contains("rupture")) {
            slice->setBrush(QColor("#b04a3a")); // Rouge bordeaux
            slice->setExploded(true);           // Mettre en évidence (détaché)
            slice->setExplodeDistanceFactor(0.1);
        } else if (statutKey.contains("approvisionnement")) {
            slice->setBrush(QColor("#d68e3a")); // Orange/Ambre
        } else {
            slice->setBrush(QColor("#5a8a2a")); // Vert forêt
        }
        
        slice->setLabelVisible(true);
        slice->setLabelColor(QColor("#3a2a20"));
        slice->setLabelFont(QFont("Segoe UI", 10, QFont::Bold));
    }

    QChart *chartStatut = new QChart();
    chartStatut->addSeries(seriesStatut);
    chartStatut->setTitle("État de l'Inventaire (Statut)");
    chartStatut->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chartStatut->setTitleBrush(QBrush(QColor("#3a1a10")));
    chartStatut->setAnimationOptions(QChart::AllAnimations);
    chartStatut->setBackgroundBrush(QBrush(Qt::transparent));
    chartStatut->legend()->setAlignment(Qt::AlignBottom);
    chartStatut->legend()->setFont(QFont("Segoe UI", 10));

    QChartView *chartViewStatut = new QChartView(chartStatut);
    chartViewStatut->setRenderHint(QPainter::Antialiasing);
    chartViewStatut->setStyleSheet("background-color: transparent; border: none;");

    // --- Fenêtre de dialogue Principale ---
    QDialog dialog(this);
    dialog.setWindowTitle("Tableau de Bord / Cockpit");
    dialog.resize(950, 450);  // Assez large pour contenir les deux graphiques
    dialog.setStyleSheet("QDialog { background-color: #faf6f1; }");

    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(chartViewCat);
    layout->addWidget(chartViewStatut);

    dialog.exec();
}

// ═══════════════════════════════════════════════
//        🔍 RECHERCHE
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_10_clicked() {
    QString nomOrRef = ui->lineEdit_5->text();
    QString prix = ui->lineEdit_6->text();
    if (nomOrRef.trimmed().isEmpty() && prix.trimmed().isEmpty())
        ui->tableView->setModel(P.afficher());
    else
        ui->tableView->setModel(P.rechercher(nomOrRef, prix));
}

// ═══════════════════════════════════════════════
//        ⇅ TRI
// ═══════════════════════════════════════════════
void produitswindow::on_comboBox_tri_currentIndexChanged(int index) {
    ui->tableView->setModel(P.afficherTri(index));
}

// ═══════════════════════════════════════════════════
//        🚀 NAVIGATION
// ═══════════════════════════════════════════════════
void produitswindow::setupNavigationMenu() {
    QString navBtnStyle =
        "QPushButton {"
        "  background: transparent;"
        "  border: none;"
        "  color: #c9b8a5;"
        "  text-align: left;"
        "  padding-left: 20px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 255, 255, 0.1);"
        "  color: white;"
        "  border-left: 4px solid #c9a87c;"
        "}";

    QWidget *sidebar = new QWidget(this);
    sidebar->setGeometry(0, 0, 240, 716);
    sidebar->setStyleSheet("background-color: #3a1f14;");
    sidebar->raise(); sidebar->show();

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

    // Bouton Employés
    QPushButton *btnEmp = new QPushButton("  Employés");
    btnEmp->setMinimumHeight(45);
    btnEmp->setStyleSheet(navBtnStyle);
    connect(btnEmp, &QPushButton::clicked, this, &produitswindow::navToEmploye);
    navLayout->addWidget(btnEmp);

    // Bouton Produits (actif)
    QPushButton *btnProd = new QPushButton("  Produits");
    btnProd->setMinimumHeight(45);
    btnProd->setStyleSheet(navBtnStyle +
        "QPushButton { background-color: rgba(255,255,255,0.1); color:white; border-left:4px solid #c9a87c; }");
    navLayout->addWidget(btnProd);

    // Bouton Commandes
    QPushButton *btnCmd = new QPushButton("  Commandes");
    btnCmd->setMinimumHeight(45);
    btnCmd->setStyleSheet(navBtnStyle);
    connect(btnCmd, &QPushButton::clicked, this, &produitswindow::navToCommandes);
    navLayout->addWidget(btnCmd);

    // Bouton Fournisseurs
    QPushButton *btnFour = new QPushButton("  Fournisseurs");
    btnFour->setMinimumHeight(45);
    btnFour->setStyleSheet(navBtnStyle);
    connect(btnFour, &QPushButton::clicked, this, &produitswindow::navToFournisseurs);
    navLayout->addWidget(btnFour);

    // Bouton Matières
    QPushButton *btnMat = new QPushButton("  Matières");
    btnMat->setMinimumHeight(45);
    btnMat->setStyleSheet(navBtnStyle);
    connect(btnMat, &QPushButton::clicked, this, &produitswindow::navToMatieres);
    navLayout->addWidget(btnMat);

    // Bouton Machines
    QPushButton *btnMach = new QPushButton("  Machines");
    btnMach->setMinimumHeight(45);
    btnMach->setStyleSheet(navBtnStyle);
    connect(btnMach, &QPushButton::clicked, this, &produitswindow::navToMachines);
    navLayout->addWidget(btnMach);

    navLayout->addStretch();

    // Bouton Déconnexion
    QPushButton *btnLogout = new QPushButton("  Déconnexion");
    btnLogout->setMinimumHeight(45);
    btnLogout->setStyleSheet(navBtnStyle);
    connect(btnLogout, &QPushButton::clicked, this, &produitswindow::navToLogout);
    navLayout->addWidget(btnLogout);


}

void produitswindow::navToEmploye() {
    this->hide();
    pageemployee *p = new pageemployee(m_idEmployeConnecte);
    p->show();
}

void produitswindow::navToCommandes() {
    this->hide();
    commandes *c = new commandes(m_idEmployeConnecte);
    c->show();
}

void produitswindow::navToFournisseurs() {
    this->hide();
    fournisseurs *f = new fournisseurs(m_idEmployeConnecte);
    f->show();
}

void produitswindow::navToMatieres() {
    this->hide();
    Matieres *m = new Matieres(m_idEmployeConnecte);
    m->show();
}

void produitswindow::navToMachines() {
    this->hide();
    pagemachine *pm = new pagemachine(m_idEmployeConnecte);
    pm->show();
}

void produitswindow::navToLogout() {
    this->hide();
    login *l = new login();
    l->show();
}
