#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();

private:
    Ui::Display *ui;

public slots:
    QImage cvMatToQImage( const cv::Mat &inMat );

private slots:
    void on_lblImage_clicked();
    void on_lblImage_released();
};

#endif // DISPLAY_H
