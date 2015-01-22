#!/usr/bin/env python

import roslib; roslib.load_manifest('drone_project')
import rospy

# Import the messages we're interested in sending and receiving
from geometry_msgs.msg import Twist  	 # for sending commands to the drone
from std_msgs.msg import Empty       	 # for land/takeoff/emergency
from ardrone_autonomy.msg import Navdata # for receiving navdata feedback

intro = """

Keyboard Controls:

t : 	Takeoff
l : 	Land
e :		Emergency State
r :		Reset
a :		Begin Tracking (to be implemented)
q :		Quit

Press Ctrl + C to Quit ROS

"""

class Controller(object):
	def __init__(self):

		self.inFlight = 0

		self.pubLand    = rospy.Publisher('/ardrone/land',Empty)
		self.pubTakeoff = rospy.Publisher('/ardrone/takeoff',Empty)
		self.pubReset   = rospy.Publisher('/ardrone/reset',Empty) 

		self.pubCommand = rospy.Publisher('/cmd_vel',Twist)
		self.twist = Twist()
	

	def getKey(self):
		key = raw_input('Enter Command: ')
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



if __name__=="__main__":
	print(intro)

	rospy.init_node('drone_controller')

	controls = Controller()


	try: 
		while(1):
			key = controls.getKey()

			if key == 't':
				print("Sent Takeoff")
				controls.sendTakeoff()
			elif key == 'l':
				print("Sent Land")
				controls.sendLand()
			elif key == 'r':
				print("Sent Reset")
				controls.sendReset
			elif key == 'q':
				controls.sendLand()
				print("Quit")
				break
			else:
				print("Try again")
	except Exception as e:
		print e
		print repr(e)