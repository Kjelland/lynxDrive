#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QLayout>
#include <QUdpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_comboBox_activated(const QString &arg1);

    void on_comboBox_highlighted(const QString &arg1);

    void on_buttonDisconnect_clicked();

    void on_pushButton_clicked();
    void readyReadUDP();
    void timerTick();
private:
    int numberOfSamples=100;
    int test=0;
    QTimer *timer;
    QUdpSocket *socket;
    QChart chart;
    //QLineSeries series ;
    QSplineSeries series ;
    Ui::MainWindow *ui;
    bool searchInProgress;
};

#endif // MAINWINDOW_H
