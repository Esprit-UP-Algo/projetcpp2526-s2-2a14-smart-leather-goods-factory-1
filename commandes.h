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
    explicit commandes(QWidget *parent = nullptr);
    ~commandes();

private slots:
    // CRUD operations
    void on_pushButton_clicked();      // Ajouter
    void on_pushButton_2_clicked();    // Modifier
    void on_pushButton_3_clicked();    // Supprimer
    void on_pushButton_4_clicked();    // Actualiser

    // Navigation
    void on_pushButton_5_clicked();     // Login
    void on_pushButton_11_clicked();    // Employee
    void on_pushButton_12_clicked();    // Fournisseurs
    void on_pushButton_19_clicked();    // Employee (duplicate)
    void on_pushButton_20_clicked();    // Fournisseurs (duplicate)
    void on_pushButton_21_clicked();    // Products
    void on_pushButton_22_clicked();    // Matieres
    void on_pushButton_23_clicked();    // Machine
    void on_pushButton_17_clicked();    // Quitter/Login

    // Features
    void on_pushButton_7_clicked();     // Exporter PDF
    void on_pushButton_9_clicked();     // Statistiques
    void on_pushButton_10_clicked();    // Rechercher

    // Search
    void on_lineEdit_5_textChanged(const QString &text);
    void on_lineEdit_6_textChanged(const QString &text);

private:
    Ui::commandes *ui;

    // Helper functions
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
