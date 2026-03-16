#ifndef FOURNISSEURS_H
#define FOURNISSEURS_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDate>

namespace Ui {
class fournisseurs;
}

class fournisseurs : public QDialog
{
    Q_OBJECT

public:
    explicit fournisseurs(int idEmploye, QWidget *parent = nullptr);
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
    int m_idEmploye;
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
