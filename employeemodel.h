#ifndef EMPLOYEEMODEL_H
#define EMPLOYEEMODEL_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class EmployeeModel {
public:
    EmployeeModel();
    
    struct EmployeeInfo {
        bool found;
        QString name;
        QString role;
    };
    
    // Récupère l'employé depuis la base de données via son UID RFID
    EmployeeInfo getEmployeeByRFID(const QString& rfid_uid);
};

#endif // EMPLOYEEMODEL_H
