#include "produit.h" // Force recompile
#include "connection.h"
#include <QSqlError>
#include <QDebug>

Produit::Produit()
{
    id_produit = "";
    nom = "";
    ref = "";
    categorie = "";
    prix = 0.0;
    quantite_stock = 0;
    statut = "";
}

Produit::Produit(QString id, QString n, QString r, QString cat, float p, int q, QString stat, QDate dateC)
{
    this->id_produit = id;
    this->nom = n;
    this->ref = r;
    this->categorie = cat;
    this->prix = p;
    this->quantite_stock = q;
    this->statut = stat;
    this->date_creation = dateC;
}

bool Produit::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO SMARTLEATHER.PRODUIT (NOM, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, DATE_CREATION, REF) "
                  "VALUES (:nom, :cat, :prix, :qty, :statut, TO_DATE(:date_creation, 'YYYY-MM-DD'), :ref)");

    query.bindValue(":nom",           nom);
    query.bindValue(":cat",           categorie);
    query.bindValue(":prix",          (double)prix);
    query.bindValue(":qty",           quantite_stock);
    query.bindValue(":statut",        statut);
    query.bindValue(":date_creation", date_creation.toString("yyyy-MM-dd"));
    query.bindValue(":ref",           ref);

    bool ok = query.exec();
    if (!ok)
        qDebug() << "[ajouter] Erreur SQL :" << query.lastError().text();
    return ok;
}

QSqlQueryModel * Produit::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = Connection::instance()->getDatabase();

    if (!db.isOpen()) {
        qDebug() << "[afficher] ERREUR : base de données non ouverte !";
        return model;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT ID_PRODUIT, NOM, REF, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM SMARTLEATHER.PRODUIT")) {
        qDebug() << "[afficher] Erreur SQL :" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Stock"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}

bool Produit::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SMARTLEATHER.PRODUIT WHERE ID_PRODUIT = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Produit::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE SMARTLEATHER.PRODUIT SET NOM=:nom, REF=:ref, CATEGORIE=:cat, PRIX=:prix, "
                  "QUANTITE_STOCK=:qty, STATUT=:statut, DATE_CREATION=:date_creation WHERE ID_PRODUIT=:id");

    query.bindValue(":id", id_produit);
    query.bindValue(":nom", nom);
    query.bindValue(":ref", ref);
    query.bindValue(":cat", categorie);
    query.bindValue(":prix", prix);
    query.bindValue(":qty", quantite_stock);
    query.bindValue(":statut", statut);
    query.bindValue(":date_creation", date_creation);

    return query.exec();
}

QSqlQueryModel * Produit::rechercher(QString nomOrRef, QString prix)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = Connection::instance()->getDatabase();
    QSqlQuery query(db);

    QString sql = "SELECT ID_PRODUIT, NOM, REF, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM SMARTLEATHER.PRODUIT WHERE 1=1";
    if (!nomOrRef.trimmed().isEmpty())
        sql += " AND (UPPER(REF) LIKE UPPER(:nom1) OR UPPER(NOM) LIKE UPPER(:nom2))";
    if (!prix.trimmed().isEmpty())
        sql += " AND PRIX = :prix";

    query.prepare(sql);

    if (!nomOrRef.trimmed().isEmpty()) {
        query.bindValue(":nom1", "%" + nomOrRef.trimmed() + "%");
        query.bindValue(":nom2", "%" + nomOrRef.trimmed() + "%");
    }
    if (!prix.trimmed().isEmpty())
        query.bindValue(":prix", prix.trimmed().toDouble());

    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Stock"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}

QSqlQueryModel * Produit::afficherTri(int critere)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = Connection::instance()->getDatabase();

    QString sql = "SELECT ID_PRODUIT, NOM, REF, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, TO_CHAR(DATE_CREATION, 'DD/MM/YYYY') FROM SMARTLEATHER.PRODUIT";
    switch (critere) {
        case 1: sql += " ORDER BY PRIX ASC"; break;
        case 2: sql += " ORDER BY PRIX DESC"; break;
        case 3: sql += " ORDER BY CATEGORIE ASC"; break;
        case 4: sql += " ORDER BY QUANTITE_STOCK ASC"; break;
        default: break; // Aucun tri
    }

    QSqlQuery query(db);
    query.exec(sql);
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Stock"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}

QMap<QString, int> Produit::statistiquesParCategorie()
{
    QMap<QString, int> stats;
    QSqlDatabase db = Connection::instance()->getDatabase();
    QSqlQuery query(db);

    if (query.exec("SELECT CATEGORIE, COUNT(*) FROM SMARTLEATHER.PRODUIT GROUP BY CATEGORIE")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    } else {
        qDebug() << "[statistiques] Erreur:" << query.lastError().text();
    }
    return stats;
}

QMap<QString, int> Produit::statistiquesParStatut()
{
    QMap<QString, int> stats;
    QSqlDatabase db = Connection::instance()->getDatabase();
    QSqlQuery query(db);

    if (query.exec("SELECT STATUT, COUNT(*) FROM SMARTLEATHER.PRODUIT GROUP BY STATUT")) {
        while (query.next()) {
            stats.insert(query.value(0).toString(), query.value(1).toInt());
        }
    } else {
        qDebug() << "[statistiquesParStatut] Erreur:" << query.lastError().text();
    }
    return stats;
}
