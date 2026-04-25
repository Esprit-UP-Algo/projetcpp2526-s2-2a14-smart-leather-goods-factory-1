#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#if __has_include(<QtSerialPort/QSerialPort>)
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#else
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);

    bool connectArduino(const QString &portName);
    void sendMessage(const QString &message);
    QString readMessage();

    QSerialPort* getSerial();

private:
    QSerialPort *serial;
};

#endif // ARDUINO_H
