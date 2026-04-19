#ifndef COMMANDES_H
#define COMMANDES_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class commandes;
}
QT_END_NAMESPACE

class commandes : public QMainWindow
{
    Q_OBJECT

public:
    commandes(QWidget *parent = nullptr);
    ~commandes();

private slots:
    void on_pushButton_clicked(); // Déclaration de la fonction pour le bouton Ajouter

private slots:
    void on_pushButton_2_clicked(); // Déclaration de la fonction pour le bouton Ajouter

private slots:
    void on_pushButton_7_clicked(); // Exporter PDF
    void on_pushButton_10_clicked(); // Recherche par référence
    void on_pushButton_9_clicked(); // Statistiques
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();


private:
    Ui::commandes *ui;

    void loadAllCommandes();
    void filterByReference(const QString &query);
     void applyFilters();
};

#endif // COMMANDES_H
