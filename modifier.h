#ifndef MODIFIER_H
#define MODIFIER_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>

class Modifier : public QDialog
{
    Q_OBJECT

public:
    explicit Modifier(QWidget *parent = nullptr);

    void setInitialData(const QString &id, const QString &client, const QString &address,
                       const QString &dateCommande, const QString &dateLivraison,
                       const QString &etat, const QString &montant, const QString &modePaiement, const QString &employeId);

    // Getters
    QString getClient() const { return clientEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    QString getDateCommande() const { return dateCommandeEdit->dateTime().toString("dd/MM/yyyy"); }
    QString getDateLivraison() const { return dateLivraisonEdit->dateTime().toString("dd/MM/yyyy"); }
    QString getEtat() const { return etatCombo->currentText(); }
    QString getMontant() const { return montantEdit->text(); }
    QString getModePaiement() const { return modePaiementCombo->currentText(); }
    QString getEmploye() const { return employeCombo->currentData().toString(); }

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
    QComboBox *employeCombo;
    QPushButton *btnSave;
    QPushButton *btnCancel;
};

#endif // MODIFIER_H
