#include"IMUSerial.h"
#define DebugMode 0

IMUSerial::IMUSerial(){
    serial = new QSerialPort(this);
    timer = new QTimer(this);
    connect(serial, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    connect(timer, SIGNAL(timeout()), this, SLOT(readData()));
}

IMUSerial::~IMUSerial(){
    delete serial;
}

QList<QString> IMUSerial::detectSerial(){
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    QList<QString> stringList;
    QList<QSerialPortInfo>::Iterator iter = serialList.begin();
    for(;iter!=serialList.end();iter++){
//        stringList.push_back(iter->description());
        stringList.push_back(iter->portName());
        qDebug()<<"serial::";
        qDebug()<<iter->portName();
    }
    qDebug()<<"serial check end";
    qDebug()<<stringList.size();
    return stringList;
}

void IMUSerial::openPort(QString portName){
    PortName = portName;
    qDebug()<<"now portName is "+ PortName;
    if(serial->isOpen())
    {
        serial->clear();
        serial->close();
    }
    serial->setPortName(portName);
    if(!serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<portName<<"打开失败!";
        qDebug()<<"serial state is :";
        qDebug()<<serial->isOpen();
        return;
    }
    serial->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
    qDebug()<<"success";
    qDebug()<<"serial state is :";
    qDebug()<<serial->isOpen();
}

void IMUSerial::readData(){
    if(!serial->isOpen()){
        openPort(PortName);
        if(!serial->isOpen()){
            qDebug()<<"IMU Serial readData error, serial is closed!";
            return;
        }
    }
    if(serial->waitForReadyRead(30)){
        QByteArray array;
        array = serial->read(144);
//        array = serial->readAll();
        qDebug()<<"qByteArray!============";
//        array.toHex();
//        qDebug()<<array.toHex();
        emit send(array);
    }
}

void IMUSerial::run(){
        QByteArray array = serial->read(128);
        QString dataStr(array);
//        emit send(dataStr);
}

void IMUSerial::onReicieve(){
    timer->start(200);
}

void IMUSerial::onStop(){
    timer->stop();
}
