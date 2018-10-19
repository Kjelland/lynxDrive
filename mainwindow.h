#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QLayout>
#include <QUdpSocket>
#include "LynxStructure.h"
#include "mycharts.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    uint8_t deviceID = 0xA3;
    uint8_t driveID = 0x32;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_comboBox_activated(const QString &arg1);

    void on_comboBox_highlighted(const QString &arg1);

    void on_buttonDisconnect_clicked();

    void on_pushButton_clicked();

    void readyReadUDP();

    void timerTick();

    void on_checkBoxChartSpeed_stateChanged(int arg1);

    void on_checkBoxChartTorque_stateChanged(int arg1);

    void on_checkBoxChartCurrent_stateChanged(int arg1);

    void on_checkBoxChartPosition_stateChanged(int arg1);

private:
    LynxStructureSpace::LynxStructure lynxController;
    LynxStructureSpace::LynxStructure lynxDrive;
    QTimer *pTimer;
    QUdpSocket *pSocket;
    MyCharts myChart;
    Ui::MainWindow *ui;
    bool searchInProgress;
};

#endif // MAINWINDOW_H
