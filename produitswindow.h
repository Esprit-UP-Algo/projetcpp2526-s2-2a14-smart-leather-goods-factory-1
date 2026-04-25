#ifndef PRODUITSWINDOW_H
#define PRODUITSWINDOW_H

#include <QMainWindow>
#include "produit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class produitswindow;
}
QT_END_NAMESPACE

class produitswindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit produitswindow(int idEmployeConnecte, QWidget *parent = nullptr);
    ~produitswindow();

private slots:
    void on_pushButton_clicked();   // Ajouter
    void on_pushButton_2_clicked(); // Modifier
    void on_pushButton_3_clicked(); // Supprimer
    void on_pushButton_4_clicked(); // Actualiser
    void on_pushButton_7_clicked(); // Exporter PDF
    void on_pushButton_9_clicked(); // Statistiques
    void on_pushButton_10_clicked(); // Recherche
    void on_comboBox_tri_currentIndexChanged(int index); // Tri

    // Navigation dynamique
    void navToEmploye();
    void navToCommandes();
    void navToMachines();
    void navToMatieres();
    void navToFournisseurs();
    void navToLogout();

private:
    Ui::produitswindow *ui;
    Produit P;
    int m_idEmployeConnecte;
    
    void setupNavigationMenu();
};

#endif // PRODUITSWINDOW_H
