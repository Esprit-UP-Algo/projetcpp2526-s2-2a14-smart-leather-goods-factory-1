#ifndef ARDUINOCONTROLLER_H
#define ARDUINOCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "employeemodel.h"

class ArduinoController : public QObject {
    Q_OBJECT
public:
    explicit ArduinoController(QObject *parent = nullptr);
    ~ArduinoController();
    void connectToArduino();
    void closeConnection();

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
    EmployeeModel *dbModel;
    QString serialBuffer;
    void processRFID(const QString& uid);
};

#endif // ARDUINOCONTROLLER_H
