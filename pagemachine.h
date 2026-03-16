#ifndef PAGEMACHINE_H
#define PAGEMACHINE_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class pagemachine;
}

class pagemachine : public QDialog
{
    Q_OBJECT

public:
    explicit pagemachine(int idEmploye, QWidget *parent = nullptr);
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
    int m_idEmploye;

    void addMachineToTable(const QString &id, const QString &nom,
                           const QString &type, const QString &etat,
                           const QString &capacite, const QString &frequence,
                           const QString &niveauCharge);

    void updateMachineInTable(int row, const QString &nom,
                              const QString &type, const QString &etat,
                              const QString &capacite, const QString &frequence,
                              const QString &niveauCharge);

    void updateRowColors(int row);
    void setupSearch();
};

#endif // PAGEMACHINE_H
