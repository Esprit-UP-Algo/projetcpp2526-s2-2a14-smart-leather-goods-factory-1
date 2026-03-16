#ifndef PAGEEMPLOYEE_H
#define PAGEEMPLOYEE_H

#include <QDialog>
#include <QVariantList>

namespace Ui {
class pageemployee;
}

class pageemployee : public QDialog
{
    Q_OBJECT

public:
    explicit pageemployee(int idEmployeConnecte, QWidget *parent = nullptr);
    ~pageemployee();

private slots:
    // Navigation
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();

    // CRUD
    void on_pushButton_clicked();     // Ajouter
    void on_pushButton_2_clicked();   // Modifier
    void on_pushButton_3_clicked();   // Supprimer

    // Search / Refresh
    void on_search_clicked();
    void on_actualiser_clicked();
    void applyFilter();

    // Export / Stats / Chat / Logout
    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_4_clicked();   // Ouvrir chat
    void on_pushButton_8_clicked();   // Logout
    void on_btnSaveFacePhoto_clicked();

private:
    Ui::pageemployee *ui;
    int m_idEmployeConnecte;

    void setupTable();
    void loadEmployeesTable(const QString &whereClause = QString(),
                            const QVariantList &binds = QVariantList());
    int currentEmployeeId() const;

    QString getCinEmployeConnecte() const;

};

#endif // PAGEEMPLOYEE_H
