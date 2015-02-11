#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
	ros::init(argc, argv, "controller");

    MainWindow w(argc, argv);
    w.show();

    return a.exec();
}
