#ifndef CONTROLLER_H
#define CONTROLLER_H

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

using namespace std;

static bool initialized = false;
static struct termios initial_settings;
static double test_flight_time = 10.0;

class controller
{
public:
	controller(){}
	bool linebufferedController(bool on = true);
	bool echoController(bool on = true);
	bool iskeypressed(unsigned timeout_ms = 0);

	void Init(string a, int b , int argc, char** argv);
	string key;
	int inFlight;
	ros::Publisher pubLand, pubTakeoff, pubReset, pubTwist, pubGetNav;
	ros::Subscriber nav_sub;
	void getKey();
	void sendTakeoff(ros::NodeHandle);
	void sendLand(ros::NodeHandle);
	void sendReset(ros::NodeHandle);
	void setMovement(double, double, double);
	void setTurn(double);
	void resetTwist();
	void sendMovement(ros::NodeHandle);
	void autoMode(ros::NodeHandle);
	void testMove(ros::Rate, ros::NodeHandle);
	void gainAltitude(double, ros::NodeHandle);
/*	void nav_callback(const ardrone_autonomy::Navdata& msg_in){
	battery = msg_in.batteryPercent;
	//cout << "Getting battery life" << endl;
	altitude = msg_in.altd*0.001; //mm to m
	vx = msg_in.vx*0.001; // From mm/s to m/s
	vy = msg_in.vy*0.001;
}*/
	void proportional(double, double, double, double);
	void hover(ros::NodeHandle);
	double getBattery();
	double getAltitude();
	geometry_msgs::Twist getTwist();


private:
	geometry_msgs::Twist twist_msg;
	std_msgs::Empty emp_msg;
	string command;
};

void nav_callback(const ardrone_autonomy::Navdata& msg_in);
/*{
	battery = msg_in.batteryPercent;
	//cout << "Getting battery life" << endl;
	altitude = msg_in.altd*0.001; //mm to m
	vx = msg_in.vx*0.001; // From mm/s to m/s
	vy = msg_in.vy*0.001;
}*/
#endif // CONTROLLER_H