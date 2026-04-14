#ifndef AJOUTFOURNISSEUR_H
#define AJOUTFOURNISSEUR_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>

class AjoutFournisseur : public QDialog {
    Q_OBJECT

public:
    explicit AjoutFournisseur(QWidget *parent = nullptr);
    void setIdFournisseur(const QString &id) { idEdit->setText(id); }

    // Getters to retrieve data after closing
    QString getIdFournisseur() const { return idEdit->text(); }
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
    QLineEdit *idEdit;
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
    QLabel *idHintLabel;
    QLabel *nomHintLabel;

    QPushButton *btnSave;
    QPushButton *btnCancel;
    bool m_validationEnabled = false;

    void setupStyle(); // Applies the Stitched Leather QSS

private slots:
    void mettreAJourValidation();
    void ouvrirOptionsPlus();
    void validerEtAccepter();
};

#endif // AJOUTFOURNISSEUR_H
