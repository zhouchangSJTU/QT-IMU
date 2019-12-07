#include "mainwindow.h"
#include "ui_mainwindow.h"
#define debugMode 0

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new IMUSerial();
    imuThuread = new QThread(this);
    serial->moveToThread(imuThuread);
    connect(ui->recieve,SIGNAL(clicked()), serial, SLOT(onReicieve()));
    connect(ui->stopButton,SIGNAL(clicked()), serial, SLOT(onStop()));
    connect(serial,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(serial,SIGNAL(send(QByteArray)),this,SLOT(onGetIMU(QByteArray)));
    imuThuread->start();
//    connect(&btn,SIGNAL(clicked()),this,SLOT(sendSerialData()));
}

MainWindow::~MainWindow()
{
    imuThuread->quit();
    delete ui;
    delete serial;
    delete imuThuread;
}

void MainWindow::on_detectSerial_clicked()
{
    qDebug()<<"debug info";
    QList<QString> list = detectSerial();
    ui->comboBox->clear();
    if(list.size()!=0){
        QList<QString>::iterator iter = list.begin();
        for(;iter!=list.end();iter++){
            ui->textBrowser->append(*iter);
            ui->comboBox->addItem(*iter);
        }
    }
    else{
        ui->textBrowser->append("no serial device detect!");
    }
}

void MainWindow::on_link_clicked(){
    serial->openPort(ui->comboBox->currentText());
        qDebug()<<ui->comboBox->currentText();
    ui->textBrowser->append("select: " + ui->comboBox->currentText());
}

void MainWindow::on_recieve_clicked(){
    qDebug()<<"debug recieve";
}

void MainWindow::on_stopButton_clicked(){
    qDebug()<<"debug stop";
}

QList<QString> MainWindow::detectSerial(){
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    QList<QString> stringList;
    QList<QSerialPortInfo>::Iterator iter = serialList.begin();
    for(;iter!=serialList.end();iter++){
//        stringList.push_back(iter->description());
        stringList.push_back(iter->portName());
        qDebug()<<"serial::";
        qDebug()<<iter->portName();
    }
#if debugMode
    qDebug()<<"serial check end";
    qDebug()<<stringList.size();
#endif
    return stringList;
}

void MainWindow::openPort(QString portName){
    serial->openPort(portName);
}

void MainWindow::onReadyRead(){
#if debugMode
    qDebug()<<"info come!";
#endif
    QString str(serial->serial->readAll());
    ui->textBrowser->append(str);
}

void MainWindow::onGetIMU(QByteArray data){
//    ui->textBrowser->append(data);
    QString dataStr = QString(data);
    imu.CopeSerialData(dataStr.toStdString(), 11);
    unsigned char year = imu.stcTime.ucYear;
    unsigned char mounth = imu.stcTime.ucMonth;
    short acc0 = imu.stcAcc.a[0];
    short acc1 = imu.stcAcc.a[1];
    short acc2 = imu.stcAcc.a[2];
    auto T = imu.stcAcc.T;
    auto angle0 = imu.stcAngle.Angle[0];
    auto angle1 = imu.stcAngle.Angle[1];
    auto angle2 = imu.stcAngle.Angle[2];
    ui->textBrowser->append("year:"+QString(year));
    ui->textBrowser->append("mounth:"+QString(mounth));
    ui->textBrowser->append(("acc0"+QString(acc0)));
    ui->textBrowser->append(("acc1"+QString(acc1)));
    ui->textBrowser->append(("acc2"+QString(acc2)));
//    qDebug()<<"=============this is the data debug===========";
//    qDebug()<<year;
//    qDebug()<<mounth;
//    qDebug()<<acc0;
//    qDebug()<<acc1;
//    qDebug()<<acc2;
//    qDebug()<<T;
//    qDebug()<<angle0;
//    qDebug()<<angle1;
//    qDebug()<<angle2;
#if debugMode
    qDebug()<<"get imu!";
#endif
}

void MainWindow::readData(){
    while(true){
        QByteArray array = serial->serial->read(1000);
        QString dataStr(array);
        ui->textBrowser->append(dataStr);
    }
}
