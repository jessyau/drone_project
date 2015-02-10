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

    void on_btnForward_pressed();

    void on_btnForward_released();

    void on_btnBackward_pressed();

    void on_btnBackward_released();

    void on_btnLeft_pressed();

    void on_btnLeft_released();

    void on_btnRight_pressed();

    void on_btnRight_released();

    void on_btnTurnLeft_pressed();

    void on_btnTurnLeft_released();

    void on_btnTurnRight_pressed();

    void on_btnTurnRight_released();

    void on_btnHover_clicked();

    void on_btnUp_pressed();

    void on_btnUp_released();

    void on_btnDown_pressed();

    void on_btnDown_released();

    void on_btnResetTracking_clicked();

    void on_btnTakeoffOrLand_clicked();

    void on_btnEmergOrRegular_clicked();

private:
    Ui::MainWindow *ui;

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
