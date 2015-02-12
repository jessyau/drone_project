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
	controller controls;
	if (!initialize()){  return 0;}
	controls.Init("-1", 0, argc, argv);
	ros::NodeHandle node;
	controls.pubLand = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
	controls.pubReset = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);
	controls.pubTakeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
	controls.pubTwist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    controls.nav_sub = node.subscribe("/ardrone/navdata", 1, &(nav_callback));
    
    std::cout << "Battery" << controls.getBattery() << " bump " << "\n";

    sleep(50);
    controls.sendReset(node);

    sleep(50);
    controls.sendReset(node);


sleep(50);
    controls.sendReset(node);


sleep(50);
    controls.sendReset(node);


sleep(50);
    controls.sendReset(node);


    // MainWindow w(controls.getBattery(), argv);
    // w.show();

    ros::shutdown();
 //   ros::spin();
    exit(0);

    return a.exec();
}
