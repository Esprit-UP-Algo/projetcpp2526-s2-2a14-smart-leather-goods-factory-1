#include "login.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test = c.createconnect();

    if (test) {
        QMessageBox::information(nullptr, "Base de données", "Connexion à la base de données réussie !");
    } else {
        QMessageBox::critical(nullptr, "Base de données", "La connexion à la base de données a échoué.\nVérifiez votre configuration ODBC.");
    }

    login w;
    w.show();
    return a.exec();
}
