#ifndef MODIFIER_PRODUIT_H
#define MODIFIER_PRODUIT_H

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

class ModifierProduit : public QDialog {
    Q_OBJECT

public:
    explicit ModifierProduit(QWidget *parent = nullptr);

    // Function to fill the dialog with existing data from the table
    void setInitialData(QString id, QString ref, QString cat, float prix, int quantite, QString statut, QDate dCreation);

    // Getters to retrieve the modified data in the main window
    QString getId() const { return idEdit->text(); }
    QString getReference() const { return referenceEdit->text(); }
    QString getCategorie() const { return categorieEdit->text(); }
    float getPrix() const { return prixEdit->text().toFloat(); }
    int getQuantiteStock() const { return quantiteSpinBox->value(); }
    QString getStatut() const { return statusCombo->currentText(); }
    QDate getDateCreation() const { return dateCreationEdit->date(); }

public slots:
    void accept() override;

private:
    // UI Elements
    QLineEdit *idEdit;
    QLineEdit *referenceEdit;
    QLineEdit *categorieEdit;
    QLineEdit *prixEdit;
    QSpinBox *quantiteSpinBox;
    QComboBox *statusCombo;
    QDateTimeEdit *dateCreationEdit;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    // Theme method
    void setupStyle();
};

#endif // MODIFIER_PRODUIT_H
