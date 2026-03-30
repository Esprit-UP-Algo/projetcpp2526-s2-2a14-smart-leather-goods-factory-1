#ifndef PAGEEMPLOYEE_H
#define PAGEEMPLOYEE_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDate>

namespace Ui {
class pageemployee;
}

class pageemployee : public QDialog
{
    Q_OBJECT

public:
    explicit pageemployee(QWidget *parent = nullptr);
    ~pageemployee();
    void addEmployeeToTable(const QString &id, const QString &nom,
                           const QString &prenom, const QString &poste,
                           const QString &salaire, const QString &dateEmbauche,
                           const QString &statut);
private slots:
    void on_pushButton_6_clicked();
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
    void setupEmployeesTable();
    void loadEmployees();

    void on_PushButton_9_clicked();

private:
    Ui::pageemployee *ui;

    void updateEmployeeInTable(int row, const QString &nom,
                              const QString &prenom, const QString &poste,
                              const QString &salaire, const QString &dateEmbauche,
                              const QString &statut);
    void updateRowColors(int row);
    void setupSearch();
};

#endif // PAGEEMPLOYEE_H
