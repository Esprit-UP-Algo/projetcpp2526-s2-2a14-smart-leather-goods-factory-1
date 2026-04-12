#ifndef AJOUT_PRODUIT_H
#define AJOUT_PRODUIT_H

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

class AjoutProduit : public QDialog {
    Q_OBJECT

public:
    explicit AjoutProduit(QWidget *parent = nullptr);

    // Getters to retrieve data after closing
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
    QLineEdit *idEdit;
    QLineEdit *referenceEdit;
    QLineEdit *categorieEdit;
    QLineEdit *prixEdit;
    QSpinBox *quantiteSpinBox;
    QComboBox *statusCombo;
    QDateTimeEdit *dateCreationEdit;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupStyle(); // Applies the Stitched Leather QSS
};

#endif
