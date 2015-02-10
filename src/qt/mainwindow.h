#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "controller.cpp"

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


    void updateGUI();

    void updateOriginal(Mat original );

    void updateCropped(Mat cropped);

    void updateResultMatching(Mat resultMatching);

    void on_btnForward_pressed(ros::NodeHandle);

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

    void on_btnTakeoffOrLand_clicked(ros::NodeHandle);

    void on_btnEmergOrRegular_clicked(ros::NodeHandle);

private:
    Ui::MainWindow *ui;

    ros::NodeHandle node;
    controller controls;

    Mat MatOriginal;
    Mat MatCropped;
    Mat MatResultMatching;

    QImage qimgOriginal;
    QImage qimgCropped;
    QImage qimgResultMatching;



    QTimer* tmrTimer;




};

#endif // MAINWINDOW_H
