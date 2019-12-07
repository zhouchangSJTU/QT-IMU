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
//    str_in.data();
    memmove(chrTemp,str_in.data(),str_length);
//    for(int j=0;j<=str_length-1;j++){
//        chrTemp[j]=str_in.data()[j];
//    }
    usRxLength += str_length;
    while (usRxLength >= 11)
    {
        if (chrTemp[0] != 0x55)
        {
            usRxLength--;
            memmove(&chrTemp[0],&chrTemp[1],usRxLength);
//            for(int j=0;j<usRxLength;j++){
//                chrTemp[j]=chrTemp[j+1];
//            }
            continue;
        }
        unsigned char temp[8];
        ////////////////////////////////////////////////////////////////////
        switch(chrTemp[1])
        {
            case 0x50:	memmove(&stcTime,&chrTemp[2],8);
            break;
            case 0x51:
            {
                memmove(&temp,&chrTemp[2],8);
//                for(int j=0;j<=7;j++){
//                    temp[j]=chrTemp[j+1];
//                }
                short temp1=(short)temp[1];
                short temp2=(short)temp[3];
                short temp3=(short)temp[5];
                short ax = ((temp1<<8)|temp[0])/32768*16*9.8;
                short ay = ((temp2<<8)|temp[2])/32768*16*9.8;
                short az = ((temp3<<8)|temp[4])/32768*16*9.8;
                short tempreture = (((short)temp[7]<<8)|temp[6])/100;
                stcAcc.a[0]=ax;
                stcAcc.a[1]=ay;
                stcAcc.a[2]=az;
                stcAcc.T=tempreture;
                qDebug()<<"---------";
                for(int j=0;j<=7;j++){
                qDebug("test:%x",temp[j]);
                }
                qDebug()<<ax;
                qDebug()<<ay;
                qDebug()<<az;
                qDebug()<<"---------";
                break;
            }
            case 0x52:
            {
                memmove(&temp,&chrTemp[2],8);
                short wx = (((short)temp[1]<<8)|temp[0])/32768*2000;
                short wy = (((short)temp[3]<<8)|temp[2])/32768*2000;
                short wz = (((short)temp[5]<<8)|temp[4])/32768*2000;
                short tempreture = (((short)temp[7]<<8)|temp[6])/100;
                stcGyro.w[0]=wx;
                stcGyro.w[1]=wy;
                stcGyro.w[2]=wz;
                stcGyro.T=tempreture;
                break;
            }
            case 0x53:
            {
                memmove(&temp,&chrTemp[2],8);
                short wx = (((short)temp[1]<<8)|temp[0])/32768*2000;
                short wy = (((short)temp[3]<<8)|temp[2])/32768*2000;
                short wz = (((short)temp[5]<<8)|temp[4])/32768*2000;
                short tempreture = (((short)temp[7]<<8)|temp[6])/100;
                stcAngle.Angle[0]=wx;
                stcAngle.Angle[1]=wy;
                stcAngle.Angle[2]=wz;
                stcAngle.T=tempreture;
                break;
            }
            case 0x54:	memmove(&stcMag,&chrTemp[2],8);
            break;
            case 0x55:	memmove(&stcDStatus,&chrTemp[2],8);
            break;
            case 0x56:	memmove(&stcPress,&chrTemp[2],8);
            break;
            case 0x57:	memmove(&stcLonLat,&chrTemp[2],8);
            break;
            case 0x58:	memmove(&stcGPSV,&chrTemp[2],8);
            break;
        }
        ////////////////////////////////////////////////////////////////////
//        switch(chrTemp[1])
//        {
//            case 0x50:	memmove(&stcTime,&chrTemp[2],8);break;
//            case 0x51:	memmove(&stcAcc,&chrTemp[2],8);break;
//            case 0x52:	memmove(&stcGyro,&chrTemp[2],8);break;
//            case 0x53:	memmove(&stcAngle,&chrTemp[2],8);break;
//            case 0x54:	memmove(&stcMag,&chrTemp[2],8);break;
//            case 0x55:	memmove(&stcDStatus,&chrTemp[2],8);break;
//            case 0x56:	memmove(&stcPress,&chrTemp[2],8);break;
//            case 0x57:	memmove(&stcLonLat,&chrTemp[2],8);break;
//            case 0x58:	memmove(&stcGPSV,&chrTemp[2],8);break;
//        }
        usRxLength -= 11;
        memmove(&chrTemp[0],&chrTemp[11],usRxLength);
//        for(int j=0;j<usRxLength;j++){
//            chrTemp[j]=chrTemp[11+j];
//        }
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

