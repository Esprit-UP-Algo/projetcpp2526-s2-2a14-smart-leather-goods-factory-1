#include "login.h"
#include "connection.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   // لازم قبل أي QMessageBox

    Connection c;
    if(!c.createconnect()){
        QMessageBox::critical(nullptr, "DB", "Connexion échouée !");
        return 1;
    }

    login w;
    w.show();
    return a.exec();
}
