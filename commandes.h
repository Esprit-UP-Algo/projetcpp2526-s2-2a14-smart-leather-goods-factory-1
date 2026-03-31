#ifndef COMMANDES_H
#define COMMANDES_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QColor>

namespace Ui {
class commandes;
}

class CustomCommandeModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomCommandeModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Amount Formatting (Column 6)
        if (index.column() == 6 && role == Qt::DisplayRole) {
            return QSqlQueryModel::data(index, role).toString() + " €";
        }

        // Color coding for State (Column 5)
        if (index.column() == 5) {
            QString etat = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (etat == "Livrée") return QColor(200, 255, 200);        // Green
                if (etat == "Annulée") return QColor(255, 200, 200);       // Red
                if (etat == "En attente") return QColor(255, 255, 200);    // Yellow
                if (etat == "Confirmée") return QColor(200, 230, 255);     // Blue
                if (etat == "En préparation") return QColor(255, 200, 230); // Pink
            }
            if (role == Qt::ForegroundRole) {
                if (etat == "Livrée") return QColor(0, 100, 0);
                if (etat == "Annulée") return QColor(139, 0, 0);
                if (etat == "En attente") return QColor(128, 128, 0);
                if (etat == "Confirmée") return QColor(0, 0, 139);
                if (etat == "En préparation") return QColor(139, 0, 139);
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

class commandes : public QMainWindow
{
    Q_OBJECT

public:
    explicit commandes(QWidget *parent = nullptr);
    ~commandes();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_lineEdit_5_textChanged(const QString &text);
    void on_lineEdit_6_textChanged(const QString &text);

private:
    Ui::commandes *ui;
    CustomCommandeModel *model;
    void setupTable();
    void loadSampleData();
    void filterTable(const QString &idFilter, const QString &dateFilter);
};

#endif // COMMANDES_H
