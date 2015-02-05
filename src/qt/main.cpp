#include <QCoreApplication>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat image;
        image = imread("/home/arianna/workshop/Qt/Lenna.jpg");   // Read the file
        if(!image.data)
        {
            cout << "where's the image!";
        }
        else
        {
            cout << "imagem found!";
        }
        cv::namedWindow("Test image");
        cv::imshow("Test image", image);
        cv::waitKey(0);
        return 0;
}
