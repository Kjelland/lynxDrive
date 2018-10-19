#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QNetworkDatagram>
#include <algorithm>
#include <QtGlobal>
#include "datagrams.h"
#include "controllerstruct.h"

using namespace LynxStructureSpace;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setChart(&myChart.chart);

    pSocket = new QUdpSocket(this);
    pSocket->bind(11000);
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));

    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(timerTick()));


    ui->comboBox->addItem("Search for devices...");
    LynxID id;
    id.deviceID=0x5;
    id.structID=0x5;


    lynxController.init(initParamsController,id);

    //lynx.getData(driveID, eDriveTorqueRef)
    //lynx.getData(motorID, eMotorResistance)

    //lynxDrive.setData<uint32_t>(eDriveSpeedRef, 0x01234567);
/*
    lynxDrive.setData<uint32_t>(eDriveTorqueRef, 0x89abcdef);

    qDebug()<<lynxDrive.getData<uint32_t>(eDriveSpeedRef);
    unsigned char buffer[64]={0};
    int writtenLength = lynxDrive.toBuffer((char*)buffer);
    qDebug()<<writtenLength;
    qDebug()<<"startbuffer";
    for (int i = 0; i < writtenLength; i++)
    {
        qDebug() <<QString::number((buffer[i]), 16); ;
    }
qDebug()<<"endbuffer";
    qDebug()<<buffer;
    int readLength = lynxDrive.fromBuffer((char*)buffer);
 */

    pTimer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete pSocket;
    delete pTimer;
}

void MainWindow::readyReadUDP()
{
    QNetworkDatagram datagram = pSocket->receiveDatagram();//receiveBuffer.data(), receiveBuffer.size(), &sender, &senderPort


    //qDebug()<<"UDP Recived:"<<datagram.senderAddress().toIPv4Address();
    QHostAddress ip = QHostAddress::Any;
    QString ipDestination;
    ipDestination=datagram.senderAddress().toString();

    ui->labelIpAddress->setText(ipDestination);

    qDebug()<<ipDestination;
    lynxController.fromBuffer(datagram.data());
    //qDebug()<<lynxController.getData<int16_t>(joy_LX);
    myChart.newCurrent=lynxController.getData<int16_t>(joy_LX);
    myChart.newTorque=lynxController.getData<int16_t>(joy_LY);
    myChart.newSpeed=lynxController.getData<int16_t>(joy_RX);
    myChart.newPosition=lynxController.getData<int16_t>(joy_RY);

}


void MainWindow::timerTick()
{
    myChart.refreshChart();

    //   socket->writeDatagram((char*)transmitBuffer,lynx.txPackage_length,QHostAddress(targetIpAddress) , 11000);
     /*
    if (socket->isValid()){
        transmitBuffer[0]=1;
        transmitBuffer[1]=2;
        transmitBuffer[2]=3;
        socket->writeDatagram((const char*)transmitBuffer, QHostAddress("10.0.0.90"), 11000);
            qDebug()<<"UDP Written"<<transmitBuffer;
        }
        else{
            qDebug()<<"UDP fail to write";
        }*/

}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if(arg1=="Slew drive - 10.0.0.5"){
        ui->labelIpAddress->setText("10.0.0.5");
        ui->labelDeciveID->setText("0xAB");
        ui->labelDeviceText->setText("Slew drive");
        ui->labelDeviceStatus->setText("Connected..");
        ui->buttonDisconnect->setEnabled(true);

    }
    if(arg1=="Winch drive - 10.0.0.99"){

    }
}

void MainWindow::on_comboBox_highlighted(const QString &arg1)
{
    if(arg1=="Search for devices..." && ui->comboBox->count()==1){
        //TODO: Add udp search for all ips. Populate the list. Create timer so that you have to wait 5-10s before next search.
        ui->comboBox->removeItem(0);
        ui->comboBox->addItem("Slew drive - 10.0.0.5");
        ui->comboBox->addItem("Winch drive - 10.0.0.99");
        ui->comboBox->addItem("Search for devices...");
        searchInProgress=true;

    }
}

void MainWindow::on_buttonDisconnect_clicked()
{
    ui->labelIpAddress->clear();
    ui->labelDeciveID->clear();
    ui->labelDeviceText->clear();
    ui->labelDeviceStatus->clear();
    ui->buttonDisconnect->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_checkBoxChartTorque_stateChanged(int arg1)
{
    if(arg1)
    {

        myChart.chart.addSeries(&myChart.seriesTorque);
        myChart.showTorqueChart=true;
        myChart.chart.createDefaultAxes();

    }
    else
    {
        myChart.showTorqueChart=false;
        myChart.chart.removeSeries(&myChart.seriesTorque);
        myChart.seriesTorque.clear();
    }

  //  ui->graphicsView->update();
}

void MainWindow::on_checkBoxChartSpeed_stateChanged(int arg1)
{
    if(arg1)
    {

        myChart.chart.addSeries(&myChart.seriesSpeed);
        myChart.showSpeedChart=true;
        myChart.chart.createDefaultAxes();
    }
    else
    {
        myChart.chart.removeSeries(&myChart.seriesSpeed);
        myChart.showSpeedChart=false;
        myChart.seriesSpeed.clear();

       // ui->graphicsView->update();

    }
}

void MainWindow::on_checkBoxChartCurrent_stateChanged(int arg1)
{
    if(arg1)
    {

        myChart.chart.addSeries(&myChart.seriesCurrent);
        myChart.showCurrentChart=true;
        myChart.chart.createDefaultAxes();
    }
    else
    {
        myChart.chart.removeSeries(&myChart.seriesCurrent);
        myChart.showCurrentChart=false;
        myChart.seriesCurrent.clear();

        //ui->graphicsView->update();

    }
}

void MainWindow::on_checkBoxChartPosition_stateChanged(int arg1)
{
    if(arg1)
    {

        myChart.chart.addSeries(&myChart.seriesPosition);
        myChart.showPositionChart=true;
        myChart.chart.createDefaultAxes();
    }
    else
    {
        myChart.chart.removeSeries(&myChart.seriesPosition);
        myChart.showPositionChart=false;
        myChart.seriesPosition.clear();

      //  ui->graphicsView->update();

    }
}
