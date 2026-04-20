#include "produit.h"
#include "connection.h"
#include <QSqlError>
#include <QDebug>

Produit::Produit()
{
    id_produit = "";
    reference = "";
    categorie = "";
    prix = 0.0;
    quantie_stock = 0;
    statut = "";
}

Produit::Produit(QString id, QString ref, QString cat, float p, int qte, QString stat, QDate dateC)
{
    this->id_produit = id;
    this->reference = ref;
    this->categorie = cat;
    this->prix = p;
    this->quantie_stock = qte;
    this->statut = stat;
    this->date_creation = dateC;
}

bool Produit::ajouter()
{
    QSqlQuery query(Connection::instance()->getDatabase());

    // Oracle ODBC ne convertit pas QDate automatiquement → utiliser TO_DATE()
    query.prepare("INSERT INTO SMARTLEATHER.PRODUITS (ID_PRODUIT, REFERENCE, CATEGORIE, PRIX, QUANTITE_STOCK, STATUT, DATE_CREATION) "
                  "VALUES (:id, :ref, :cat, :prix, :qte, :statut, TO_DATE(:date_creation, 'YYYY-MM-DD'))");

    query.bindValue(":id",            id_produit.toInt());  // NUMBER dans Oracle
    query.bindValue(":ref",           reference);
    query.bindValue(":cat",           categorie);
    query.bindValue(":prix",          (double)prix);
    query.bindValue(":qte",           quantie_stock);
    query.bindValue(":statut",        statut);
    query.bindValue(":date_creation", date_creation.toString("yyyy-MM-dd"));

    bool ok = query.exec();
    if (!ok)
        qDebug() << "[ajouter] Erreur SQL :" << query.lastError().text();
    return ok;
}

QSqlQueryModel * Produit::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();

    QSqlDatabase db = Connection::instance()->getDatabase();

    // Vérifier que la connexion est ouverte
    if (!db.isOpen()) {
        qDebug() << "[afficher] ERREUR : base de données non ouverte !";
        return model;
    }

    // Exécuter la requête et vérifier les erreurs
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM SMARTLEATHER.PRODUITS")) {
        qDebug() << "[afficher] Erreur SQL :" << query.lastError().text();
        return model;
    }

    qDebug() << "[afficher] Requête OK, nombre de lignes :" << query.size();

    model->setQuery(std::move(query));

    if (model->lastError().isValid()) {
        qDebug() << "[afficher] Erreur modèle :" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}

bool Produit::supprimer(QString id)
{
    QSqlQuery query(Connection::instance()->getDatabase());
    query.prepare("DELETE FROM SMARTLEATHER.PRODUITS WHERE ID_PRODUIT = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Produit::modifier()
{
    QSqlQuery query(Connection::instance()->getDatabase());
    query.prepare("UPDATE SMARTLEATHER.PRODUITS SET REFERENCE=:ref, CATEGORIE=:cat, PRIX=:prix, "
                  "QUANTITE_STOCK=:qte, STATUT=:statut, DATE_CREATION=:date_creation WHERE ID_PRODUIT=:id");

    query.bindValue(":id", id_produit);
    query.bindValue(":ref", reference);
    query.bindValue(":cat", categorie);
    query.bindValue(":prix", prix);
    query.bindValue(":qte", quantie_stock);
    query.bindValue(":statut", statut);
    query.bindValue(":date_creation", date_creation);

    return query.exec();
}

int Produit::getNextId()
{
    QSqlQuery query(Connection::instance()->getDatabase());
    query.exec("SELECT NVL(MAX(ID_PRODUIT),0)+1 FROM SMARTLEATHER.PRODUITS");
    if (query.next())
        return query.value(0).toInt();
    return 1;
}

QSqlQueryModel * Produit::rechercher(QString ref, QString prix)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = Connection::instance()->getDatabase();
    QSqlQuery query(db);

    QString sql = "SELECT * FROM SMARTLEATHER.PRODUITS WHERE 1=1";
    if (!ref.trimmed().isEmpty())
        sql += " AND UPPER(REFERENCE) LIKE UPPER(:ref)";
    if (!prix.trimmed().isEmpty())
        sql += " AND PRIX = :prix";

    query.prepare(sql);

    if (!ref.trimmed().isEmpty())
        query.bindValue(":ref", "%" + ref.trimmed() + "%");
    if (!prix.trimmed().isEmpty())
        query.bindValue(":prix", prix.trimmed().toDouble());

    query.exec();
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}

QSqlQueryModel * Produit::afficherTri(int critere)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = Connection::instance()->getDatabase();

    QString sql = "SELECT * FROM SMARTLEATHER.PRODUITS";
    switch (critere) {
        case 1: sql += " ORDER BY PRIX ASC"; break;
        case 2: sql += " ORDER BY PRIX DESC"; break;
        case 3: sql += " ORDER BY CATEGORIE ASC"; break;
        default: break; // Aucun tri
    }

    QSqlQuery query(db);
    query.exec(sql);
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Quantité Stock"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Création"));

    return model;
}
