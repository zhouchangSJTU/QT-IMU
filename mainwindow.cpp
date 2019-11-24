#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_detectSerial_clicked()
{
    qDebug()<<"debug info";
    QList<QString> list = imu.getSerialList();
    if(list.size()!=0){
        QList<QString>::iterator iter = list.begin();
        for(;iter!=list.end();iter++){
            ui->textBrowser->append(*iter);
        }
        qDebug()<<"information: " + *iter;
    }
    else{
        ui->textBrowser->append("no serial device detect!");
    }
}
