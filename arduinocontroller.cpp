#include "arduinocontroller.h"
#include <QDebug>
#include <QMessageBox>

ArduinoController::ArduinoController(QObject *parent) : QObject(parent) {
    serial = new QSerialPort(this);
    dbModel = new EmployeeModel();
    
    // Initialisation de la BDD (assurez-vous que la connexion principale de votre projet est déjà ouverte)
    // Si la base de données est gérée globalement (comme dans connection.cpp), 
    // dbModel utilisera la connexion par défaut de Qt.
    
    connect(serial, &QSerialPort::readyRead, this, &ArduinoController::readSerialData);
}

ArduinoController::~ArduinoController() {
    closeConnection();
    delete dbModel;
}

void ArduinoController::closeConnection() {
    if (serial->isOpen()) {
        serial->close();
    }
}

void ArduinoController::connectToArduino() {
    bool arduino_is_available = false;
    QString arduino_port_name;
    
    // Parcours des ports disponibles
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        // Remplacer 9025 par le Vendor ID de votre Arduino si différent
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == 9025 || serialPortInfo.vendorIdentifier() == 6790){
                arduino_is_available = true;
                arduino_port_name = serialPortInfo.portName();
                break;
            }
        }
    }

    if(arduino_is_available){
        serial->setPortName(arduino_port_name);
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        
        if(serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Connecté à l'Arduino sur le port" << arduino_port_name;
        } else {
            qDebug() << "Erreur de connexion série:" << serial->errorString();
        }
    } else {
        qDebug() << "Arduino introuvable. Vérifiez la connexion USB.";
    }
}

void ArduinoController::readSerialData() {
    QByteArray data = serial->readAll();
    serialBuffer += QString::fromStdString(data.toStdString());
    
    // L'Arduino doit envoyer l'UID avec un saut de ligne (Serial.println)
    if (serialBuffer.contains("\n") || serialBuffer.contains("\r")) {
        QString uid = serialBuffer.trimmed();
        serialBuffer.clear();
        
        if (!uid.isEmpty()) {
            qDebug() << "UID Reçu de l'Arduino:" << uid;
            processRFID(uid);
        }
    }
}

void ArduinoController::processRFID(const QString& uid) {
    if(!serial->isOpen()) return;

    if(uid == "RFID_READY") {
        qDebug() << "Arduino est prêt et initialisé.";
        return;
    }

    EmployeeModel::EmployeeInfo info = dbModel->getEmployeeByRFID(uid);

    QString response;
    if (info.found) {
        response = info.name + ";" + info.role + "\n";
        qDebug() << "Employé trouvé, envoi:" << response.trimmed();
    } else {
        response = "NOT_FOUND\n";
        qDebug() << "UID Inconnu:" << uid;
    }

    // Envoi à l'Arduino
    serial->write(response.toUtf8());
}
