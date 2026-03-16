#ifndef COMMANDES_H
#define COMMANDES_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QDate>

namespace Ui {
class commandes;
}

class commandes : public QMainWindow
{
    Q_OBJECT

public:
    explicit commandes(int idEmploye, QWidget *parent = nullptr);
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
    int m_idEmploye;

    void setupTable();
    void loadSampleData();
    void filterTable(const QString &idFilter, const QString &dateFilter);
    void updateTableColors();
    void exportToPDF();
    void showStatistics();
    bool validateData(const QString &id, const QString &client, const QString &address,
                      const QString &montant);
};

#endif // COMMANDES_H
