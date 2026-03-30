#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("youssef");
    db.setUserName("smartleather");
    db.setPassword("smartleather");
    
    if (db.open()) {
        std::cout << "SUCCESS: Connected to database!" << std::endl;
        QSqlQuery query("SELECT * FROM MACHINE WHERE ROWNUM = 1");
        if (query.exec()) {
            QSqlRecord rec = query.record();
            std::cout << "The MACHINE table has " << rec.count() << " columns:" << std::endl;
            for(int i = 0; i < rec.count(); ++i) {
                std::cout << " - " << rec.fieldName(i).toStdString() << std::endl;
            }
        } else {
            std::cout << "FAIL: Could not query MACHINE: " << query.lastError().text().toStdString() << std::endl;
        }
    } else {
        std::cout << "FAIL: Connection error: " << db.lastError().text().toStdString() << std::endl;
    }
    
    return 0;
}
