#include "mainwindow.h"
#include <QApplication>

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

#include "controller.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
	ros::init(argc, argv, "controller");
    ros::NodeHandle node;

	controller controls;
	if (!initialize()){  return 0;}
	controls.Init("-1", 0, argc, argv);
	

//    sleep(1);
//    ros::spinOnce();
    


    
    MainWindow w(controls, node);
     w.show();


     

    ros::shutdown();
 //   ros::spin();
//    exit(0);
//    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
