#include "login.h"
#include "connection.h"
#include "arduino.h"

#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Arduino arduino;

static bool isShowingPopup = false;

// 🔍 chercher employé
void chercherEmployeParRFID(const QString &uid)
{
    if (isShowingPopup) return;
    isShowingPopup = true;

    QSqlQuery query;

    // Utilisation du schéma SMARTLEATHER
    query.prepare(
        "SELECT NOM, PRENOM, POSTE "
        "FROM SMARTLEATHER.EMPLOYE "
        "WHERE RFID_UID = :uid"
        );

    query.bindValue(":uid", uid);

    if (!query.exec()) {
        qDebug() << "❌ SQL Error:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Base de Données", 
            "Impossible de vérifier le badge RFID.\nErreur: " + query.lastError().text());
        isShowingPopup = false;
        return;
    }

    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    
    // Style Premium Leather
    msgBox.setStyleSheet(
        "QMessageBox {"
        "  background-color: #f7efe7;"
        "  border: 3px solid #b5835a;"
        "  border-radius: 15px;"
        "}"
        "QLabel {"
        "  color: #2c1f15;"
        "  font-family: 'Segoe UI';"
        "  font-size: 14px;"
        "  padding: 10px;"
        "}"
        "QPushButton {"
        "  background-color: #d8b39d;"
        "  color: #2c1f15;"
        "  border-radius: 8px;"
        "  padding: 8px 20px;"
        "  font-weight: bold;"
        "  min-width: 100px;"
        "  margin-bottom: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c89b7b;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #b5835a;"
        "}"
    );

    if (query.next()) {
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString poste = query.value("POSTE").toString();

        msgBox.setWindowTitle("Accès Autorisé");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(QString(
            "<div style='text-align: center;'>"
            "<h2 style='color: #6f8f3d;'>✅ ACCÈS AUTORISÉ</h2>"
            "<hr style='border: 1px solid #d8b39d;'>"
            "<p><b>Carte RFID :</b> %1</p>"
            "<p style='font-size: 16px;'><b>Bienvenue, %2 %3</b></p>"
            "<p><b>Poste :</b> %4</p>"
            "</div>"
        ).arg(uid).arg(prenom).arg(nom).arg(poste));
    }
    else {
        msgBox.setWindowTitle("Accès Refusé");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QString(
            "<div style='text-align: center;'>"
            "<h2 style='color: #c4573a;'>❌ ACCÈS REFUSÉ</h2>"
            "<hr style='border: 1px solid #d8b39d;'>"
            "<p><b>Carte RFID détectée :</b> %1</p>"
            "<p><b>Statut :</b> Carte inconnue</p>"
            "<p style='font-size: 12px; color: #7b6656;'>Veuillez contacter un administrateur.</p>"
            "</div>"
        ).arg(uid));
    }

    msgBox.exec();
    isShowingPopup = false;
}

// 📡 lecture RFID
void lireRFID()
{
    if (isShowingPopup) return;

    QString uid = arduino.readMessage().trimmed();

    if (uid.isEmpty())
        return;

    qDebug() << "Arduino UID reçu:" << uid;

    if (uid == "RFID_READY") {
        qDebug() << "✅ Arduino est prêt";
        return;
    }

    chercherEmployeParRFID(uid);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "🚀 APP STARTED";

    // 🎨 style
    a.setStyleSheet(
        "QMessageBox { background-color: #f7efe7; color: #2c1f15; }"
        "QMessageBox QPushButton { background-color: #d8b39d; }"
        );

    // 🛢 DB
    Connection *c = Connection::instance();

    if (!c->createConnect()) {
        QMessageBox::critical(nullptr, "DB", "Connexion échouée !");
        return 1;
    }

    // 🔌 Arduino
    qDebug() << "Trying Arduino COM7...";

    if (!arduino.connectArduino("COM7")) {
        qDebug() << "❌ Arduino not connected";
        QMessageBox::warning(nullptr, "Arduino", "La carte Arduino n'est pas connectée sur COM7.\nLes fonctionnalités RFID ne seront pas disponibles.");
    }

    // ⏱ Timer
    QTimer rfidTimer;
    QObject::connect(&rfidTimer, &QTimer::timeout, lireRFID);
    rfidTimer.start(300);

    // 🖥 UI
    login w;
    w.show();

    return a.exec();
}
