#ifndef MATIERES_H
#define MATIERES_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QColor>

class CustomMatiereModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomMatiereModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Price Column (Column 5) Display Formatting
        if (role == Qt::DisplayRole && index.column() == 5) {
            return QSqlQueryModel::data(index, role).toString() + " €";
        }

        // Color coding for Quality (Column 2)
        if (index.column() == 2) {
            QString qualite = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (qualite.contains("A")) return QColor(200, 255, 200); // Green
                if (qualite.contains("B")) return QColor(255, 255, 200); // Yellow
                if (qualite.contains("C")) return QColor(255, 200, 200); // Red
            }
            if (role == Qt::ForegroundRole) {
                if (qualite.contains("A")) return QColor(0, 100, 0);
                if (qualite.contains("B")) return QColor(128, 128, 0);
                if (qualite.contains("C")) return QColor(139, 0, 0);
            }
        }

        // Color coding for Stock level (Column 6) vs Threshold (Column 7)
        if (index.column() == 6 && role == Qt::BackgroundRole) {
            int stock = QSqlQueryModel::data(index, Qt::DisplayRole).toInt();
            int seuil = QSqlQueryModel::data(this->index(index.row(), 7), Qt::DisplayRole).toInt();
            if (stock < seuil) return QColor(255, 200, 200); // Red if below threshold
            else if (stock < seuil * 1.5) return QColor(255, 255, 200); // Yellow if warning
        }

        return QSqlQueryModel::data(index, role);
    }
};

namespace Ui {
class Matieres;
}

class Matieres : public QDialog
{
    Q_OBJECT

public:
    explicit Matieres(QWidget *parent = nullptr);
    ~Matieres();

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
    void setupMatieresTable();
    void loadMatieres();

private:
    Ui::Matieres *ui;
    CustomMatiereModel *model;
    void setupSearch();
};

#endif // MATIERES_H
