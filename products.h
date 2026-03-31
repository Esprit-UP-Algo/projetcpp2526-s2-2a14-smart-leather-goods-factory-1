#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QColor>

namespace Ui {
class products;
}

class CustomProductModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomProductModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Specific formatting for the Price column (Column 3)
        if (role == Qt::DisplayRole && index.column() == 3) {
            return QSqlQueryModel::data(index, role).toString() + " €";
        }

        // Color coding for Status (Column 5)
        if (index.column() == 5) {
            QString status = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (status == "Actif") return QColor(200, 255, 200);   // Green
                if (status == "Arrêté") return QColor(255, 200, 200); // Red
                if (status == "En attente") return QColor(255, 255, 200); // Yellow
            }
            if (role == Qt::ForegroundRole) {
                if (status == "Actif") return QColor(0, 100, 0);
                if (status == "Arrêté") return QColor(139, 0, 0);
                if (status == "En attente") return QColor(128, 128, 0);
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

class products : public QDialog
{
    Q_OBJECT

public:
    explicit products(QWidget *parent = nullptr);
    ~products();

private slots:
    void on_pushButton_6_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void filterTable();
    void setupProductsTable();
    void loadProducts();

private:
    Ui::products *ui;
    CustomProductModel *model;
    void setupSearch();
};

#endif // PRODUCTS_H
