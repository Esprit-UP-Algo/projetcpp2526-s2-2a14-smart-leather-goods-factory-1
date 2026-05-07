#ifndef FOURNISSEURS_H
#define FOURNISSEURS_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QDate>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class fournisseurs;
}

class fournisseurs : public QMainWindow
{
    Q_OBJECT

public:
    explicit fournisseurs(int idEmploye, QWidget *parent = nullptr);
    ~fournisseurs();

private slots:
    void on_pushButton_16_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_23_clicked();
    
    // Core CRUD
    void on_pushButton_3_clicked(); // Ajouter
    void on_pushButton_4_clicked(); // Modifier
    void on_pushButton_5_clicked(); // Supprimer
    void on_pushButton_6_clicked(); // Actualiser
    
    // Advanced
    void on_pushButton_7_clicked(); // Recherche
    void on_pushButton_8_clicked(); // Export PDF
    void on_pushButton_9_clicked(); // Statistiques
    void on_pushButton_maps_clicked();
    void on_pushButton_recommandation_clicked();
    
    // Pagination
    void on_pushButton_10_clicked(); // Prev
    void on_pushButton_14_clicked(); // Next
    void on_pushButton_11_clicked(); // Page 1
    void on_pushButton_12_clicked(); // Page 2
    void on_pushButton_13_clicked(); // Page 3

    void filterTable();
    void setupFournisseursTable();
    void loadFournisseurs();
    void on_comboBox_currentIndexChanged(int index); // Sort

private:
    Ui::fournisseurs *ui;
    int m_idEmploye;
    
    // Pagination state
    int m_currentPage = 1;
    const int m_pageSize = 5; 
    
    void updateRowColors(int row);
    void setupSearch();
};

#endif // FOURNISSEURS_H
