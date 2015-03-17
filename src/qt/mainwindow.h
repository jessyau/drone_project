#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ros/ros.h>

#include "controller.h"
#include "image_converter.h"

using namespace cv;

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;

QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(controller control, ros::NodeHandle node, QWidget *parent = 0);
    ~MainWindow();

public slots: 

    void qtCallBack(const sensor_msgs::ImageConstPtr& msg);

    void updateGUI(ImageConverter ic);

    void updateOriginal(Mat original );

    void updateCropped(Mat cropped);

    void updateResultMatching(Mat resultMatching);

    QImage cvMatToQImage( const cv::Mat &inMat );

private slots:

    void testTimer();
    void Init(ros::NodeHandle qnode);

/*    void on_btnForward_pressed(ros::NodeHandle);

    void on_btnForward_released(ros::NodeHandle);

    void on_btnBackward_pressed(ros::NodeHandle);

    void on_btnBackward_released(ros::NodeHandle);

    void on_btnLeft_pressed(ros::NodeHandle);

    void on_btnLeft_released(ros::NodeHandle);

    void on_btnRight_pressed(ros::NodeHandle);

    void on_btnRight_released(ros::NodeHandle);

    void on_btnTurnLeft_pressed(ros::NodeHandle);

    void on_btnTurnLeft_released(ros::NodeHandle);

    void on_btnTurnRight_pressed(ros::NodeHandle);

    void on_btnTurnRight_released(ros::NodeHandle);

    void on_btnHover_clicked(ros::NodeHandle);

    void on_btnUp_pressed(ros::NodeHandle);

    void on_btnUp_released(ros::NodeHandle);

    void on_btnDown_pressed(ros::NodeHandle);

    void on_btnDown_released(ros::NodeHandle);

    void on_btnResetTracking_clicked(ros::NodeHandle);

    void on_btnTakeoffOrLand_clicked(int i);

    void on_btnEmergOrRegular_clicked(ros::NodeHandle);
*/
private:
    Ui::MainWindow *ui;

 //   ros::NodeHandle node;
    controller controls;
    ros::NodeHandle qnode;
//    ImageConverter ic;

    bool land; // flag of landing
    int count; // used for test timer, no other purpose

    Mat MatOriginal;
    Mat MatCropped;
    Mat MatResultMatching;

    QImage qimgOriginal;
    QImage qimgCropped;
    QImage qimgResultMatching;

 //   QLabel lblStatus;

    QTimer* tmrTimer;

    cv_bridge::CvImagePtr cv_ptr;
//    image_transport::Subscriber image_sub_;




};

#endif // MAINWINDOW_H
