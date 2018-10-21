#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QNetworkDatagram>
#include <algorithm>
#include <QtGlobal>
#include "driveDatagram.h"
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

    id.deviceID=0x5;//Uniq id for
    id.structID=0x5;
    //id.structID=0x5;
    lynxController.init(driveOperation,id);
    /*
    lynxController.init(MY_ID,"my drive");
    lynxController.add(driveOperation,id);
    lynxController.add(driveParams,id);
    lynxController.add(driveControlParams,id);
    */
    pTimer->start(500);

    updateButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pSocket;
    delete pTimer;
}
void setButtonGreen(QPushButton *button, bool state)
{
    if(state)
    {
        button->setStyleSheet("background-color: lightgreen");
    }
    else
    {
        button->setStyleSheet("");
    }
}
void MainWindow::updateButtons(void)
{
    setButtonGreen(ui->buttonEnableDrive,drive.getStatusBit(eEnabled));
    setButtonGreen(ui->buttonOn1,drive.getStatusBit(eRunning));
    setButtonGreen(ui->buttonOff2,drive.getStatusBit(eCoastDownEnabled));
    setButtonGreen(ui->buttonOff3,drive.getStatusBit(eQuickStopEnabled));
    ui->buttonOn1->setEnabled(drive.getStatusBit(eEnabled));
    ui->buttonOff2->setEnabled(drive.getStatusBit(eEnabled));
    ui->buttonOff3->setEnabled(drive.getStatusBit(eEnabled));
    ui->buttonEnableDrive->setEnabled(!drive.getStatusBit(eRunning));
}

void MainWindow::readyReadUDP()
{
    QNetworkDatagram datagram = pSocket->receiveDatagram();
    ui->labelIpAddress->setText(datagram.senderAddress().toString());
    lynxController.fromBuffer(datagram.data());
    // Map booleans
    drive.statusWord=lynxController.getData<uint16_t>(eDriveControl);
    // Update buttons
    updateButtons();
    /*
    myChart.newCurrent=lynxController.getData<int16_t>(joy_LX);
    myChart.newTorque=lynxController.getData<int16_t>(joy_LY);
    myChart.newSpeed=lynxController.getData<int16_t>(joy_RX);
    myChart.newPosition=lynxController.getData<int16_t>(joy_RY);
    */

}


void MainWindow::timerTick()
{
    myChart.refreshChart();
    drive.operation();

    lynxController.setData<uint16_t>(eDriveControl,drive.statusWord);
    QHostAddress targetAddress = QHostAddress("192.168.20.14");
    int length = lynxController.toBuffer(dataBufferOut);
    pSocket->writeDatagram(dataBufferOut, length, targetAddress, 11000);
    drive.setControlBit(eReadFromFlash,false);
    drive.setControlBit(eWriteToFlash,false);
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

        //int length = lynxController.sendWHOIS(dataBufferOut);
        //pSocket->writeDatagram(dataBufferOut, length, QHostAddress::Any, 11000);

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
    }
}

void MainWindow::on_buttonEnableDrive_toggled(bool checked)
{
    drive.setControlBit(eEnableDrive,checked);
}

void MainWindow::on_buttonOn1_toggled(bool checked)
{
    drive.setControlBit(eONOFF1,checked);
}

void MainWindow::on_buttonOff2_toggled(bool checked)
{
    drive.setControlBit(eOFF2,checked);
}

void MainWindow::on_buttonOff3_toggled(bool checked)
{
    drive.setControlBit(eOFF3,checked);
}

void MainWindow::on_buttonSaveFlash_clicked()
{
    drive.setControlBit(eWriteToFlash,true);
}

void MainWindow::on_buttonReadFlash_clicked()
{
    drive.setControlBit(eReadFromFlash,true);
}

void MainWindow::on_labelNumerofPoles_ACT_editingFinished()
{

    float var = ui->labelNumerofPoles_ACT->text().toFloat();
     ui->labelNumerofPoles_STA->setText(QString::number(var));
}

void MainWindow::on_labelResistance_ACT_editingFinished()
{
    float var = ui->labelResistance_ACT->text().toFloat();
}

void MainWindow::on_labelLsq_ACT_editingFinished()
{
    float var = ui->labelLsq_ACT->text().toFloat();
}

void MainWindow::on_labelLsd_ACT_editingFinished()
{
    float var = ui->labelLsd_ACT->text().toFloat();
}

void MainWindow::on_labelEMF_ACT_editingFinished()
{
    float var = ui->labelEMF_ACT->text().toFloat();
}
