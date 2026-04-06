#include "faceid.h"

#include <QMessageBox>
#include <QDebug>
#include <QProcessEnvironment>

FaceID::FaceID(QObject *parent) : QObject(parent)
{
    process = new QProcess(this);

    connect(process,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,
            &FaceID::onFaceRecognitionFinished);

    connect(process,
            &QProcess::readyReadStandardOutput,
            this,
            &FaceID::onReadyReadStandardOutput);

    connect(process,
            &QProcess::readyReadStandardError,
            this,
            &FaceID::onReadyReadStandardError);
}

void FaceID::startFaceRecognition()
{
    if (process->state() != QProcess::NotRunning) {
        qDebug() << "Face ID process already running.";
        return;
    }

    QString program = "C:/Users/AZERTY/Desktop/qt/venv311/Scripts/python.exe";
    QString scriptPath = "C:/Users/AZERTY/Desktop/qt/facial_recognition.py";

    QStringList arguments;
    arguments << scriptPath;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // Nettoyer les variables Python polluées par Qt / MinGW
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");

    process->setProcessEnvironment(env);

    qDebug() << "Program =" << program;
    qDebug() << "Script path =" << scriptPath;
    qDebug() << "PYTHONHOME removed =" << !env.contains("PYTHONHOME");
    qDebug() << "PYTHONPATH removed =" << !env.contains("PYTHONPATH");

    process->start(program, arguments);

    if (!process->waitForStarted(3000)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de lancer le script Python.");
        qDebug() << "Erreur démarrage Python:" << process->errorString();
    }
}

void FaceID::onFaceRecognitionFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        qDebug() << "Processus terminé. Exit code =" << exitCode;
    } else {
        qDebug() << "Le processus de reconnaissance faciale a échoué.";
        QMessageBox::warning(nullptr, "Erreur", "Le processus de reconnaissance faciale a échoué.");
    }
}

void FaceID::onReadyReadStandardOutput()
{
    QString output = QString::fromUtf8(process->readAllStandardOutput()).trimmed();
    qDebug() << "Python output =" << output;

    if (output.isEmpty())
        return;

    if (output.contains("DB_FOLDER_NOT_FOUND")) {
        QMessageBox::warning(nullptr, "Erreur", "Dossier database_faces introuvable.");
        emit faceRejected();
        return;
    }

    if (output.contains("NO_DATABASE_FACES")) {
        QMessageBox::warning(nullptr, "Erreur", "Aucun visage valide dans la base.");
        emit faceRejected();
        return;
    }

    if (output.contains("CAMERA_ERROR")) {
        QMessageBox::warning(nullptr, "Erreur", "Caméra non disponible.");
        emit faceRejected();
        return;
    }

    if (output.contains("UNKNOWN")) {
        QMessageBox::information(nullptr, "Reconnaissance faciale", "Aucun visage reconnu.");
        emit faceRejected();
        return;
    }

    int pos = output.lastIndexOf("USER:");
    if (pos != -1) {
        QString cin = output.mid(pos + 5).trimmed();

        if (cin.isEmpty()) {
            emit faceRejected();
            return;
        }

        emit faceRecognized(cin);
        return;
    }
}

void FaceID::onReadyReadStandardError()
{
    QString err = QString::fromUtf8(process->readAllStandardError()).trimmed();
    if (!err.isEmpty()) {
        qDebug() << "Python stderr =" << err;
    }
}
