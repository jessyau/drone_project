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
