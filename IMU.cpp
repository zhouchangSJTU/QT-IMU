#include "IMU.h"

#define Pi 3.14159265359

CJY901::CJY901(){
    serialIMU = new QSerialPort();
}

CJY901::~CJY901(){
    if(serialIMU->isOpen())
    {
        serialIMU->clear();
        serialIMU->close();
    }
    delete serialIMU;
}

// check serial port list
QList<QString> CJY901::getSerialList(){

    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    QList<QString> stringList;
    QList<QSerialPortInfo>::Iterator iter = serialList.begin();
    for(;iter!=serialList.end();iter++){
        stringList.push_back(iter->description());
        qDebug()<<"serial::";
        qDebug()<<iter->description();
    }
    qDebug()<<"serial check end";
    qDebug()<<stringList.size();
    return stringList;
}

void CJY901::openPort(QString portName){
    if(serialIMU->isOpen())
    {
        serialIMU->clear();
        serialIMU->close();
    }
    serialIMU->setPortName(portName);
    if(!serialIMU->open(QIODevice::ReadWrite))
    {
        qDebug()<<portName<<"打开失败!";
        qDebug()<<"serial state is :";
        qDebug()<<serialIMU->isOpen();
        return;
    }
    serialIMU->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
    qDebug()<<"success";
    qDebug()<<"serial state is :";
    qDebug()<<serialIMU->isOpen();
}

QString CJY901::recieveData(){
    if(!serialIMU->isOpen())
    {
        qDebug()<<"port is not open";
        return "error, port is not opem";
    }
    else
    {
        QByteArray data = serialIMU->readAll();
        QString dataStr = QString::fromStdString(data.toStdString());
        return dataStr;
    }
}
//convert serial data to jy901 data
void CJY901::CopeSerialData(std::string str_in, unsigned short usLength)
{
    unsigned int str_length = str_in.size();
    static unsigned char chrTemp[2000];
    static unsigned char ucRxCnt = 0;
    static unsigned int usRxLength = 0;

    memcpy(chrTemp,str_in.data(),str_length);
    usRxLength += str_length;
    while (usRxLength >= 11)
    {
        if (chrTemp[0] != 0x55)
        {
            usRxLength--;
            memcpy(&chrTemp[0],&chrTemp[1],usRxLength);
            continue;
        }
        switch(chrTemp[1])
        {
            case 0x50:	memcpy(&stcTime,&chrTemp[2],8);break;
            case 0x51:	memcpy(&stcAcc,&chrTemp[2],8);break;
            case 0x52:	memcpy(&stcGyro,&chrTemp[2],8);break;
            case 0x53:	memcpy(&stcAngle,&chrTemp[2],8);break;
            case 0x54:	memcpy(&stcMag,&chrTemp[2],8);break;
            case 0x55:	memcpy(&stcDStatus,&chrTemp[2],8);break;
            case 0x56:	memcpy(&stcPress,&chrTemp[2],8);break;
            case 0x57:	memcpy(&stcLonLat,&chrTemp[2],8);break;
            case 0x58:	memcpy(&stcGPSV,&chrTemp[2],8);break;
        }
        usRxLength -= 11;
        memcpy(&chrTemp[0],&chrTemp[11],usRxLength);
    }
}

//EulerToQuaternion, euler in rad
template<typename T>
vector<T> EulerToQuaternion(T roll, T pitch, T yaw)
{
    vector<T> q;
    double x, y, z, w;
    double a = roll/2.0;
    double b = pitch/2.0;
    double g = yaw/2.0;
    w = cos(a)*cos(b)*cos(g) + sin(a)*sin(b)*sin(g);
    x = sin(a)*cos(b)*cos(g) - cos(a)*sin(b)*sin(g);
    y = cos(a)*sin(b)*cos(g) + sin(a)*cos(b)*sin(g);
    z = cos(a)*cos(b)*sin(g) - sin(a)*sin(b)*cos(g);
    q.push_back(w);
    q.push_back(x);
    q.push_back(y);
    q.push_back(z);
    return q;
}

//QuaternionToEuler, euler in rad
template<typename T>
vector<T> QuaternionToEuler(vector<T> q)
{
    vector<T> e;
    T roll = atan2(2*(q[0]*q[1]+q[2]*q[3]),1-2*(q[1]*q[1]+q[2]*q[2]));
    T pitch = asin(2*(q[0]*q[2]-q[1]*q[3]));
    T yaw = atan2(2*(q[0]*q[3]+q[1]*q[2]),1-2*(q[2]*q[2]+q[3]*q[3]));
    e.push_back(roll);
    e.push_back(pitch);
    e.push_back(yaw);
    return e;
}

