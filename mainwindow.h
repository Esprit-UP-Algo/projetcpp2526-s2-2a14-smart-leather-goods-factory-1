#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked(); // Bouton Ajouter
    void on_pushButton_4_clicked(); // Bouton Modifier
    void on_pushButton_5_clicked(); // Bouton Supprimer
    void on_pushButton_6_clicked(); // Bouton Actualiser
    void on_pushButton_7_clicked(); // Bouton Recherche
    void on_pushButton_8_clicked(); // Bouton Exporter PDF
    void on_pushButton_9_clicked(); // Bouton Statistiques
    void on_pushButton_maps_clicked(); // Bouton Maps
    void on_pushButton_recommandation_clicked(); // Bouton Recommandation intelligente
    void on_comboBox_currentIndexChanged(int index); // Tri

private:
    bool resoudreStructureFournisseurs();
    bool chargerFournisseurs(const QString &idFiltre = QString(), const QString &typeFiltre = QString());
    QString genererIdFournisseur();
    bool validerTelephone(const QString &telephone) const;
    void afficherErreurSql(const QString &operation, const QString &detail);

    Ui::MainWindow *ui;
    QString m_tableFournisseurs;
    bool m_hasAdvancedRecommendationFields = false;
};
#endif // MAINWINDOW_H
