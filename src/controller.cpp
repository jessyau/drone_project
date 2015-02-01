#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include "std_msgs/String.h"
#include <cxcore.h>
#include <string>

using namespace std;


class controller
{
private:
	ros::NodeHandle& nh;
	ros::Subscriber sub_pos;
	ros::Publisher pubLand, pubTakeoff, pubReset;
	
	string command;
	int inFlight;

public:
	controller(string, int);
	void getKey();
	void sendTakeoff();
	void sendLand();
	void sendReset();
};

void StartMenu()
{
	cout << "Keyboard Controls\n";
	cout << "t 	:	Takeoff\n";
	cout << "l 	:	Land \n";
	cout << "r 	: 	Reset\n";
	cout << "q 	: 	Quit\n\n";
}

controller::controller(string a, int b)
{
	command = a;
	inFlight = b;
}

void controller::getKey()
{
	cout << "Enter Command: ";
	getline (cin, command);
}

void controller::sendTakeoff()
{
	if(inFlight == 0)
	{
		pubTakeoff.publish(Empty());
		inFlight = 1;
	}
}

void controller::sendLand()
{
	pubLand.publish(Empty());
}

void controller::sendReset()
{
	pubReset.publish(Empty());
}

int main()
{
	controller controls ("-1", 0);

	StartMenu();

	int quit = 0;

	while(quit == 0) 
	{
		controls.getKey();

		if(controls.key.compare("t"))
		{
			controls.sendTakeoff();
			cout<<"Sent Takeoff";
		} else if(controls.key.compare("l"))
		{
			controls.sendLand();
			cout<<"Sent Land";
		} else if(controls.key.compare("r"))
		{
			controls.sendReset();
			cout<<"Sent Reset";
		} else if(controls.key.compare("q"))
		{
			quit = 1;
		} else
			cout<<"Try again."
	}

}