#include "login.h"
#include "connection.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   // لازم قبل أي QMessageBox

    if (!Connection::instance()->createConnect()) {
        QMessageBox::critical(nullptr, "Connexion échouée",
            "Impossible de se connecter à la base Oracle.\n"
            "Vérifiez que le service Oracle XE est démarré.");
        return 1;
    }

    login w;
    w.show();
    return a.exec();
}
