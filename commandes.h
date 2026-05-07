#ifndef COMMANDES_H
#define COMMANDES_H

#include <QMainWindow>
#include <memory>

class TwilioSms;

QT_BEGIN_NAMESPACE
namespace Ui { class commandes; }
QT_END_NAMESPACE

class commandes : public QMainWindow
{
    Q_OBJECT

public:
    explicit commandes(int idEmployeConnecte = -1, QWidget *parent = nullptr);
    ~commandes() override;

private slots:
    void on_pushButton_clicked();     // Ajouter commande
    void on_pushButton_2_clicked();   // Modifier commande
    void on_pushButton_3_clicked();   // Supprimer commande
    void on_pushButton_4_clicked();   // Actualiser liste
    void on_pushButton_7_clicked();   // Exporter PDF
    void on_pushButton_qr_clicked();  // Générer QR Code paiement
    void on_pushButton_9_clicked();   // Statistiques
    void on_pushButton_10_clicked();  // Recherche par référence

private:
    Ui::commandes *ui;
    int m_idEmployeConnecte;
    std::unique_ptr<TwilioSms> m_twilio;

    void loadAllCommandes();
    void filterByReference(const QString &query);
    void applyFilters();
};

#endif // COMMANDES_H
