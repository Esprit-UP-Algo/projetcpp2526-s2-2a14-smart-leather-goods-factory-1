#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDateTime>

namespace Ui {
class products;
}

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
    void addProductToTable(const QString &id, const QString &name,
                          const QString &category, const QString &price,
                          const QString &stock, const QString &status,
                          const QDate &creationDate);
    void updateProductInTable(int row, const QString &name,
                             const QString &category, const QString &price,
                             const QString &stock, const QString &status,
                             const QDate &creationDate);
    void updateRowColors(int row);
    void setupSearch();
};

#endif // PRODUCTS_H
