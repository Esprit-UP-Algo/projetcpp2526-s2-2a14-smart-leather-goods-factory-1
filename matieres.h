#ifndef MATIERES_H
#define MATIERES_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDateTime>

namespace Ui {
class Matieres;
}

class Matieres : public QDialog
{
    Q_OBJECT

public:
    explicit Matieres(int idEmploye, QWidget *parent = nullptr);
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
    void setupMatieresTable();
    void loadMatieres();
    void setupSearch();
    void filterTable();
    void updateRowColors(int row);
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();

private:
    Ui::Matieres *ui;
    int m_idEmploye;

    void addMatiereToTable(const QString &id, const QString &type,
                           const QString &couleur, const QString &qualite,
                           const QString &prix, const QString &stock,
                           const QString &fournisseur);

    void updateMatiereInTable(int row, const QString &type,
                              const QString &couleur, const QString &qualite,
                              const QString &prix, const QString &stock,
                              const QString &fournisseur);
};

#endif // MATIERES_H
