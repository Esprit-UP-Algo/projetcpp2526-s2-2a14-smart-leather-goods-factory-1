#ifndef PAGEEMPLOYEE_H
#define PAGEEMPLOYEE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QColor>

namespace Ui {
class pageemployee;
}

class CustomEmployeeModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomEmployeeModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Salary Formatting (Column 4)
        if (index.column() == 4 && role == Qt::DisplayRole) {
            return QString::number(QSqlQueryModel::data(index, role).toDouble(), 'f', 2) + " €";
        }

        // Color coding for Status (Column 6)
        if (index.column() == 6) {
            QString statut = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (statut == "Actif") return QColor(200, 255, 200);
                if (statut == "Congé") return QColor(255, 200, 200);
                if (statut == "Formation") return QColor(255, 255, 200);
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

class pageemployee : public QDialog
{
    Q_OBJECT

public:
    explicit pageemployee(QWidget *parent = nullptr);
    ~pageemployee();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void filterTable();
    void setupEmployeesTable();
    void loadEmployees();
    void setupSearch();

private:
    Ui::pageemployee *ui;
    CustomEmployeeModel *model;
};

#endif // PAGEEMPLOYEE_H
