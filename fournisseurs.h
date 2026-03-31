#ifndef FOURNISSEURS_H
#define FOURNISSEURS_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QColor>

class CustomFournisseurModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit CustomFournisseurModel(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        // Color coding for Quality (Column 6)
        if (index.column() == 6) {
            QString qualite = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (qualite.contains("A")) return QColor(200, 230, 255); // Blue
                if (qualite.contains("B")) return QColor(255, 255, 200); // Yellow
                if (qualite.contains("C")) return QColor(255, 200, 230); // Pink
            }
        }

        // Color coding for Status (Column 7)
        if (index.column() == 7) {
            QString statut = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
            if (role == Qt::BackgroundRole) {
                if (statut == "Actif") return QColor(200, 255, 200);   // Green
                if (statut == "Inactif") return QColor(255, 200, 200); // Red
            }
            if (role == Qt::ForegroundRole) {
                if (statut == "Actif") return QColor(0, 100, 0);
                if (statut == "Inactif") return QColor(139, 0, 0);
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

namespace Ui {
class fournisseurs;
}

class fournisseurs : public QDialog
{
    Q_OBJECT

public:
    explicit fournisseurs(QWidget *parent = nullptr);
    ~fournisseurs();

private slots:
    void on_pushButton_16_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_15_clicked();
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
    void setupFournisseursTable();
    void loadFournisseurs();

private:
    Ui::fournisseurs *ui;
    CustomFournisseurModel *model;
    void addFournisseurToTable(const QString &id, const QString &nom,
                               const QString &typeMatiere, const QString &telephone,
                               const QString &adresse, const QString &delaiLivraison,
                               const QString &qualite, const QString &statut);
    void updateFournisseurInTable(int row, const QString &nom,
                                  const QString &typeMatiere, const QString &telephone,
                                  const QString &adresse, const QString &delaiLivraison,
                                  const QString &qualite, const QString &statut);
    void updateRowColors(int row);
    void setupSearch();
};

#endif // FOURNISSEURS_H
