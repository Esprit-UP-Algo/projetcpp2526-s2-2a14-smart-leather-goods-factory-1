#ifndef PAGEMACHINE_H
#define PAGEMACHINE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class pagemachine;
}

class CustomSqlModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomSqlModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Specific formatting for the last column (Level of Charge)
        if (role == Qt::DisplayRole && index.column() == 7) {
            return QSqlQueryModel::data(index, role).toString() + "%";
        }

        // Color coding for State (Column 4)
        if (index.column() == 4) {
            QString etat = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (etat == "Fonctionnelle") return QColor(200, 255, 200);
                if (etat == "En panne") return QColor(255, 200, 200);
                if (etat == "Maintenance") return QColor(255, 255, 200);
                if (etat == "Arrêtée") return QColor(200, 200, 200);
            }
            if (role == Qt::ForegroundRole) {
                if (etat == "Fonctionnelle") return QColor(0, 100, 0);
                if (etat == "En panne") return QColor(139, 0, 0);
                if (etat == "Maintenance") return QColor(128, 128, 0);
                if (etat == "Arrêtée") return QColor(70, 70, 70);
            }
        }

        // Color coding for Charge Level (Column 7)
        if (index.column() == 7) {
            if (role == Qt::BackgroundRole) {
                int charge = QSqlQueryModel::data(index, Qt::DisplayRole).toInt();
                if (charge > 80) return QColor(255, 200, 200);
                if (charge > 50) return QColor(255, 255, 200);
                return QColor(200, 255, 200);
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

class pagemachine : public QDialog
{
    Q_OBJECT

public:
    explicit pagemachine(QWidget *parent = nullptr);
    ~pagemachine();

private slots:
    void on_pushButton_6_clicked();
    void on_pushButton_11_clicked();
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
    void setupMachinesTable();
    void loadMachines();

private:
    Ui::pagemachine *ui;
    CustomSqlModel *model;
    void setupSearch();
};

#endif // PAGEMACHINE_H
