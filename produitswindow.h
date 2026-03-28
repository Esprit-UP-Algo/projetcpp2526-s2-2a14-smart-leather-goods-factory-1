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
    produitswindow(QWidget *parent = nullptr);
    ~produitswindow();

private slots:
    void on_pushButton_clicked();   // Ajouter
    void on_pushButton_2_clicked(); // Modifier
    void on_pushButton_3_clicked(); // Supprimer
    void on_pushButton_4_clicked(); // Actualiser
    void on_pushButton_7_clicked(); // Exporter PDF
    void on_pushButton_10_clicked(); // Recherche
    void on_comboBox_tri_currentIndexChanged(int index); // Tri

private:
    Ui::produitswindow *ui;
    Produit P;
};

#endif // PRODUITSWINDOW_H
