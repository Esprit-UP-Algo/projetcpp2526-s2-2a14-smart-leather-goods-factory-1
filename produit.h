#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QMap>

class Produit
{
    QString id_produit;
    QString nom;
    QString ref;
    QString categorie;
    float prix;
    int quantite_stock;
    QString statut;
    QDate date_creation;

public:
    // Constructeurs
    Produit();
    Produit(QString id, QString n, QString r, QString cat, float p, int q, QString stat, QDate dateC);

    // Getters
    QString getID() const { return id_produit; }
    QString getNom() const { return nom; }
    QString getRef() const { return ref; }
    QString getCategorie() const { return categorie; }
    float getPrix() const { return prix; }
    int getQuantiteStock() const { return quantite_stock; }
    QString getStatut() const { return statut; }
    QDate getDateCreation() const { return date_creation; }

    // Setters
    void setID(QString id) { id_produit = id; }
    void setNom(QString n) { nom = n; }
    void setRef(QString r) { ref = r; }
    void setCategorie(QString c) { categorie = c; }
    void setPrix(float p) { prix = p; }
    void setQuantiteStock(int q) { quantite_stock = q; }
    void setStatut(QString s) { statut = s; }
    void setDateCreation(QDate d) { date_creation = d; }

    // Fonctionnalités CRUD de base
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(QString id);
    bool modifier();

    // Fonctionnalités avancées
    QSqlQueryModel * rechercher(QString nomOrRef, QString prix);
    QSqlQueryModel * afficherTri(int critere);
    QMap<QString, int> statistiquesParCategorie();
    QMap<QString, int> statistiquesParStatut();
};

#endif // PRODUIT_H
