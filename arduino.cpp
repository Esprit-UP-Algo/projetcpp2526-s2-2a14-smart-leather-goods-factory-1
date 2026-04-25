#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

bool Arduino::connectArduino(const QString &portName)
{
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connecté sur" << portName;
        return true;
    } else {
        qDebug() << "Erreur connexion Arduino :" << serial->errorString();
        return false;
    }
}

void Arduino::sendMessage(const QString &message)
{
    if (serial->isOpen()) {
        serial->write(message.toUtf8());
        serial->flush();
    }
}

QString Arduino::readMessage()
{
    if (serial->isOpen() && serial->canReadLine()) {
        return QString::fromUtf8(serial->readLine()).trimmed();
    }
    return "";
}

QSerialPort* Arduino::getSerial()
{
    return serial;
}
