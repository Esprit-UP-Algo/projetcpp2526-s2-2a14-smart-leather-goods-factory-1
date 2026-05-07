#include "employeemodel.h"
#include <QDebug>

EmployeeModel::EmployeeModel() {
    // Le constructeur suppose que la connexion à la base de données est déjà 
    // établie ailleurs dans l'application (par exemple dans connection.cpp).
    // Si ce n'est pas le cas, vous devrez ouvrir la connexion ici.
}

EmployeeModel::EmployeeInfo EmployeeModel::getEmployeeByRFID(const QString& rfid_uid) {
    EmployeeInfo info;
    info.found = false;

    QSqlQuery query;
    // Utilisation de la table EMPLOYE et des colonnes NOM, PRENOM, POSTE, UID_RFID
    query.prepare("SELECT NOM, PRENOM, POSTE FROM EMPLOYE WHERE UID_RFID = :uid");
    query.bindValue(":uid", rfid_uid);

    if (query.exec()) {
        if (query.next()) {
            info.found = true;
            // On concatène le NOM et le PRENOM pour l'affichage sur l'écran LCD
            info.name = query.value(0).toString() + " " + query.value(1).toString();
            info.role = query.value(2).toString(); // POSTE
        }
    } else {
        qDebug() << "Erreur SQL lors de la recherche RFID:" << query.lastError().text();
    }

    return info;
}
