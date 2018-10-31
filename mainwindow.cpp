#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QNetworkDatagram>
#include <algorithm>
#include <QtGlobal>
#include "driveDatagram.h"
#include "controllerstruct.h"

#define PORT 11000


using namespace LynxStructureSpace;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    lynx(LYNX_MACHINE_ID),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setChart(&myChart.chart);

    pSocket = new QUdpSocket(this);
    pSocket->bind(PORT);
    targetAddress = QHostAddress("10.0.0.90");
    connect(pSocket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));

    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(timerTick()));

    pXboxTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(xBoxTick()));

    ui->comboBox->addItem("Search for devices...");

    operation = lynx.addStructure(OPERATION_STRUCT, 0x1, driveOperation);
    parameters = lynx.addStructure(DRIVE_PARAM_STRUCT, 0x2, driveParams);
    controlParam = lynx.addStructure(CONTROL_PARAM_STRUCT, 0x3, driveControlParams);

    pTimer->start(100);
    pXboxTimer->start(100);
    updateButtons();

    ui->labelPositionSetpoint->setPalette(Qt::red);
    ui->labelTorqueSetpoint->setPalette(Qt::red);
    ui->labelSpeedSetpoint->setPalette(Qt::red);

    ui->labelPositionFeedback->setPalette(Qt::blue);
    ui->labelTorqueFeedback->setPalette(Qt::blue);
    ui->labelSpeedFeedback->setPalette(Qt::blue);
    drive.setBit(&drive.controlWord,eSpeedMode,true);
   }

MainWindow::~MainWindow()
{
    delete ui;
    delete pSocket;
    delete pTimer;
}
void setButtonColor(QPushButton *button, bool state,QString color)
{
    if(state)
    {
        button->setStyleSheet("background-color: "+color+";"
                              "border-style: solid;"
                              "border-color: black;"
                              "border-width: 2px;"
                              "border-radius: 10px;");
    }
    else
    {
        button->setStyleSheet("background-color: white;"
                              "border-style: solid;"
                              "border-color: gray;"
                              "border-width: 2px;"
                              "border-radius: 10px;");
    }
}
static double deadbandScaling(double input, double inMax, double inMin, double outMax, double outMin, double deadband)
{

    if (input > deadband)
    {
        return ((input - deadband)*(outMax / (inMax - deadband)));
    }
    else if (input < - deadband)
    {
        return ((input + deadband)*(outMin / (inMin + deadband)));
    }
    return 0;
}
void MainWindow::xBoxTick()
{
    DWORD dwResult;
    for(DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
         dwResult = XInputGetState(i, &controllerState);
        if(dwResult == ERROR_SUCCESS)
        {
            double temp=deadbandScaling(double(controllerState.Gamepad.sThumbLY),32767.0,-32768.0,1337.0,-1337.0,5000.0);
            ui->labelSpeedSetpoint->display(temp);
            drive.speedSetpoint=temp;
            temp=deadbandScaling(double(controllerState.Gamepad.sThumbRY),32767.0,-32768.0,2.5,-2.5,5000.0);
            ui->labelTorqueSetpoint->display(temp);
            drive.torqueSetpoint=temp;
            setButtonColor(ui->labelButtonA,controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A,"lightGreen");
            setButtonColor(ui->labelButtonB,controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B,"lightGreen");
            setButtonColor(ui->labelButtonX,controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X,"lightGreen");
            setButtonColor(ui->labelButtonY,controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y,"lightGreen");
            break;
        }
    }

}



void MainWindow::updateButtons(void)
{
    setButtonColor(ui->indicatorDisabled,!drive.getBit(&drive.statusWord,eEnabled),"lightGreen");
    setButtonColor(ui->indicatorReady,drive.getBit(&drive.statusWord,eEnabled) && !drive.getBit(&drive.statusWord,eRunning) && !drive.getBit(&drive.statusWord,eCoastDownEnabled),"lightGreen");
    setButtonColor(ui->indicatorRunning,drive.getBit(&drive.statusWord,eRunning),"lightGreen");
    setButtonColor(ui->indicatorStopping,drive.getBit(&drive.statusWord,eCoastDownEnabled)|drive.getBit(&drive.statusWord,eCoastDownEnabled),"lightGreen");
    setButtonColor(ui->indicatorSpeedMode,drive.getBit(&drive.statusWord,eSpeedModeEnabled),"lightGreen");
    setButtonColor(ui->indicatorTorqueMode,!drive.getBit(&drive.statusWord,eSpeedModeEnabled),"lightGreen");
    //qDebug()<<drive.getBit(&drive.statusWord,eSpeedModeEnabled);
    setButtonColor(ui->indicatorEncoderFault,drive.getBit(&drive.statusWord,eEncoderFault),"red");
    setButtonColor(ui->indicatorFlashFault,drive.getBit(&drive.statusWord,eFlashFault),"yellow");
    setButtonColor(ui->indicatorDriveFault,drive.getBit(&drive.statusWord,eDriveFault),"yellow");
    setButtonColor(ui->indicatorIdentifying,drive.getBit(&drive.statusWord,eIdenfifying),"yellow");
    setButtonColor(ui->indicatorParameterFault,drive.getBit(&drive.statusWord,eUserParamFault),"yellow");
    /*
    setButtonColor(ui->buttonOff2,drive.getBit(&drive.statusWord,eCoastDownEnabled));
    setButtonColor(ui->buttonOff3,drive.getBit(&drive.statusWord,eQuickStopEnabled));
*/
    ui->buttonOn1->setEnabled(drive.getBit(&drive.statusWord,eEnabled));
    if (drive.getBit(&drive.statusWord,eCoastDownEnabled))
    {
        ui->buttonOn1->setChecked(false);
        //ui->buttonEnableDrive->setChecked(false);
    }
    ui->buttonOn1->setText(ui->buttonOn1->isChecked() ? "Stop" : "Start");
    ui->buttonOff2->setText(ui->buttonOff2->isChecked() ? "Reset" : "STOP!");
    //ui->buttonOn1->setEnabled(drive.getBit(&drive.statusWord,eEnabled));
    if(!drive.getBit(&drive.statusWord,eEnabled))
    {
        ui->buttonOn1->setChecked(false);
        //ui->buttonOff2->setChecked(false);
    }
    //ui->buttonOff2->setEnabled(drive.getBit(&drive.statusWord,eEnabled));
    //ui->buttonEnableDrive->setEnabled(!drive.getBit(&drive.statusWord,eRunning));

}

void MainWindow::readyReadUDP()
{
    QNetworkDatagram datagram = pSocket->receiveDatagram();
    ui->labelIpAddress->setText(datagram.senderAddress().toString());
    lynx.fromBuffer(datagram.data());
    if(lynx.dataChanged(operation))
    {
            // Map new variables
            drive.statusWord=lynx.getData<uint16_t>(operation,eDriveWord);
            drive.speedFeedback=double(lynx.getData<int32_t>(operation,eDriveSpeed))*1000.0/Q24;
            drive.torqueFeedback=double(lynx.getData<int32_t>(operation,eDriveTorque))*1000.0/Q24;
            drive.positionFeedback=double(lynx.getData<int32_t>(operation,eDrivePosition))/Q16;

            ui->labelPositionFeedback->display(double(drive.positionFeedback));
            ui->labelSpeedFeedback->display(double(drive.speedFeedback));
            ui->labelTorqueFeedback->display(double(drive.torqueFeedback));
            //qDebug()<<drive.getBit(&drive.statusWord,eEncoderFault);
            // Update buttons
            updateButtons();
            myChart.refreshChart(&drive.positionFeedback,&drive.speedFeedback,&drive.torqueFeedback,&drive.speedSetpoint);
    }
    if(lynx.dataChanged(parameters))
    {
        ui->labelNumerofPoles_STA->     setText(QString::number(double(lynx.getData<float>(parameters,eMotorPoles))));
        ui->labelResistance_STA->       setText(QString::number(double(lynx.getData<float>(parameters,eMotorResistance))));
        ui->labelLsd_STA->              setText(QString::number(double(lynx.getData<float>(parameters,eMotorDInductance))));
        ui->labelLsq_STA->              setText(QString::number(double(lynx.getData<float>(parameters,eMotorQInductance))));
        ui->labelEMF_STA->              setText(QString::number(double(lynx.getData<float>(parameters,eMotorBackEmf))));
        ui->labelMaxAcceleration_STA->  setText(QString::number(double(lynx.getData<int32_t>(parameters,eMotorMaxAcceleration))*1000.0/Q24));
        ui->labelMaxSpeed_STA->         setText(QString::number(double()));
        ui->labelMaxCurrent_STA->       setText(QString::number(double(lynx.getData<float>(parameters,eMotorMaxCurrent))));
        ui->labelMaxVolt_STA->          setText(QString::number(double(lynx.getData<float>(parameters,eMotorMaxVoltage))));
        ui->labelPWM_STA->              setText(QString::number(double(lynx.getData<float>(parameters,eMotorPwmFrequency))));
        ui->labelDriveMaxCurrent_STA->  setText(QString::number(double(lynx.getData<float>(parameters,eInverterMaxCurrent))));
        ui->labelDriveMaxVoltage_STA->  setText(QString::number(double(lynx.getData<float>(parameters,eInverterMaxVoltage))));
        ui->labelEncResolution_STA->    setText(QString::number(double(lynx.getData<uint16_t>(parameters,eEncoderResolution))));
        ui->labelEncOffset_STA->        setText(QString::number(double(lynx.getData<uint32_t>(parameters,eEncoderOffset))));

    }
    if(lynx.dataChanged(controlParam))
    {
        ui->labelSpeedPGain_STA->setText(QString::number(double(lynx.getData<int32_t>(controlParam,eControlSpeedGainP))/Q24));
        ui->labelSpeedIGain_STA->setText(QString::number(double(lynx.getData<int32_t>(controlParam,eControlSpeedGainI))/Q24));
        ui->labelTorquePGain_STA->setText(QString::number(double(lynx.getData<int32_t>(controlParam,eControlCurrentGainP))/Q24));
        ui->labelTorqueIGain_STA->setText(QString::number(double(lynx.getData<int32_t>(controlParam,eControlCurrentGainI))/Q24));
     }
}


void MainWindow::timerTick()
{

    //drive.operation();


    lynx.setData<uint16_t>(operation,eDriveWord,drive.controlWord);
    lynx.setData<int32_t>(operation,eDriveTorque,int32_t(drive.torqueSetpoint*Q24));
    lynx.setData<int32_t>(operation,eDriveSpeed,int32_t(drive.speedSetpoint*Q24/1000.0));
    lynx.setData<int32_t>(operation,eDrivePosition,0);
   // qDebug()<<lynx.getData<int32_t>(operation,eDriveSpeed);
    int length = lynx.toBuffer(operation,dataBufferOut);

    pSocket->writeDatagram(dataBufferOut,length, targetAddress, PORT);
    //pSocket->writeDatagram(dataBufferOut, length, targetAddress, 11000);
    drive.setBit(&drive.controlWord,eReadFromFlash,false);
    drive.setBit(&drive.controlWord,eWriteToFlash,false);
    //drive.setBit(&drive.controlWord,eOFF2,false);

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

        myChart.chart.addSeries(&myChart.seriesSpeedFeedback);
        myChart.showSpeedChart=true;
        myChart.chart.createDefaultAxes();
    }
    else
    {
        myChart.chart.removeSeries(&myChart.seriesSpeedFeedback);
        myChart.showSpeedChart=false;
        myChart.seriesSpeedFeedback.clear();
    }
}

void MainWindow::on_checkBoxChartCurrent_stateChanged(int arg1)
{
    if(arg1)
    {

        myChart.chart.addSeries(&myChart.seriesSpeedSetpoint);
        myChart.showCurrentChart=true;
        myChart.chart.createDefaultAxes();
    }
    else
    {
        myChart.chart.removeSeries(&myChart.seriesSpeedSetpoint);
        myChart.showCurrentChart=false;
        myChart.seriesSpeedSetpoint.clear();
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
    drive.setBit(&drive.controlWord,eEnableDrive,checked);
}

void MainWindow::on_buttonOff2_toggled(bool checked)
{
    drive.setBit(&drive.controlWord,eOFF2,checked);
}

void MainWindow::on_buttonOn1_toggled(bool checked)
{
    drive.setBit(&drive.controlWord,eONOFF1,checked);
}


void MainWindow::on_buttonSaveFlash_clicked()
{
    drive.setBit(&drive.controlWord,eWriteToFlash,true);
}

void MainWindow::on_buttonReadFlash_clicked()
{
    drive.setBit(&drive.controlWord,eReadFromFlash,true);
}






void MainWindow::on_buttonDownloadMotorParam_clicked()
{



    lynx.setData<float>(parameters,eMotorPoles,ui->labelNumerofPoles_ACT->text().toFloat());
    lynx.setData<float>(parameters,eMotorResistance,ui->labelResistance_ACT->text().toFloat());
    lynx.setData<float>(parameters,eMotorDInductance,ui->labelLsd_ACT->text().toFloat());
    lynx.setData<float>(parameters,eMotorQInductance,ui->labelLsq_ACT->text().toFloat());
    lynx.setData<float>(parameters,eMotorBackEmf,ui->labelEMF_ACT->text().toFloat());
    lynx.setData<int32_t>(parameters,eMotorMaxAcceleration,int32_t(ui->labelMaxAcceleration_ACT->text().toDouble()/1000.0*Q24));
    //lynx.setData<float>(parameters,eMotorMaxSpeed,ui->labelMaxSpeed_ACT->text().toFloat());
    //lynx.setData<float>(parameters,eMotorMaxCurrent,ui->labelMaxCurrent_ACT->text().toFloat());
    //lynx.setData<float>(parameters,eMotorMaxCurrentOverload,ui->labelmax->text().toFloat());
    //lynx.setData<float>(parameters,eMotorMaxVoltage,ui->labelMaxVolt_ACT->text().toFloat());
    //lynx.setData<float>(parameters,eMotorPwmFrequency,ui->labelPWM_ACT->text().toFloat());
    //lynx.setData<float>(parameters,eInverterMaxCurrent,15);
    //lynx.setData<float>(parameters,eInverterMaxVoltage,30);
    //lynx.setData<uint32_t>(parameters,eEncoderResolution,ui->labelEncResolution_ACT->text().toULong());
    //lynx.setData<uint32_t>(parameters,eEncoderOffset,ui->labelEncOffset_ACT->text().toULong());

    //Send request
    drive.setBit(&drive.parameterControlWord,eWriteParamters,true);
    lynx.setData<uint16_t>(parameters,eMotorParameterControl,drive.parameterControlWord);

    int length = lynx.toBuffer(parameters,dataBufferOut);

    pSocket->writeDatagram(dataBufferOut,length, targetAddress, PORT);
    drive.setBit(&drive.parameterControlWord,eWriteParamters,false);

}

void MainWindow::on_buttonUploadMotorParam_clicked()
{
    //Send request
    drive.setBit(&drive.parameterControlWord,eReadParamters,true);
    lynx.setData<uint16_t>(parameters,eMotorParameterControl,drive.parameterControlWord);

    QHostAddress targetAddress = QHostAddress("10.0.0.90");
    int length = lynx.toBuffer(parameters,dataBufferOut);

    pSocket->writeDatagram(dataBufferOut,length, targetAddress, PORT);
    drive.setBit(&drive.parameterControlWord,eReadParamters,false);
}

void MainWindow::on_buttonSetTorqueMode_clicked()
{
    drive.setBit(&drive.controlWord,eSpeedMode,false);
    ui->buttonSetTorqueMode->setChecked(false);
}

void MainWindow::on_buttonSetSpeedMode_clicked()
{
    drive.setBit(&drive.controlWord,eSpeedMode,true);
    ui->buttonSetSpeedMode->setChecked(false);
}

void MainWindow::on_buttonUploadControlParam_clicked()
{
    //Send request
    drive.setBit(&drive.parameterControlWord,eReadParamters,true);
    lynx.setData<uint16_t>(parameters,eControlParameterControl,drive.parameterControlWord);

    int length = lynx.toBuffer(controlParam,dataBufferOut);

    pSocket->writeDatagram(dataBufferOut,length, targetAddress, PORT);
    drive.setBit(&drive.parameterControlWord,eReadParamters,false);
}

void MainWindow::on_buttonDownloadControlParam_clicked()
{
    drive.setBit(&drive.parameterControlWord,eWriteParamters,true);
    lynx.setData<uint16_t>(controlParam,eControlParameterControl,drive.parameterControlWord);
    lynx.setData<int32_t>(controlParam,eControlSpeedGainP,int32_t(ui->labelSpeedPGain_ACT->text().toDouble()*Q24));
    lynx.setData<int32_t>(controlParam,eControlSpeedGainI,int32_t(ui->labelSpeedIGain_ACT->text().toDouble()*Q24));
    lynx.setData<int32_t>(controlParam,eControlCurrentGainP,int32_t(ui->labelTorquePGain_ACT->text().toDouble()*Q24));
    lynx.setData<int32_t>(controlParam,eControlCurrentGainI,int32_t(ui->labelTorqueIGain_ACT->text().toDouble()*Q24));


    int length = lynx.toBuffer(controlParam,dataBufferOut);

    pSocket->writeDatagram(dataBufferOut,length, targetAddress, PORT);
    drive.setBit(&drive.parameterControlWord,eWriteParamters,false);
}



