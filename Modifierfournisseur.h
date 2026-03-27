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

class ModifierFournisseur : public QDialog {
    Q_OBJECT

public:
    explicit ModifierFournisseur(QWidget *parent = nullptr);

    // Function to fill the dialog with existing data from the table
    void setInitialData(QString nom, QString type, QString tel, QString addr,
                        QDate delai, QString qualite, QString statut);

    // Getters to retrieve the modified data in the main window
    QString getNomFournisseur() const { return nomEdit->text(); }
    QString getTypeMatiere() const { return typeCombo->currentText(); }
    QString getTelephone() const { return telephoneEdit->text(); }
    QString getAdresse() const { return adresseEdit->text(); }
    QDate getDelaiLivraison() const { return delaiDate->date(); }
    QString getQualiteMatiere() const { return qualiteEdit->text(); }
    QString getStatut() const { return statutCombo->currentText(); }

private:
    // UI Elements
    QLineEdit *nomEdit;
    QComboBox *typeCombo;
    QLineEdit *telephoneEdit;
    QLineEdit *adresseEdit;
    QDateEdit *delaiDate;
    QLineEdit *qualiteEdit;
    QComboBox *statutCombo;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    // Theme method
    void setupStyle();
};

#endif // MODIFIERFOURNISSEUR_H
