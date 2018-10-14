#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QNetworkDatagram>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{ui->setupUi(this);
    socket = new QUdpSocket(this);
        socket->bind(11000);

        connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadUDP()));
        timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
            timer->start(10);
        /*for (int n=0;n<numberOfSamples;n++)
        {
            series.append(n,0);
        }
        */



        series.setName("Batman");
        QPen pen(0x059605);
        pen.setWidth(3);
        series.setPen(pen);


       // QChart *chart = new QChart();
        chart.addSeries(&series);
        chart.setTitle("Simple areachart example");
        chart.createDefaultAxes();
        //chart->axisX()->setRange(0, 20);
        chart.axisY()->setRange(-10, 10);
        ui->graphicsView->setRenderHint(QPainter::Antialiasing);
        ui->graphicsView->setChart(&chart);

    ui->comboBox->addItem("Search for devices...");
    // Used to display the chart

    //chart->setParent(ui->graph);
    //chart->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyReadUDP()
{
    QNetworkDatagram datagram = socket->receiveDatagram();//receiveBuffer.data(), receiveBuffer.size(), &sender, &senderPort


    qDebug()<<"UDP Recived:"<<datagram.senderAddress().toIPv4Address();


    //dataStoring.m_motorSpeed.setText(QString::number((float)(buffer[1])/1.0,'f',1));
    //dataStoring.m_motorTorque.setText(QString::number((float)(buffer[2])/1.0,'f',1));
}
void MainWindow::timerTick()
{
    if(series.count() > numberOfSamples)
         series.removePoints(0,1);

    series.append(test++, (qrand() % 10-5)*1.8);

chart.axisX()->setRange(series.at(0).x(), series.at(series.count()- 1).x());
    //qDebug()<<dataList.at(1);
    //qDebug()<<dataList.at(2);

//dataList.append(new DataSource("Item 5", "green"));
//m_appViewer->rootContext()->setContextProperty("myModel", QVariant::fromValue(dataList));
    // mapping
    /*
    while(lynx.txPackages_left>0){
       lynx.LynxTransmit(transmitBuffer, &targetIpAddress);

        socket->writeDatagram((char*)transmitBuffer,lynx.txPackage_length,QHostAddress(targetIpAddress) , 11000);
     }*/
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
    //series.clear();

    //chart.axisY()->setRange(0, 6500);

    //ui->graphicsView->setChart(&chart);
}
