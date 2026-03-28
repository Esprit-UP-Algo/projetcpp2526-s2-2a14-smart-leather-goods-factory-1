#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Produit
{
    QString id_produit;
    QString reference;
    QString categorie;
    float prix;
    int quantie_stock;
    QString statut;
    QDate date_creation;

public:
    // Constructeurs
    Produit();
    Produit(QString id, QString ref, QString cat, float p, int qte, QString stat, QDate dateC);

    // Getters
    QString getID() const { return id_produit; }
    QString getReference() const { return reference; }
    QString getCategorie() const { return categorie; }
    float getPrix() const { return prix; }
    int getQuantiteStock() const { return quantie_stock; }
    QString getStatut() const { return statut; }
    QDate getDateCreation() const { return date_creation; }

    // Setters
    void setID(QString id) { id_produit = id; }
    void setReference(QString ref) { reference = ref; }
    void setCategorie(QString c) { categorie = c; }
    void setPrix(float p) { prix = p; }
    void setQuantiteStock(int qte) { quantie_stock = qte; }
    void setStatut(QString s) { statut = s; }
    void setDateCreation(QDate d) { date_creation = d; }

    // Fonctionnalités CRUD de base
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(QString id);
    bool modifier();

    // Fonctionnalités avancées
    static int getNextId();
    QSqlQueryModel * rechercher(QString ref, QString prix);
    QSqlQueryModel * afficherTri(int critere);
};

#endif // PRODUIT_H
