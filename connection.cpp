#include "connection.h" // Force recompile
#include <QSqlError>
#include <QDebug>

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

    db.setDatabaseName("Driver={Oracle in XE};Dbq=XE;Uid=smartleather;Pwd=smartleather;");
    if (db.open()) {
        test = true;
        qDebug() << "Connexion à la base de données réussie";
    } else {
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

// Retourner la connexion active
QSqlDatabase Connection::getDatabase()
{
    return db;
}

// Destructeur privé
Connection::~Connection()
{
    closeConnection();
}
