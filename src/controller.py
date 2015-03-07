#!/usr/bin/env python

import roslib; roslib.load_manifest('drone_project')
import rospy

# Import the messages we're interested in sending and receiving
from geometry_msgs.msg import Twist  	 # for sending commands to the drone
from std_msgs.msg import Empty       	 # for land/takeoff/emergency
from ardrone_autonomy.msg import Navdata # for receiving navdata feedback

import sys, select, termios, tty

intro = """

Keyboard Controls:

t : Takeoff
l : Land
r :	Reset
a :	Begin Tracking (to be implemented)
q :	Qui

Press Ctrl + C to Quit ROS

"""

class Controller(object):
	def __init__(self):

		self.inFlight = 0

		self.pubLand    = rospy.Publisher('/ardrone/land',Empty)
		self.pubTakeoff = rospy.Publisher('/ardrone/takeoff',Empty)
		self.pubReset   = rospy.Publisher('/ardrone/reset',Empty) 
		self.pubToggle	= rospy.Publisher('/ardrone/togglecam', Empty)

		self.pubMove = rospy.Publisher('/cmd_vel',Twist)
		self.move = Twist()
	

	def getKey(self):
		key = raw_input("Enter Command: ")
		return key

	def sendTakeoff(self):
		if(self.inFlight == 0):
			self.pubTakeoff.publish(Empty())
			self.inFlight = 1

	def sendLand(self):
		self.pubLand.publish(Empty())
		self.inFlight = 0

	def sendReset(self):
		self.pubReset.publish(Empty())

	def sendToggle(self):
		self.pubToggle.publish(Empty())

	def setMovement(self, pitch=0, roll=0, yaw=0):
		# x - forward (1)/ backward (-1) velocity 
		# y - left (1) / right (-1) velocity
		# z - up (1) /down (-1) velocity

		self.move.linear.x = pitch
		self.move.linear.y = roll
		self.move.linear.z = yaw

	def sendMovement(self):
		self.pubMove.publish(Twist)

	def autoMode(self, distance, angle):
		self.sendTakeoff()
		a = ((distance*tan(angle))**2) + 1
		v_y = 0.05/(a**(1/2))
		v_x = distance*tan(angle)*v_y
		self.setMovement(self, v_x, v_y)
		self.sendMovement()


if __name__=="__main__":
	print(intro)

	rospy.init_node('drone_controller')

	controls = Controller()


	try: 
		while(1):
			key = controls.getKey()

			if key == "t":
				print("Sent Takeoff")
				controls.sendTakeoff()
			elif key == "l":
				print("Sent Land")
				controls.sendLand()
			elif key == "r":
				print("Sent Reset")
				controls.sendReset()
			elif key == "a":
				print("Starting Auto Mode")
				controls.autoMode()
			elif key == "c":
				print("Toggle Camera")
				controls.sendToggle()
			elif key == "q":
				controls.sendLand()
				print("Quit")
				break
			else:
				print("Try again")
	except Exception as e:
		print e
		print repr(e)