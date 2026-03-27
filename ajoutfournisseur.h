#ifndef AJOUTFOURNISSEUR_H
#define AJOUTFOURNISSEUR_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

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
    QString getAdresse() const { return adresseEdit->text(); }
    QDate getDelaiLivraison() const { return delaiDate->date(); }
    QString getQualiteMatiere() const { return qualiteEdit->text(); }
    QString getStatut() const { return statutCombo->currentText(); }

private:
    QLineEdit *idEdit;
    QLineEdit *nomEdit;
    QComboBox *typeCombo;
    QLineEdit *telephoneEdit;
    QLineEdit *adresseEdit;
    QDateEdit *delaiDate;
    QLineEdit *qualiteEdit;
    QComboBox *statutCombo;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupStyle(); // Applies the Stitched Leather QSS
};

#endif // AJOUTFOURNISSEUR_H
