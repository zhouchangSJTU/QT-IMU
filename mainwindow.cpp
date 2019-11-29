#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(serial,SIGNAL(send(QString)),this,SLOT(onGetIMU(QString)));
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
    qDebug()<<"debug link";
    serial->openPort(ui->comboBox->currentText());
        qDebug()<<ui->comboBox->currentText();
    ui->textBrowser->append("select: " + ui->comboBox->currentText());
//    openPort("COM8");
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
    qDebug()<<"serial check end";
    qDebug()<<stringList.size();
    return stringList;
}

void MainWindow::openPort(QString portName){
    serial->openPort(portName);
}

void MainWindow::onReadyRead(){
    qDebug()<<"info come!";
    QString str(serial->serial->readAll());
    ui->textBrowser->append(str);
}

void MainWindow::onGetIMU(QString data){
    ui->textBrowser->append(data);
    qDebug()<<"get imu!";
}

void MainWindow::readData(){
    while(true){
        QByteArray array = serial->serial->read(1000);
        QString dataStr(array);
        ui->textBrowser->append(dataStr);
    }
}
