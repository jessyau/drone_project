#include "display.h"
#include <QApplication>
#include <QWidget>
#include "widget.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
//#include <ros/ros.h>

//#include "imageconverter.h"

int main(int argc, char *argv[])
{

 /*   QApplication a(argc, argv);
        Widget w;
        QLabel cropped;

        Display d;
        cv::Mat inMat = cv::imread("/home/arianna/workshop/Qt/displayImage/quadcopter.jpg");
        QImage image = d.cvMatToQImage(inMat);
        QPixmap pix = QPixmap::fromImage(image);

        w.setPixmap(pix);
        w.resize(pix.size());
        w.show();
        cropped.setPixmap( w.getSavedPix());

        return a.exec();
*/
    QApplication a(argc, argv);
    Display w;
    w.show();

    return a.exec();


}

/*
int main(int argc, char** argv)
{
//  ros::init(argc, argv, "image_converter");
//  ImageConverter ic;
//  ros::spin();
  return 0;
}
*/
