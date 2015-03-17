#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QtWidgets>
#include <QtCore>


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



#include "widget.h"

#include "image_converter.h"
//#include "controller.h"
#include "controller.h"

using namespace cv;
using namespace std;



MainWindow::MainWindow(controller control, ros::NodeHandle node, QWidget *parent) :
    controls(control),
    qnode(node),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    land = 1;
    count = 0;
 //   tmrTimer = new QTimer(this);
        controls.pubLand = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
    controls.pubReset = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);
    controls.pubTakeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
    controls.pubTwist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    controls.nav_sub = node.subscribe("/ardrone/navdata", 1, &(nav_callback));

    ros::spinOnce();
    

//    ImageConverter ic(node);
 //  ros::spinOnce();
    

//    MatOriginal = ic.getCameraFeed();
/*    image_transport::Subscriber image_sub_;
    image_transport::ImageTransport it_(node);
    image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &MainWindow::qtCallBack, this);
*/
//    ui->txtStatus->setText("Getting from controls, Altitude : " );//+ QString::number(controls.getAltitude())
/*    
    controls.pubLand = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
    controls.pubReset = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);
    controls.pubTakeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
    controls.pubTwist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
 */   

    int i = 0;
    while(i < 5){
        sleep(0.5);
        controls.sendReset(node);

        i++;
    }

 //   ui->lblVideofeed->setText("Battery is :"+ QString::number(controls.getBattery()));
    ui->lblStatus->setText("Battery: " + QString::number(controls.getBattery()) + ". Altitude: " + QString::number(controls.getAltitude()));
    
std::cout << "Battery" << controls.getBattery() << " bump " << "\n";
 /******
     * This is how to put a image into a label using cv::Mat and QImage
    Mat inMat;
        inMat = imread("Lenna.jpg");   // Read the file

    cout << inMat.type();
    QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32);

    ui->lblScreenshot->setPixmap(QPixmap::fromImage(image));

    ui->lblScreenshot->adjustSize();
*/
/*    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(this->updateGUI()));
    connect(ui->btnTakeoffOrLand, SIGNAL(clicked()), this, SLOT(on_btnTakeoffOrLand_clicked(3)) );
    connect(ui->btnEmergOrRegular, SIGNAL(clicked()), this, SLOT(on_btnEmergOrRegular_clicked(node)));
    tmrTimer->start(20);
*/

//    ros::spinOnce();
}

MainWindow::~MainWindow()
{
    ros::shutdown();
    delete ui;
}

void MainWindow::Init(ros::NodeHandle qnode)
{

}


//timer works fine. this function is used for testing buttons -- all test works fine
void MainWindow::testTimer(){
    ui->lblStatus->setText(QString::number(count));
    count++;
}

void MainWindow::qtCallBack(const sensor_msgs::ImageConstPtr& msg){
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
}

void MainWindow::updateGUI(ImageConverter ic){
//    ros::spinOnce();
    MatOriginal = cv_ptr->image;
    updateOriginal(MatOriginal);
    
//    MatOriginal = ic.getCameraFeed();
    
}

void MainWindow::updateOriginal(Mat original ){
    ui->lblVideofeed->setPixmap(QPixmap::fromImage(cvMatToQImage(original)));
    ui->txtStatus->setText("Hello world~");
}

void MainWindow::updateCropped(Mat cropped){

}

void MainWindow::updateResultMatching(Mat resultMatching){

}


// calling control functions that twist the drone to the left
/*
void MainWindow::on_btnForward_pressed(ros::NodeHandle node)
{
    controls.setMovement(1,0,0);
    controls.sendMovement(node);
    ros::spinOnce();
}

void MainWindow::on_btnForward_released(ros::NodeHandle node)
{
    controls.Hover(node);
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
    ui->txtStatus->append("clicked reset tracking");
}

void MainWindow::on_btnTakeoffOrLand_clicked(int i)
{
    ui->txtStatus->append("clicked takeoff " + QString::number(i));
    if(land){
//        controls.sendTakeoff(node);
        land = 0;
        ui->btnTakeoffOrLand->setText("Land");
    }else{
//        controls.sendLand(node);
        land = 1;
        ui->btnTakeoffOrLand->setText("Takeoff");
    }
    
}

void MainWindow::on_btnEmergOrRegular_clicked(ros::NodeHandle node)
{
    controls.sendReset(node);
    ui->txtStatus->append("Emergency Button clicked");
}
*/

QImage MainWindow::cvMatToQImage( const cv::Mat &inMat )
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
   }

