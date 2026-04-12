#ifndef AJOUT_H
#define AJOUT_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>

class Ajout : public QDialog
{
    Q_OBJECT

public:
    explicit Ajout(QWidget *parent = nullptr);

    // Getters for all fields
    QString getId() const { return idEdit->text(); }
    QString getClient() const { return clientEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    QString getDateCommande() const { return dateCommandeEdit->dateTime().toString("dd/MM/yyyy"); }
    QString getDateLivraison() const { return dateLivraisonEdit->dateTime().toString("dd/MM/yyyy"); }
    QString getEtat() const { return etatCombo->currentText(); }
    QString getMontant() const { return montantEdit->text(); }
    QString getModePaiement() const { return modePaiementCombo->currentText(); }

private slots:
    void setupStyle();

private:
    QLineEdit *idEdit;
    QLineEdit *clientEdit;
    QLineEdit *addressEdit;
    QDateTimeEdit *dateCommandeEdit;
    QDateTimeEdit *dateLivraisonEdit;
    QComboBox *etatCombo;
    QLineEdit *montantEdit;
    QComboBox *modePaiementCombo;
    QPushButton *btnSave;
    QPushButton *btnCancel;
};

#endif // AJOUT_H
