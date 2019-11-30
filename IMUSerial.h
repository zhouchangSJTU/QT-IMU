#ifndef IMUSERIAL_H
#define IMUSERIAL_H
#include<QObject>
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QThread>
#include<QDebug>
#include<QTimer>

class IMUSerial : public QObject{
Q_OBJECT
public:
    QSerialPort *serial;
    QTimer *timer;
    QList<QString> detectSerial();
    QString PortName;
    void openPort(QString portName);
    void run();
    IMUSerial();
    ~IMUSerial();
private:

signals:
    void send(QByteArray data);
    void readyRead();

public slots:
    void onReicieve();
    void readData();
    void onStop();

};

#endif // IMUSERIAL_H
