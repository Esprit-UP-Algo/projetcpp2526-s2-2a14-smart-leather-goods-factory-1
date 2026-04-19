#include "connexion.h"

Connexion::Connexion()
{
    // Constructor — you could initialize settings here if needed
}

bool Connexion::ouvrir()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("leathergoods");  // Your ODBC source name
    db.setUserName("smartleather");              // Oracle username
    db.setPassword("smartleather");             // Oracle password

    if (!db.open()) {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
        return false;
    }

    qDebug() << "Connexion établie avec succès !";
    return true;
}

void Connexion::fermer()
{
    db.close();
}
