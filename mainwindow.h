#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QLayout>
#include <QUdpSocket>
#include "LynxStructure.h"
#include "mycharts.h"
#include "lynxdrive.h"

#define MY_ID 0xAB

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

    void readyReadUDP();

    void timerTick();

    void on_checkBoxChartSpeed_stateChanged(int arg1);

    void on_checkBoxChartTorque_stateChanged(int arg1);

    void on_checkBoxChartCurrent_stateChanged(int arg1);

    void on_checkBoxChartPosition_stateChanged(int arg1);

    void on_buttonEnableDrive_toggled(bool checked);

    void on_buttonOn1_toggled(bool checked);

    void on_buttonOff2_toggled(bool checked);

    void on_buttonOff3_toggled(bool checked);

    void on_buttonSaveFlash_clicked();

    void on_buttonReadFlash_clicked();

    void on_labelNumerofPoles_ACT_editingFinished();

    void on_labelResistance_ACT_editingFinished();

    void on_labelLsq_ACT_editingFinished();

    void on_labelLsd_ACT_editingFinished();

    void on_labelEMF_ACT_editingFinished();

private:
    char dataBufferOut[64];

    LynxDrive drive;

    void updateButtons(void);

    LynxStructureSpace::LynxStructure lynxController;

    LynxStructureSpace::LynxStructure lynxDrive;

    QTimer *pTimer;

    QUdpSocket *pSocket;

    MyCharts myChart;

    Ui::MainWindow *ui;

    bool searchInProgress;
};

#endif // MAINWINDOW_H
