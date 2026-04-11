#include "login.h"
#include "connection.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   // لازم قبل أي QMessageBox

    a.setStyleSheet(
        "QMessageBox {"
        "  background-color: #f7efe7;"
        "  color: #2c1f15;"
        "}"
        "QMessageBox QLabel {"
        "  color: #2c1f15;"
        "}"
        "QMessageBox QPushButton {"
        "  background-color: #d8b39d;"
        "  color: black;"
        "  border-radius: 8px;"
        "}"
        "QMessageBox QPushButton:hover {"
        "  background-color: #e2c6b1;"
        "}"
        );

    a.setStyleSheet(
        "QProgressDialog { background-color: #f7efe7; color: #2c1f15; }"
        "QProgressDialog QLabel { color: #2c1f15; }"
        "QProgressDialog QPushButton { background-color: #d8b39d; color: black; }"
        "QProgressBar { background-color: #efe4d8; border: 1px solid #c9b2a2; border-radius: 8px; }"
        "QProgressBar::chunk { background-color: #a07a5a; }"
        );

    Connection c;
    if(!c.createconnect()){
        QMessageBox::critical(nullptr, "DB", "Connexion échouée !");
        return 1;
    }

    login w;
    w.show();
    return a.exec();
}
