#include "Header1.h"
#include "controller.cpp"

using namespace std;
using namespace cv;

Mat cameraFeed;
Mat cameraFeedCropped;

//matrix storage for HSV image
Mat HSV;
//matrix storage for binary threshold image
Mat threshold;
//x and y values for the location of the object

cv_bridge::CvImagePtr cv_ptr;

Mat gray;
Mat tmpImage;


cv::Mat croppedImage;
cv::Mat croppedCamera;
cv::Rect myROI;
cv::Rect croppedCameraROI;

Mat resultOriginal;
Mat resultMoving;
Mat resultMovingFull;
Mat binary_image;
Mat movement;
Mat imageb;
Mat imageF;

double PrevMatchLocX = 0;
double PrevMatchLocY = 0;

int posCroppedCameraX = 0;
int posCroppedCameraY = 0;
int shiftCropped = 30;
int prePosCroppedCameraX = 0;
int prePosCroppedCameraY = 0;
int prePrePosCroppedCameraX = 0;
int prePrePosCroppedCameraY = 0;
int OriginalMatchLocMovingX = 0;
int OriginalMatchLocMovingY = 0;

bool lost = true;
bool freezeMoveImage = false;
bool firstCapture = true;
bool calibrate = true;

int changeInX = 0;
int changeInY = 0;

bool hitLeftWall = false;
bool hitRightWall = false;
bool hitUpWall = false;
bool hitDownWall = false;
	
bool doOnce = false;
bool useFullCamera = false;

int lostCount = 0;
double confidPercent = 1;

//This rectangle is the for the area selected using the mouse
int Rectanglex1;
int Rectangley1;
int Rectanglex2;
int Rectangley2;

//Boolean variable to search only after an image has been selecte
bool searchImg = false;
bool mouseUp;

//default capture width and height
int FRAME_WIDTH = 640;
int FRAME_HEIGHT = 380;

int argc;
char** argv;

controller controls;

int numLoops = 0;

double angle = 0;		

bool land = false;

bool freezeFrame = false;
bool copyImage = false;
int leftClickCount = 0;

double lastGoodxVel = 0;
double lastGoodyVel = 0;

int moveForward = 0;
int moveBackward = 0;
int moveLeft = 0;
int moveRight = 0;

bool mouseControlMove = false;


//This function uses the mouse clicks to setup the search image
void CallBackFunc(int event, int x, int y, int flags, void* param) {

	//Where the mouses' left button is clicked store the x, and y points into the rectangle 	variables
	if ((event == EVENT_LBUTTONDOWN) && (mouseControlMove == false)) {
		cout << "Mouse Down (" << x << ", " << y << ")" << endl;
		Rectanglex1 = x;
		Rectangley1 = y;
		mouseUp = false;
		searchImg = false;
	//When the left button is released store the x, and y position into rectangle 2
	} else if ((event == EVENT_LBUTTONUP) && (mouseControlMove == false)) {
		cout << "Mouse Up (" << x << ", " << y << ")" << endl;
		Rectanglex2 = x;
		Rectangley2 = y;
		mouseUp = true;
		freezeFrame = false;

		//this is just for testing
		copyImage = true;
		
	} else if (event == EVENT_RBUTTONUP)  {
		if (leftClickCount == 0) {
			freezeFrame = true;
			copyImage = true;
			leftClickCount = leftClickCount + 1;
			cout << "leftClickCount: " << leftClickCount << endl;
		} else {
			land = true;
			freezeFrame = false;
			cout << "Sent Reset\n";
		}
	}else if ((event == EVENT_LBUTTONDOWN) && (mouseControlMove == true)) {
		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
		
		cout << "FRAME_WIDTH: " << FRAME_WIDTH << "\tFRAME_HEIGHT: " << FRAME_HEIGHT << endl;

		moveForward = 0;
		moveBackward = 0;
		moveLeft = 0;
		moveRight = 0;

		if ((y < 100) && (x > FRAME_WIDTH / 2 - 50) && (x < FRAME_WIDTH / 2 + 50)) {
			cout << "move forward " << endl;
			moveForward = 6;

		}
		else if ((y > FRAME_HEIGHT - 100) && (x > FRAME_WIDTH / 2 - 50) && (x < FRAME_WIDTH / 2 + 50)) {
			cout << "move backwards" << endl;
			moveBackward = 6;
		}
		else if ((y > FRAME_HEIGHT / 2 - 50) && (y < FRAME_HEIGHT / 2 + 50) && (x < 100)) {
			cout << "move left" << endl;
			moveLeft = 6;
		}
		else if ((y > FRAME_HEIGHT / 2 - 50) && (y < FRAME_HEIGHT / 2 + 50) && (x > FRAME_WIDTH - 100)) {
			cout << "move right" << endl;
			moveRight = 6;
		}
	}
}


class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
  
	public:
	
	void setMouseLeftClickDown(int Rectanglex1, int Rectangley1);
	void setMouseLeftRelease(int Rectanglex2, int Rectangley2);
	void setLost(bool lostSetTo);


	Mat getCroppedImage();
	Mat getCameraFeed();
	Mat getMatchingResults();

	ImageConverter(ros::NodeHandle node)
	  : nh_(node), it_(nh_)
	{
	    // Subscrive to input video feed and publish output video feed
	  image_sub_ = it_.subscribe("/ardrone/image_raw", 1, 
	    &ImageConverter::imageCb, this);
	  image_pub_ = it_.advertise("/image_converter/output_video", 1);

	  cv::namedWindow("Drone Camera");
	}

	~ImageConverter()
	{
	  cv::destroyWindow("Drone Camera");
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		/// Localizing the best match with minMaxLoc
		double minValMoving; double maxValMoving; Point minLocMoving; Point maxLocMoving;
		Point matchLocMoving;

		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

		double distance = 0;
	    
	    try
	    {
	      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	    }
	    catch (cv_bridge::Exception& e)
	    {
	      ROS_ERROR("cv_bridge exception: %s", e.what());
	      return;
	    }
			// controls.nav_sub = nh_.subscribe("/ardrone/navdata", 1, &nav_callback);	
			
		linebufferedController(false);
		echoController(false);

		if (iskeypressed(5) && cin.get() == 'm') {
			mouseControlMove = !(mouseControlMove);
			cout << "Toggled control with mouse, results: " << mouseControlMove << endl;
		}

		echoController();
		linebufferedController();

		if (mouseControlMove == true) {
			if (moveForward > 0) {
				controls.setMovement(1, 0, 0);
				controls.sendMovement(nh_);
				cout << "Moving forward" << endl;
				moveForward = moveForward - 1;
			} else if (moveLeft > 0) {
				controls.setMovement(0, 1, 0);
				controls.sendMovement(nh_);
				cout << "Moving left" << endl;
				moveLeft = moveLeft - 1;
			} else if  (moveBackward > 0) {
				controls.setMovement(-1, 0, 0);
				controls.sendMovement(nh_);
				cout << "Moving back" << endl;
				moveBackward = moveBackward - 1;
			} else if  (moveRight > 0) {
				controls.setMovement(0, -1, 0);
				controls.sendMovement(nh_);
				cout << "Moving right" << endl;
				moveRight = moveRight - 1;
			} else {

				controls.setMovement(0, 0, 0);
				controls.sendMovement(nh_);
			}
		}

		FRAME_WIDTH = cv_ptr->image.cols;
		FRAME_HEIGHT = cv_ptr->image.rows;

		if (searchImg == false) {

			// if (numLoops == 0) {
			// 	controls.pubLand = nh_.advertise<std_msgs::Empty>("/ardrone/land", 1);
			// 	controls.pubReset = nh_.advertise<std_msgs::Empty>("/ardrone/reset", 1);
			// 	controls.pubTakeoff = nh_.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
			// 	controls.pubTwist = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
			// }
			ros::Rate loop_rate(100);
		
			if (numLoops == 10) {
				controls.sendTakeoff(nh_);
				cout << "TakeOff" << endl;
				cout << "inFlight: " << controls.inFlight << "\n";
				cout << "Battery: " << controls.getBattery() << endl;
			} else if ((numLoops > 140) && (controls.getAltitude() < 1.1) && (mouseControlMove == false)) {
				// controls.setMovement(0, 0, 0);
				// controls.sendMovement(nh_);

				controls.gainAltitude(1.2, nh_); 
				cout << "Altitude: " << controls.getAltitude() << endl;
			} else if ((numLoops >= 140) && (mouseControlMove == false)) {
				//after rising, keep position at 0,0,0
				controls.setMovement(0, 0, 0);
				controls.sendMovement(nh_);
			}
		}
		
		numLoops = numLoops + 1;

		if (numLoops > 2000) {
			numLoops = 2000;
		}

		try {

			if (land == true) {
					controls.resetTwist();
					controls.setMovement(0, 0, 0);
					controls.sendMovement(nh_);
					controls.sendLand(nh_);
					cout << "Sent Land\n";
					cout << "inFlight: " << controls.inFlight << "\n";
					land = false;
			}

			//set a callback duty for any rodent event
			setMouseCallback("Drone Camera", CallBackFunc, NULL);

			if (mouseUp == true) {

				cout << "x1 y1 (" << Rectanglex1 << ", " << Rectangley1 << ")" << endl;
				cout << "x2 y2 (" << Rectanglex2 << ", " << Rectangley2 << ")" << endl;		
				
				myROI = Rect(Rectanglex1, Rectangley1, abs(Rectanglex2 - Rectanglex1), 
							 abs(Rectangley2 - Rectangley1));

				Mat imgPanelRoi(cameraFeed, myROI);
				imgPanelRoi.copyTo(croppedImage);
				mouseUp = false;
				searchImg = true;
			}

			if (searchImg == true) {

				/// Create the result matrix
				int resultOriginal_cols = cv_ptr->image.cols - croppedImage.cols + 1;
				int resultOriginal_rows = cv_ptr->image.rows - croppedImage.rows + 1;

				resultOriginal.create(resultOriginal_cols, resultOriginal_rows, CV_32FC1);

				//cout << "confidPercent: " << confidPercent << endl;
				if ((confidPercent < 0.5) && (lostCount <= 0)){
					lost = true;
					lostCount = 3;
					//cout << "Lost!!!!!" << endl;
				}

				if (lost == true) {
					
					int rowsCamera = cv_ptr->image.rows;
					int rowsCropped = croppedImage.rows;
					
					if (croppedImage.type() == CV_8U) {
						cvtColor(croppedImage, croppedImage, CV_GRAY2RGB);
					}

					if ((croppedImage.cols < cv_ptr->image.cols) && (croppedImage.rows < cv_ptr->image.rows)) {				
						matchTemplate(cv_ptr->image, croppedImage, resultMovingFull, CV_TM_CCOEFF_NORMED);
						useFullCamera = true;
					}
				} 
				else {
					int width = croppedImage.cols + shiftCropped * 2;
					int height = croppedImage.rows + shiftCropped * 2;

					x1 = posCroppedCameraX;
					y1 = posCroppedCameraY;

					if (posCroppedCameraX <= 0) {
						x1 = 0;
					}

					if (posCroppedCameraY <= 0) {
						y1 = 0;
					}

					if ((x1 + width < cv_ptr->image.cols) && (y1 + height < cv_ptr->image.rows)) {
						croppedCameraROI = Rect(x1, y1, width, height);
						Mat imgPanelRoi(cv_ptr->image, croppedCameraROI);
						imgPanelRoi.copyTo(croppedCamera);
					}

					//make sure the 'search area' does not exceded the cameras image
					x1 = PrevMatchLocX;
					y1 = PrevMatchLocY;
					x2 = x1 + croppedImage.cols;
					y2 = y1 + croppedImage.rows;

					if (x2 > cv_ptr->image.cols) {
						x2 = cv_ptr->image.cols;
					}

					if (y2 > cv_ptr->image.rows) {
						y2 = cv_ptr->image.rows;
					}

					if ((!(croppedCamera.empty())) && (croppedImage.cols < cv_ptr->image.cols) && (croppedImage.rows < cv_ptr->image.rows)) {
						if (useFullCamera == true) {
							matchTemplate(croppedCamera, croppedImage, resultMovingFull, CV_TM_CCOEFF_NORMED);
						} else {
							matchTemplate(croppedCamera, croppedImage, resultMoving, CV_TM_CCOEFF_NORMED);
						}
					}
				}

				if (useFullCamera == true) {
					minMaxLoc(resultMovingFull, &minValMoving, &maxValMoving, &minLocMoving, &maxLocMoving, Mat());
				} else {
					minMaxLoc(resultMoving, &minValMoving, &maxValMoving, &minLocMoving, &maxLocMoving, Mat());
				}

				useFullCamera = false;
			
				confidPercent = maxValMoving;
				/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
				
				matchLocMoving = maxLocMoving;

				distance = PrevMatchLocX - matchLocMoving.x;
				//distanceY = PrevMatchLocY - matchLocMoving.y;
				
				lostCount = lostCount - 1;

				if (lostCount < 0) {
					lostCount = 0;
				}

				if (lost == true) {
					posCroppedCameraX = matchLocMoving.x - shiftCropped;
					posCroppedCameraY = matchLocMoving.y - shiftCropped;

					calibrate = true;
					
					hitLeftWall = false;
					hitRightWall = false;
					hitUpWall = false;
					hitDownWall = false;

					//store original matchLocMoving values to be used as a reference
					OriginalMatchLocMovingX = matchLocMoving.x;
					OriginalMatchLocMovingY = matchLocMoving.y;
				} 
				else {
					//this is to keep it stable when tracking an object
					//store 2nd previous camera position in prePrePosCroppedCamera
					prePrePosCroppedCameraX = prePosCroppedCameraX;
					prePrePosCroppedCameraY = prePosCroppedCameraY;

					//calculate previous position
					prePosCroppedCameraX = matchLocMoving.x + posCroppedCameraX;
					prePosCroppedCameraY = matchLocMoving.y + posCroppedCameraY;

					//if not the first time time finding image, then
					if (calibrate == false) {
						posCroppedCameraX = posCroppedCameraX + prePosCroppedCameraX - prePrePosCroppedCameraX;
						posCroppedCameraY = posCroppedCameraY + prePosCroppedCameraY - prePrePosCroppedCameraY;
					}

					calibrate = false;

					//if search area x2 value is greater than the width (hit the right wall) then limit
					//its value so it does not cause an error
					if (posCroppedCameraX + croppedImage.cols + shiftCropped * 2 > cv_ptr->image.cols) {
						posCroppedCameraX = cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 1;
						hitRightWall = true;
					}

					//if search area y2 value is greater than the height (hit the down wall) then limit
					//its value so it does not cause an error
					if (posCroppedCameraY + croppedImage.rows + shiftCropped * 2 > cv_ptr->image.rows) {
						posCroppedCameraY = cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 1;
						hitDownWall = true;
					}

					//if hit left wall stop from causing an error
					if (posCroppedCameraX < 0) {
						posCroppedCameraX = 0;
						hitLeftWall = true;
					}

					//if hit top wall from from causing an error
					if (posCroppedCameraY < 0) {
						posCroppedCameraY = 0;
						hitUpWall = true;
					}
					
					if ((posCroppedCameraX > shiftCropped) && (hitLeftWall == true)) {
						posCroppedCameraX = 0;
						prePosCroppedCameraX = shiftCropped;
						matchLocMoving.x = shiftCropped;
						hitLeftWall = false;
					}
					
					if ((posCroppedCameraX < cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 51) && (hitRightWall == true)) {
						posCroppedCameraX = cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - shiftCropped - 1;
						prePosCroppedCameraX = cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 1;
						matchLocMoving.x = shiftCropped;
						hitRightWall = false;
					}

					if ((posCroppedCameraY > shiftCropped) && (hitUpWall == true)) {
						posCroppedCameraY = 0;
						prePosCroppedCameraY = shiftCropped;
						matchLocMoving.y = shiftCropped;
						hitUpWall = false;
					}

					if ((posCroppedCameraY < cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 51) && (hitDownWall == true)) {
						posCroppedCameraY = cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - shiftCropped - 1;
						prePosCroppedCameraY = cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 1;
						matchLocMoving.y = shiftCropped;
						hitDownWall = false;
					}

					PrevMatchLocX = matchLocMoving.x;
					PrevMatchLocY = matchLocMoving.y;	
				}

				x1 = posCroppedCameraX;
				y1 = posCroppedCameraY;

				if (x1 > cv_ptr->image.cols) {
					x1 = cv_ptr->image.cols;
					hitRightWall = true;
				}
				else if (x1 <= 0) {
					x1 = 0;
				}

				if (y1 > cv_ptr->image.rows) {
					y1 = cv_ptr->image.rows;
				}
				else if (y1 <= 0) {
					y1 = 0;
				}

				x2 = x1 + croppedImage.cols + shiftCropped * 2;
				y2 = y1 + croppedImage.rows + shiftCropped * 2;

				double xVelocity = ((double) (((FRAME_HEIGHT) / 2) - ((y1 + y2)  / 2))) / 500.0;
				double yVelocity = ((double) ((FRAME_WIDTH / 2) -  ((x1 + x2) / 2))) / 900.0;

				if (x2 > cv_ptr->image.cols) {
					x2 = cv_ptr->image.cols;
				} else if (x2 <= 0) {
					x2 = 0;
				}

				if (y2 > cv_ptr->image.rows) {
					y2 = cv_ptr->image.rows;
				} else if (y2 <= 0) {
					y2 = 0;
				}

				//draw blue rectangle around 
				rectangle(cv_ptr->image, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 2, 8, 0);
				double distance = sqrt(abs(((x1 + x2) / 2 - FRAME_WIDTH / 2)) * abs(((x1 + x2) / 2 - 
					FRAME_WIDTH / 2)) + abs((y1 + y2) / 2 - FRAME_HEIGHT) * abs((y1 + y2) / 2 - FRAME_HEIGHT));

				if (distance < 80) {
					xVelocity = 0;
					yVelocity = 0;
					cout << "Close to the center: zeroing velocities" << endl;
				}

				if (confidPercent > 0.5) {
					if (mouseControlMove == false) {
						controls.setMovement(xVelocity, yVelocity, 0);
						controls.sendMovement(nh_);
					}

					lastGoodxVel = xVelocity;
					lastGoodyVel = yVelocity;

					cout << "Flying xVel: " << xVelocity << "\tyVel: " << yVelocity << "\tconfidPercent: " << confidPercent << endl;
				
				} 
				else {
					if (lostCount > 0) {
						//controls.setMovement(lastGoodxVel, lastGoodyVel, 0);
						//controls.sendMovement(nh_);
					} else {
						cout << "last xvel: " << lastGoodxVel << "\tlast yvel: " << lastGoodyVel << "\tconfidence: " << confidPercent << endl;
						
						controls.setMovement(0, 0, 0);
						controls.sendMovement(nh_);
					}
					//cout << "Hovering"  << "\tconfidPercent: " << confidPercent << endl;

					geometry_msgs::Twist getTwistInfo = controls.getTwist();
					//cout << "Twist x: " << getTwistInfo.linear.x << "\tTwist y: " << 	//getTwistInfo.linear.y << "\tTwist z: " << getTwistInfo.linear.z << endl;
				}

				if (lost == true) {
					matchLocMoving.x = shiftCropped;
					matchLocMoving.y = shiftCropped;
					prePrePosCroppedCameraX = 0;
					prePrePosCroppedCameraY = 0;
					prePosCroppedCameraX = 0;
					prePosCroppedCameraY = 0;
				}

				lost = false;

				x1 = posCroppedCameraX + matchLocMoving.x;
				y1 = posCroppedCameraY + matchLocMoving.y;

				if (x1 > cv_ptr->image.cols) {
					x1 = cv_ptr->image.cols;
				} else if (x1 <= 0) {
					x1 = 0;
				}

				if (y1 > cv_ptr->image.rows) {
					y1 = cv_ptr->image.rows;
				} else if (y1 <= 0) {
					y1 = 0;
				}

				x2 = x1 + croppedImage.cols;
				y2 = y1 + croppedImage.rows;

				if (x2 > cv_ptr->image.cols) {
					x2 = cv_ptr->image.cols;
				} else if (x2 <= 0) {
					x2 = 0;
				} 

				if (y2 > cv_ptr->image.rows) {
					y2 = cv_ptr->image.rows;
				} else if (y2 <= 0) {
					y2 = 0;
				}

				double velocity = 0.1;
				
				angle = atan2((y1 + (y2 - y1) / 2 - FRAME_HEIGHT / 2), 
					(x1 + (x2 - x1) / 2 - FRAME_WIDTH / 2)) / 3.14159265 * 180;

				//draw green rectangle around object
				if (useFullCamera == false) {
					rectangle(cv_ptr->image, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 8, 0);
					line(cv_ptr->image, Point(x1 + (x2 - x1) / 2, y1 + (y2 - y1) / 2), 						
					Point(FRAME_WIDTH / 2,(FRAME_HEIGHT) / 2), Scalar(0, 255, 0), 3, 1, 0);
				}

				circle(resultMoving, matchLocMoving, 15, Scalar(255, 255, 255), 2, 8, 0);
				
				if ((!(resultMoving.empty())) && useFullCamera == false) {
					imshow("Results Moving", resultMoving);
				}
				if (!(croppedImage.empty())) {
					imshow("Search Images", croppedImage);
				}
			}

			if (copyImage == true) {
				cameraFeed = cv_ptr->image.clone();
				copyImage = false;
			}

			// Update GUI Window
			if (freezeFrame == false) {
		    		cv::imshow("Drone Camera", cv_ptr->image);
			}
		    cv::waitKey(3);
		    
		    // Output modified video stream
		    image_pub_.publish(cv_ptr->toImageMsg());
		} catch (...) {
			posCroppedCameraX = prePosCroppedCameraX;
			posCroppedCameraY = prePosCroppedCameraY;
		}
	}
};

void ImageConverter::setMouseLeftClickDown(int x1, int y1) {
	cout << "Mouse Down (" << x1 << ", " << y1 << ")" << endl;
	Rectanglex1 = x1;
	Rectangley1 = y1;
	mouseUp = false;
	searchImg = false;
}


void ImageConverter::setMouseLeftRelease(int x2, int y2) {
	cout << "Mouse Up (" << x2 << ", " << y2 << ")" << endl;
	Rectanglex2 = x2;
	Rectangley2 = y2;
	mouseUp = true;
	freezeFrame = false;
}

Mat ImageConverter::getCameraFeed() {
	return cv_ptr->image;
}


Mat ImageConverter::getMatchingResults() {
	return resultMoving;
}

void ImageConverter::setLost(bool lostSetTo) {
	lost = lostSetTo;
}

int main(int argc2, char** argv2)
{
	int keyPressed;
	int inputLoop = 0;	
	
	argc = argc2;
	argv = argv2;

	controls.Init("-1", 0, argc, argv);
	
	ros::init(argc, argv, "image_converter");
	
	ros::NodeHandle node;

	controls.nav_sub = node.subscribe("/ardrone/navdata", 1, &nav_callback);
	controls.pubLand = node.advertise<std_msgs::Empty>("/ardrone/land", 1);
	controls.pubReset = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);
	controls.pubTakeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
	controls.pubTwist = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	
	ImageConverter ic(node);

  	ros::spin();

	return 0;

}

