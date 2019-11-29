#include"IMUSerial.h"

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
//    serial->setParity(QSerialPort::Parity())
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
    QByteArray array = serial->read(1000);
    QString dataStr(array);
    emit send(dataStr);
    qDebug()<<"IMU Serial readData success!";
}

void IMUSerial::run(){
        QByteArray array = serial->read(1000);
        QString dataStr(array);
        emit send(dataStr);
}

void IMUSerial::onReicieve(){
    timer->start(500);
}

void IMUSerial::onStop(){
    timer->stop();
    qDebug()<<"is stop!";
}
