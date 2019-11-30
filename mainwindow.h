#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "IMU.h"
#include<QList>
#include<QString>
#include<QDebug>
#include<QThread>
#include"IMUSerial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CJY901 imu;
    QThread *imuThuread;
//    QTimer *timer;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    IMUSerial *serial;
    QList<QString> detectSerial();
    void openPort(QString portName);
    void readData();

public slots:

    void on_detectSerial_clicked();
    void on_link_clicked();
    void on_recieve_clicked();
    void on_stopButton_clicked();
    void onReadyRead();
    void onGetIMU(QByteArray data);
};
#endif // MAINWINDOW_H
