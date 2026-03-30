#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("youssef");//inserer le nom de la source de données
    db.setUserName("smartleather");//inserer nom de l'utilisateur
    db.setPassword("smartleather");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;



    else
    {
        qDebug() << db.lastError().text();
        test = false;
    }

    return  test;
}
