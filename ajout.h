#ifndef AJOUT_H
#define AJOUT_H

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class Ajout : public QDialog {
    Q_OBJECT

public:
    explicit Ajout(QWidget *parent = nullptr);

    // Getters to retrieve data after closing
    QString getReference() const { return referenceEdit->text(); }
    QString getClient() const { return clientEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    QDateTime getDateOrder() const { return dateOrder->dateTime(); }
    QDateTime getDateDelivery() const { return dateDelivery->dateTime(); }
    QString getStatus() const { return statusCombo->currentText(); }
    QString getAmount() const { return amountEdit->text(); }

private:
    QLineEdit *referenceEdit;
    QLabel *errorRef;
    QLineEdit *clientEdit;
    QLabel *errorClient;
    QLineEdit *addressEdit;
    QLabel *errorAddr;
    QDateTimeEdit *dateOrder;
    QDateTimeEdit *dateDelivery;
    QLabel *errorDate;
    QComboBox *statusCombo;
    QLineEdit *amountEdit;
    QLabel *errorAmount;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupStyle(); // Applies the Stitched Leather QSS
    void accept() override;

private slots:
    void validateForm();
};

#endif
