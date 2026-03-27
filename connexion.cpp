#include "connexion.h"

Connexion::Connexion()
{
}

namespace {
QString erreurConnexion(const QSqlDatabase &db)
{
    return db.lastError().text();
}
}

bool Connexion::ouvrir()
{
    if (db.isValid() && db.isOpen()) {
        db.close();
    }

    db = QSqlDatabase();
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    const QStringList pilotes = QSqlDatabase::drivers();
    qDebug() << "Pilotes Qt SQL disponibles:" << pilotes;

    if (pilotes.contains("QOCI")) {
        db = QSqlDatabase::addDatabase("QOCI");
        db.setHostName("localhost");
        db.setPort(1521);
        db.setDatabaseName("xe");
        db.setUserName("smartleather");
        db.setPassword("smartleather");

        if (db.open()) {
            qDebug() << "Connexion Oracle etablie via QOCI.";
            return true;
        }

        qDebug() << "Echec QOCI:" << erreurConnexion(db);
    }

    if (!pilotes.contains("QODBC")) {
        qDebug() << "QODBC introuvable. Installez le plugin Qt SQL ODBC ou Oracle Client.";
        return false;
    }

    db = QSqlDatabase::addDatabase("QODBC");
    db.setUserName("smartleather");
    db.setPassword("smartleather");

    const QStringList chainesConnexion = {
        "DRIVER={Oracle in XE};DBQ=XE;",
        "DRIVER={Oracle in XE};DBQ=xe;",
        "DRIVER={Oracle in OraClient11g_home1};DBQ=XE;",
        "DRIVER={Oracle in OraDB11g_home1};DBQ=XE;",
        "DRIVER={Oracle in OraClient12Home1};DBQ=XE;",
        "DRIVER={Oracle in OraClient19Home1};DBQ=XE;"
    };

    for (const QString &chaine : chainesConnexion) {
        db.setDatabaseName(chaine);
        if (db.open()) {
            qDebug() << "Connexion Oracle etablie via QODBC.";
            return true;
        }
    }

    qDebug() << "Erreur de connexion:" << erreurConnexion(db);
    qDebug() << "Conseil: verifiez le nom exact du pilote Oracle dans ODBC (odbcad32).";
    return false;
}

void Connexion::fermer()
{
    if (db.isOpen()) {
        db.close();
    }
}
