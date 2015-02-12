#include "display.h"
#include "ui_display.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <QtWidgets>

#include <math.h>

#include <widget.h>



using namespace cv;
using namespace std;


QImage  Display::cvMatToQImage( const cv::Mat &inMat )
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


Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);

    Mat inMat = imread("/home/arianna/workshop/Qt/displayImage/quadcopter.jpg");
    QImage image = cvMatToQImage(inMat);
    QPixmap pix = QPixmap::fromImage(image);

//    ui->lblImage->setPixmap(pix);



    ui->txtDisplay->setText("What up!");
    if(inMat.data == NULL){}
 //        ui->lblImage->setText("cannot read the image");
    else
     {   ui->lblImage->setPixmap(pix);
        ui->lblImage->resize(pix.size());

    }

    connect(ui->lblImage, SIGNAL(pressed()), this, SLOT(on_lblImage_clicked()));


}

Display::~Display()
{
    delete ui;
}


void Display::on_lblImage_clicked()
{
    ui->txtDisplay->setText("lblImage clicked~");

}

void Display::on_lblImage_released()
{
    ui->txtDisplay->setText("lblImage released~");
//    ui->lblSaved->setText(QString::number(ui->lblImage->getSelectionRect().width()));
    ui->lblSaved->setPixmap(ui->lblImage->pixmap()->copy(ui->lblImage->getSelectionRect()));
}
