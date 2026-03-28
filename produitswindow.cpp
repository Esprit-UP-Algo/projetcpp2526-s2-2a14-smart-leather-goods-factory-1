#include "produitswindow.h"
#include "ui_produitswindow.h"
#include "connection.h"
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
produitswindow::produitswindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::produitswindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(P.afficher());

    // Activer l'édition par double-clic dans le tableau
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
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
    dialog.setFixedSize(440, 600);

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

    QSpinBox *quantiteSpinBox = new QSpinBox();
    quantiteSpinBox->setMaximum(100000);
    quantiteSpinBox->setMinimum(0);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"En approvisionnement", "En stock", "Rupture de stock"});

    QDateTimeEdit *dateCreationEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("CATÉGORIE"));
    mainLayout->addWidget(categorieEdit);
    mainLayout->addWidget(catError);

    QHBoxLayout *prixQteLayout = new QHBoxLayout();
    QVBoxLayout *prixCol = new QVBoxLayout();
    prixCol->addWidget(new QLabel("PRIX (DT)"));
    prixCol->addWidget(prixEdit);
    prixCol->addWidget(prixError);
    QVBoxLayout *qteCol = new QVBoxLayout();
    qteCol->addWidget(new QLabel("QUANTITÉ"));
    qteCol->addWidget(quantiteSpinBox);
    prixQteLayout->addLayout(prixCol);
    prixQteLayout->addSpacing(12);
    prixQteLayout->addLayout(qteCol);
    mainLayout->addLayout(prixQteLayout);

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
    QObject::connect(referenceEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(categorieEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(prixEdit, &QLineEdit::textChanged, [&]() { validateAll(); });

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_GREEN);

    if (dialog.exec() == QDialog::Accepted) {
        int nextId = Produit::getNextId();
        Produit p(QString::number(nextId), referenceEdit->text(), categorieEdit->text(),
                  prixEdit->text().toFloat(), quantiteSpinBox->value(),
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
    QString ref = model->data(model->index(currentRow, 1)).toString();
    QString cat = model->data(model->index(currentRow, 2)).toString();
    float prix = model->data(model->index(currentRow, 3)).toFloat();
    int qte = model->data(model->index(currentRow, 4)).toInt();
    QString statut = model->data(model->index(currentRow, 5)).toString();
    QDate dCreation = model->data(model->index(currentRow, 6)).toDate();

    QDialog dialog(this);
    dialog.setWindowTitle("Modifier le produit #" + id);
    dialog.setFixedSize(440, 650);

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

    QLineEdit *referenceEdit = new QLineEdit(ref);
    QLabel *refError = new QLabel(); refError->setObjectName("errorLabel"); refError->setVisible(false);
    QLineEdit *categorieEdit = new QLineEdit(cat);
    QLabel *catError = new QLabel(); catError->setObjectName("errorLabel"); catError->setVisible(false);
    QLineEdit *prixEdit = new QLineEdit(QString::number(prix));
    QLabel *prixError = new QLabel(); prixError->setObjectName("errorLabel"); prixError->setVisible(false);

    QSpinBox *quantiteSpinBox = new QSpinBox();
    quantiteSpinBox->setMaximum(100000);
    quantiteSpinBox->setMinimum(0);
    quantiteSpinBox->setValue(qte);

    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"En approvisionnement", "En stock", "Rupture de stock"});
    statusCombo->setCurrentText(statut);

    QDateTimeEdit *dateCreationEdit = new QDateTimeEdit(QDateTime(dCreation, QTime()));
    dateCreationEdit->setDisplayFormat("dd/MM/yyyy");
    dateCreationEdit->setCalendarPopup(true);

    mainLayout->addWidget(new QLabel("RÉFÉRENCE"));
    mainLayout->addWidget(referenceEdit);
    mainLayout->addWidget(refError);
    mainLayout->addWidget(new QLabel("CATÉGORIE"));
    mainLayout->addWidget(categorieEdit);
    mainLayout->addWidget(catError);

    QHBoxLayout *prixQteLayout = new QHBoxLayout();
    QVBoxLayout *prixCol = new QVBoxLayout();
    prixCol->addWidget(new QLabel("PRIX (DT)"));
    prixCol->addWidget(prixEdit);
    prixCol->addWidget(prixError);
    QVBoxLayout *qteCol = new QVBoxLayout();
    qteCol->addWidget(new QLabel("QUANTITÉ"));
    qteCol->addWidget(quantiteSpinBox);
    prixQteLayout->addLayout(prixCol);
    prixQteLayout->addSpacing(12);
    prixQteLayout->addLayout(qteCol);
    mainLayout->addLayout(prixQteLayout);

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
    QObject::connect(referenceEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(categorieEdit, &QLineEdit::textChanged, [&]() { validateAll(); });
    QObject::connect(prixEdit, &QLineEdit::textChanged, [&]() { validateAll(); });

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.setStyleSheet(QString(DIALOG_BASE_STYLE) + BTN_SAVE_AMBER);

    if (dialog.exec() == QDialog::Accepted) {
        Produit p(id, referenceEdit->text(), categorieEdit->text(),
                  prixEdit->text().toFloat(), quantiteSpinBox->value(),
                  statusCombo->currentText(), dateCreationEdit->date());

        if (p.modifier()) {
            QMessageBox::information(this, "Succès", "Produit mis à jour !");
            ui->tableView->setModel(P.afficher());
        } else {
            QMessageBox::critical(this, "Erreur", "La modification a échoué !");
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

    QString html;
    html += "<div style='text-align:center; margin-bottom:20px;'>";
    html += "<h1 style='color:#4a2517; font-family:Georgia,serif; letter-spacing:3px;'>SMART LEATHER GOODS</h1>";
    html += "<p style='color:#8b6f5a; font-family:Segoe UI; font-size:12px; letter-spacing:2px;'>INVENTAIRE DES PRODUITS</p>";
    html += "<hr style='border:1px solid #d4c4b0; width:60%;'/></div>";
    html += "<table border='0' cellspacing='0' cellpadding='10' style='border-collapse:collapse; width:100%; font-family:Segoe UI; font-size:11px;'>";
    html += "<tr>";
    for (int c = 0; c < model->columnCount(); c++)
        html += "<th style='background-color:#4a2517; color:#faf6f1; font-weight:bold; padding:12px 8px; letter-spacing:1px; border-bottom:3px solid #c9a87c;'>" + model->headerData(c, Qt::Horizontal).toString() + "</th>";
    html += "</tr>";
    for (int r = 0; r < model->rowCount(); r++) {
        QString bg = (r % 2 == 0) ? "#faf6f1" : "#f0e8de";
        html += "<tr style='background-color:" + bg + ";'>";
        for (int c = 0; c < model->columnCount(); c++)
            html += "<td style='padding:10px 8px; border-bottom:1px solid #e8ddd0; color:#3a2a20;'>" + model->data(model->index(r, c)).toString() + "</td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "<div style='text-align:center; margin-top:30px; color:#8b6f5a; font-size:10px;'><p>Smart Leather Goods Factory</p></div>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);
    QMessageBox::information(this, "Export PDF", "PDF exporté avec succès !");
}

// ═══════════════════════════════════════════════
//        🔍 RECHERCHE
// ═══════════════════════════════════════════════
void produitswindow::on_pushButton_10_clicked() {
    QString ref = ui->lineEdit_5->text();
    QString prix = ui->lineEdit_6->text();
    if (ref.trimmed().isEmpty() && prix.trimmed().isEmpty())
        ui->tableView->setModel(P.afficher());
    else
        ui->tableView->setModel(P.rechercher(ref, prix));
}

// ═══════════════════════════════════════════════
//        ⇅ TRI
// ═══════════════════════════════════════════════
void produitswindow::on_comboBox_tri_currentIndexChanged(int index) {
    ui->tableView->setModel(P.afficherTri(index));
}
