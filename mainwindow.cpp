#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->comboBox->addItem("Search for devices...");
}

MainWindow::~MainWindow()
{
    delete ui;
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
