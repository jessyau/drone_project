#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QtWidgets>
#include "controller.cpp"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controls.Init(0);

    ui->lblStatus->setText("Battery is :"+ QString::number(controls.getBattery()));



    /******
     * This is how to put a image into a label using cv::Mat and QImage
    Mat inMat;
        inMat = imread("Lenna.jpg");   // Read the file

    cout << inMat.type();
    QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32);

    ui->lblScreenshot->setPixmap(QPixmap::fromImage(image));

    ui->lblScreenshot->adjustSize();
*/


}

MainWindow::~MainWindow()
{
    delete ui;
}


// calling control functions that twist the drone to the left

void MainWindow::on_btnForward_pressed()
{

}

void MainWindow::on_btnForward_released()
{

}

void MainWindow::on_btnBackward_pressed()
{

}

void MainWindow::on_btnBackward_released()
{

}

void MainWindow::on_btnLeft_pressed()
{

}

void MainWindow::on_btnLeft_released()
{

}

void MainWindow::on_btnRight_pressed()
{

}

void MainWindow::on_btnRight_released()
{

}

void MainWindow::on_btnTurnLeft_pressed()
{

}

void MainWindow::on_btnTurnLeft_released()
{

}

void MainWindow::on_btnTurnRight_pressed()
{

}

void MainWindow::on_btnTurnRight_released()
{

}



void MainWindow::on_btnHover_clicked()
{

}

void MainWindow::on_btnUp_pressed()
{

}

void MainWindow::on_btnUp_released()
{

}

void MainWindow::on_btnDown_pressed()
{

}

void MainWindow::on_btnDown_released()
{

}

void MainWindow::on_btnResetTracking_clicked()
{

}

void MainWindow::on_btnTakeoffOrLand_clicked()
{

}

void MainWindow::on_btnEmergOrRegular_clicked()
{

}
