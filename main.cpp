#include "mainwindow.h"
#include <QApplication>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtMultimedia/QAudioDeviceInfo>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
