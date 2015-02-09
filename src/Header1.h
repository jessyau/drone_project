#ifndef Header1_H
#define Header1_H
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <ardrone_autonomy/Navdata.h>
extern bool multiplyTwoNumbers(int test1, int test2);
extern bool initialize();
extern void sendTakeoff(ros::NodeHandle node);
#endif