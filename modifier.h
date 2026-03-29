#ifndef MODIFIER_H
#define MODIFIER_H

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class Modifier : public QDialog {
    Q_OBJECT

public:
    explicit Modifier(QWidget *parent = nullptr);

    // Function to fill the dialog with existing data from the table
    void setInitialData(QString id, QString client, QString addr, QString amount);
    void setDates(const QDateTime &dateOrderIn, const QDateTime &dateDeliveryIn);

    // Getters to retrieve the modified data in the main window
    QString getId() const { return idEdit->text(); }
    QString getClient() const { return clientEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    QString getAmount() const { return amountEdit->text(); }
    QString getStatus() const { return statusCombo->currentText(); }
    QDateTime getDateOrder() const { return dateOrder->dateTime(); }
    QDateTime getDateDelivery() const { return dateDelivery->dateTime(); }

private:
    // UI Elements
    QLineEdit *idEdit;
    QLineEdit *clientEdit;
    QLineEdit *addressEdit;
    QDateTimeEdit *dateOrder;
    QDateTimeEdit *dateDelivery;
    QComboBox *statusCombo;
    QLineEdit *amountEdit;

    QPushButton *btnSave;
    QPushButton *btnCancel;

    // Theme method
    void setupStyle();
    void accept() override;
};

#endif // MODIFIER_H
