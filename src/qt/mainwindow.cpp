#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QtWidgets>

#include <ros/ros.h>
#include <sstream>
#include <string>
#include <iostream>
#include "std_msgs/String.h"
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <cstdio>
#include <limits>
#include <unistd.h>
#include <termios.h>
#include <poll.h>
#include <ardrone_autonomy/Navdata.h>
#include "controller.cpp"

using namespace cv;
using namespace std;

MainWindow::MainWindow(int argc, char* argv[], QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ros::init(argc, argv, "controller");
    controls.Init("-1", 0, argc, argv);
    ros::NodeHandle node;
    controls.pubLand = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
    controls.pubReset = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);
    controls.pubTakeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
    controls.pubTwist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);




    ui->lblVideofeed->setText("Battery is :"+ QString::number(controls.getBattery()));



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

void MainWindow::on_btnForward_pressed(ros::NodeHandle node)
{
    controls.setMovement(1,0,0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnForward_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnBackward_pressed(ros::NodeHandle node)
{
    controls.setMovement(-1,0,0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnBackward_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnLeft_pressed(ros::NodeHandle node)
{
    controls.setMovement(0,1,0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnLeft_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnRight_pressed(ros::NodeHandle node)
{
    controls.setMovement(0,-1,0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnRight_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnTurnLeft_pressed(ros::NodeHandle node)
{
    controls.setTurn(-1);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnTurnLeft_released(ros::NodeHandle node)
{
    controls.setTurn(0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnTurnRight_pressed(ros::NodeHandle node)
{
    controls.setTurn(1);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnTurnRight_released(ros::NodeHandle node)
{
    controls.setTurn(0);
    controls.sendMovement(node);
    ros::spinOnce();
}


void MainWindow::on_btnHover_clicked(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnUp_pressed(ros::NodeHandle node)
{
    controls.setMovement(0,0,1);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnUp_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnDown_pressed(ros::NodeHandle node)
{
    controls.setMovement(0,0,-1);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnDown_released(ros::NodeHandle node)
{
    controls.hover(node);
    ros::spinOnce();
}

void MainWindow::on_btnResetTracking_clicked(ros::NodeHandle node)
{

}

void MainWindow::on_btnTakeoffOrLand_clicked(ros::NodeHandle node)
{
    controls.sendTakeoff(node);
}

void MainWindow::on_btnEmergOrRegular_clicked(ros::NodeHandle node)
{
    controls.sendReset(node);
}

void MainWindow::updateGUI(){

}

void MainWindow::updateOriginal(Mat original ){

}

void MainWindow::updateCropped(Mat cropped){

}

void MainWindow::updateResultMatching(Mat resultMatching){

}