



#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include<QMessageBox>
#include <QFile>
#include <QTextStream>
// Initialisation du pointeur d'instance
Connection* Connection::p_instance = nullptr;

// Constructeur privé
Connection::Connection()
{
    // Initialisation de la base de données
    db = QSqlDatabase::addDatabase("QODBC");
}

// Méthode statique pour obtenir l'instance unique
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

// Méthode pour établir la connexion
bool Connection::createConnect()
{
    bool test = false;

    db.setDatabaseName("youssef");//inserer le nom de la source de données
    db.setUserName("smartleather");//inserer nom de l'utilisateur
    db.setPassword("smartleather");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        test = true;
        qDebug() << "Connexion à la base de données réussie";
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de connexion"), db.lastError().text(), QMessageBox::Cancel);
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }

    return test;
}

// Fermer la connexion
void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Destructeur privé
Connection::~Connection()
{
    closeConnection();
}





