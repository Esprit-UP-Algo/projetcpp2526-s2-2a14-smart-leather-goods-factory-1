#ifndef MODIFIERFOURNISSEUR_H
#define MODIFIERFOURNISSEUR_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>

class ModifierFournisseur : public QDialog {
    Q_OBJECT

public:
    explicit ModifierFournisseur(QWidget *parent = nullptr);

    // Function to fill the dialog with existing data from the table
    void setInitialData(QString nom, QString type, QString tel, QString addr,
                        QDate delai, QString qualite, QString statut,
                        double prixUnitaire, int capaciteMax, double tauxFiabilite);

    // Getters to retrieve the modified data in the main window
    QString getNomFournisseur() const { return nomEdit->text(); }
    QString getTypeMatiere() const { return typeCombo->currentText(); }
    QString getTelephone() const { return telephoneEdit->text(); }
    QString getAdresse() const;
    QDate getDelaiLivraison() const { return delaiDate->date(); }
    QString getQualiteMatiere() const { return qualiteCombo->currentText(); }
    QString getStatut() const { return statutCombo->currentText(); }
    double getPrixUnitaireEstime() const { return prixUnitaireSpin->value(); }
    int getCapaciteMax() const { return capaciteSpin->value(); }
    double getTauxFiabilite() const { return fiabiliteSpin->value(); }

private:
    // UI Elements
    QLineEdit *nomEdit;
    QComboBox *typeCombo;
    QLineEdit *telephoneEdit;
    QLineEdit *paysEdit;
    QLineEdit *regionEdit;
    QLineEdit *villeEdit;
    QLineEdit *rueEdit;
    QLineEdit *localisationExacteEdit;
    QDateEdit *delaiDate;
    QComboBox *qualiteCombo;
    QComboBox *statutCombo;
    QDoubleSpinBox *prixUnitaireSpin;
    QSpinBox *capaciteSpin;
    QDoubleSpinBox *fiabiliteSpin;
    QLabel *nomHintLabel;

    QPushButton *btnSave;
    QPushButton *btnCancel;
    bool m_validationEnabled = false;

    // Theme method
    void setupStyle();

private slots:
    void mettreAJourValidation();
    void ouvrirOptionsPlus();
    void validerEtAccepter();
};

#endif // MODIFIERFOURNISSEUR_H
