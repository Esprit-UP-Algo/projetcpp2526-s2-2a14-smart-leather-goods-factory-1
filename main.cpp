#include "connexion.h"
#include "mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Segoe UI", 10));

    // --- Connexion à la base Oracle ---
    Connexion c;
    bool test = c.ouvrir();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Connexion"),
                                 QObject::tr("Connexion établie avec succès.\nCliquez sur OK pour continuer."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Connexion"),
                              QObject::tr("Échec de la connexion à la base de données.\nCliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return -1;  // Arrête le programme si la connexion échoue
    }

    // --- Chargement du style QSS ---
    QFile styleFile(":/style.qss"); // si le fichier est dans les ressources (.qrc)
    // QFile styleFile("style.qss"); // sinon, s'il est dans le même dossier que l'exécutable

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = styleFile.readAll();
        a.setStyleSheet(style);
        styleFile.close();
    } else {
        qDebug("Impossible de charger le fichier style.qss");
    }

    // --- Afficher la fenêtre principale ---
    MainWindow w;
    w.show();

    return a.exec();
}
