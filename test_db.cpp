#include <QCoreApplication>
#include <QSqlDatabase>
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
    } else {
        std::cout << "FAIL: Connection error: " << db.lastError().text().toStdString() << std::endl;
    }
    
    return 0;
}
