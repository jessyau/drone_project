
#include "Header1.h"

#include "Source1.cpp"
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
Mat gray;
Mat tmpImage;
cv::Mat croppedImage;
cv::Mat croppedCamera;
cv::Rect myROI;
cv::Rect croppedCameraROI;
Mat resultOriginal;
Mat resultMoving;
Mat binary_image;
Mat movement;
Mat imageb;
Mat imageF;
Point PrevMatchLoc;
Point posCroppedCamera;
Point prePosCroppedCamera;
Point prePrePosCroppedCamera;
int shiftCropped = 30;
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
Point matchLocMoving;
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
int FRAME_HEIGHT = 480;
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
//This function uses the mouse clicks to setup the search image
void CallBackFunc(int event, int x, int y, int flags, void* param) {
//Where the mouses' left button is clicked store the x, and y points into the rectangle variables
	if (event == EVENT_LBUTTONDOWN){
		cout << "Mouse Down (" << x << ", " << y << ")" << endl;
		Rectanglex1 = x;
		Rectangley1 = y;
		mouseUp = false;
		searchImg = false;
//When the left button is released store the x, and y position into rectangle 2
	} else if (event == EVENT_LBUTTONUP) {
		cout << "Mouse Up (" << x << ", " << y << ")" << endl;
		Rectanglex2 = x;
		Rectangley2 = y;
		mouseUp = true;
		freezeFrame = false;
//asdf
//this is just for testing
//copyImage = true;
	} else if (event == EVENT_RBUTTONUP) {
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
	}
}
class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
public:
	ImageConverter()
	: it_(nh_)
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
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		double distance = 0;
		bool matchingSuccessful = false;
		Point centerOfFoundImage;
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}
		linebuffered(false);
		echoController(false);
		if (iskeypressed(1) && cin.get() == 'j') {
			cout << "Left" << endl;
//controls.setMovement(0, -1, 0);
//controls.sendMovement(nh_);
		} else if (iskeypressed(1) && cin.get() == 'l') {
			cout << "Right" << endl;
//controls.setMovement(0, 1, 0);
//controls.sendMovement(nh_);
		} else if (iskeypressed(1) && cin.get() == 'k') {
			cout << "Up" << endl;
//controls.setMovement(1, 0, 0);
//controls.sendMovement(nh_);
		} else if (iskeypressed(1) && cin.get() == 'i') {
			cout << "Down" << endl;
//controls.setMovement(-1, 0, 0);
//controls.sendMovement(nh_);
		} else if (iskeypressed(1) && cin.get() == '\n') {
			cout << "Hovering" << endl;
//controls.setMovement(0, 0, 0);
//controls.sendMovement(nh_);
		}
		controls.nav_sub = nh_.subscribe("/ardrone/navdata", 1, &nav_callback);
		if (searchImg == false) {
			if (numLoops == 0) {
//controls.pubLand = nh_.advertise<std_msgs::Empty>("/ardrone/land", 1);
//controls.pubReset = nh_.advertise<std_msgs::Empty>("/ardrone/reset", 1);
//controls.pubTakeoff = nh_.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
//controls.pubTwist = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
			}
			ros::Rate loop_rate(100);
			if (numLoops == 5) {
//controls.sendTakeoff(nh_);
				cout << "TakeOff" << endl;
				cout << "inFlight: " << controls.inFlight << "\n";
				cout << "Battery: " << controls.getBattery() << endl;
			} else if ((numLoops > 170) && (controls.getAltitude() < 1)) {
//controls.gainAltitude(1, nh_);
				cout << "Resetting\tnumLoops: " << numLoops << endl;
				cout << "inFlight: " << controls.inFlight << "\n";
				cout << "Altitude: " << controls.getAltitude() << endl;
			} else if (numLoops >= 170) {
//after rising, keep position at 0,0,0
//controls.setMovement(0, 0, 0);
//controls.sendMovement(nh_);
			}
		}
		numLoops = numLoops + 1;
		if (numLoops > 2000) {
			numLoops = 2000;
		}
		try {
			if (land == true) {
//controls.resetTwist();
//controls.setMovement(0, 0, 0);
//controls.sendMovement(nh_);
//controls.sendLand(nh_);
				cout << "Sent Land\n";
				cout << "inFlight: " << controls.inFlight << "\n";
				land = false;
			}
//set a callback duty for any mouse event
			setMouseCallback("Drone Camera", CallBackFunc, NULL);
			linebuffered(false);
			echoController(false);
			if (mouseUp == true) {
				croppedImage = captureCroppedImage();
				cout << "x1 y1 (" << Rectanglex1 << ", " << Rectangley1 << ")" << endl;
				cout << "x2 y2 (" << Rectanglex2 << ", " << Rectangley2 << ")" << endl;
				mouseUp = false;
				searchImg = true;
			}
			if (searchImg == true) {
//
/// Create the result matrix
				int resultOriginal_cols = cv_ptr->image.cols - croppedImage.cols + 1;
				int resultOriginal_rows = cv_ptr->image.rows - croppedImage.rows + 1;
				resultOriginal.create(resultOriginal_cols, resultOriginal_rows, CV_32FC1);
				FRAME_WIDTH = cv_ptr->image.cols;
				FRAME_HEIGHT = cv_ptr->image.rows;
//cout << "confidPercent: " << confidPercent << endl;
				if ((confidPercent < 0.5) && (lostCount <= 0)){
					lost = true;
					lostCount = 10;
					cout << "Lost!!!!!" << endl;
				}
				if (lost == true) {
					if (croppedImage.type() == CV_8U) {
						cvtColor(croppedImage, croppedImage, CV_GRAY2RGB);
					}
					useFullCamera = true;
					matchingSuccessful = doTemplateMatching(cv_ptr->image);
				}
				else {
					getCroppedCamera(cv_ptr->image);
/*
Point bottomRightPoint;
bottomRightPoint.x = PrevMatchLoc.x + croppedImage.cols;
bottomRightPoint.y = PrevMatchLoc.y + croppedImage.rows;
bottomRightPoint = checkPointWithinBounds(bottomRightPoint, cv_ptr->image);
*/
/// Do the Matching and Normalize
useFullCamera = false;
matchingSuccessful = doTemplateMatching(croppedCamera);
}
minMaxLoc(resultMoving, &minValMoving, &maxValMoving, &minLocMoving, &maxLocMoving, Mat());
confidPercent = maxValMoving;
/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
matchLocMoving = maxLocMoving;
cout << "maxLocMoving2.x: " << maxLocMoving.x << "\tmaxLocMoving2.y: " << maxLocMoving.y << "\tconfidPercent: " << confidPercent << endl;
lostCount = lostCount - 1;
if (lostCount < 0) {
	lostCount = 0;
}
if (lost == true) {
	resetValues();
}
else {
	calcNewPosition();
	checkHitWall(cv_ptr->image);
	hitLeftWall = checkBouncedBackLeft(hitLeftWall);
	hitRightWall = checkBouncedBackRight(hitRightWall, cv_ptr->image);
	hitUpWall = checkBouncedBackUp(hitUpWall);
	hitDownWall = checkBouncedBackDown(hitDownWall, cv_ptr->image);
//store the new 'shift'
	PrevMatchLoc = matchLocMoving;
}
lost = false;
if (matchingSuccessful == true) {
	drawBlueRectangle(cv_ptr->image);
}
//draw green rectangle around object
if (matchingSuccessful == true) {
	centerOfFoundImage = drawGreenRectangle(cv_ptr->image);
} else {
	centerOfFoundImage.x = FRAME_WIDTH / 2;
	centerOfFoundImage.y = FRAME_HEIGHT / 2;
}
double xVelocity = ((double) (((FRAME_HEIGHT / 2) - centerOfFoundImage.y / 2))) / 100.0;
double yVelocity = ((double) (((FRAME_WIDTH / 2) - centerOfFoundImage.x / 2))) / 200.0;
double distance = sqrt(abs(centerOfFoundImage.x - FRAME_WIDTH / 2) * abs(centerOfFoundImage.x - FRAME_WIDTH / 2) + abs(centerOfFoundImage.y - FRAME_HEIGHT) * abs(centerOfFoundImage.y - FRAME_HEIGHT));
//testing purposes
//yVelocity = 0;
if (distance < 10) {
	xVelocity = 0;
	yVelocity = 0;
	cout << "Close to the center: zeroing velocities" << endl;
}
if (confidPercent > 0.5) {
//controls.setMovement(xVelocity, yVelocity, 0);
//controls.sendMovement(nh_);
	lastGoodxVel = xVelocity;
	lastGoodyVel = yVelocity;
//cout << "Flying xVel: " << xVelocity << "\tyVel: " << yVelocity << // "\tconfidPercent: " << confidPercent << endl;
} else {
	if (lostCount > 5) {
//controls.setMovement(lastGoodxVel, lastGoodyVel, 0);
//controls.sendMovement(nh_);
	}
//cout << "Lost, last xvel: " << lastGoodxVel << "\tlast yvel: " << lastGoodyVel << // endl;
//controls.setMovement(0, 0, 0);
//controls.sendMovement(nh_);
//cout << "Hovering" << "\tconfidPercent: " << confidPercent << endl;
	geometry_msgs::Twist getTwistInfo = controls.getTwist();
//cout << "Twist x: " << getTwistInfo.linear.x << "\tTwist y: " << //getTwistInfo.linear.y << "\tTwist z: " << getTwistInfo.linear.z << endl;
}
/*
cout << "\t angle: " << angle << "\tdistsance: " << distance << endl;
*/
circle(resultMoving, matchLocMoving, 15, Scalar(255, 255, 255), 2, 8, 0);
if ((!(resultMoving.empty())) && (lost == false)) {
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
echoController();
linebuffered();
}
catch (...) {
	posCroppedCamera.x = prePosCroppedCamera.x;
	posCroppedCamera.y = prePosCroppedCamera.y;
}
}
//capture cropped image
Mat captureCroppedImage() {
	Mat getCroppedImage;
	int tempRectangle;
	tempRectangle = minimum(Rectanglex1, Rectanglex2);
	Rectanglex2 = maximum(Rectanglex1, Rectanglex2);
	Rectanglex1 = tempRectangle;
	tempRectangle = minimum(Rectangley1, Rectangley2);
	Rectangley2 = maximum(Rectangley1, Rectangley2);
	Rectangley1 = tempRectangle;
	myROI = Rect(Rectanglex1, Rectangley1, Rectanglex2 - Rectanglex1, Rectangley2 - Rectangley1);
	Mat imgPanelRoi(cameraFeed, myROI);
	imgPanelRoi.copyTo(getCroppedImage);
	return getCroppedImage;
}
int minimum(int value1, int value2) {
	if (value1 < value2) {
		return value1;
	} else if (value2 < value1) {
		return value2;
	}
	return value1;
}
int maximum(int value1, int value2) {
	if (value1 > value2) {
		return value1;
	} else if (value2 > value1) {
		return value2;
	}
	return value2;
}
void calcNewPosition() {
//this is to keep it stable when tracking an object
//store 2nd previous camera position in prePrePosCroppedCamera
	prePrePosCroppedCamera.x = prePosCroppedCamera.x;
	prePrePosCroppedCamera.y = prePosCroppedCamera.y;
//calculate previous position
	prePosCroppedCamera.x = matchLocMoving.x + posCroppedCamera.x;
	prePosCroppedCamera.y = matchLocMoving.y + posCroppedCamera.y;
//if not the first time time finding image, then
	if (calibrate == false) {
		posCroppedCamera.x = posCroppedCamera.x + prePosCroppedCamera.x - prePrePosCroppedCamera.x;
		posCroppedCamera.y = posCroppedCamera.y + prePosCroppedCamera.y - prePrePosCroppedCamera.y;
	}
	calibrate = false;
}
void checkHitWall(Mat hitWallImage) {
//if search area x2 value is greater than the width (hit the right wall) then limit
//its value so it does not cause an error
	if (posCroppedCamera.x + croppedImage.cols + shiftCropped * 2 > hitWallImage.cols) {
		posCroppedCamera.x = hitWallImage.cols - (croppedImage.cols + shiftCropped * 2) - 1;
		hitRightWall = true;
	}
//if search area y2 value is greater than the height (hit the down wall) then limit
//its value so it does not cause an error
	if (posCroppedCamera.y + croppedImage.rows + shiftCropped * 2 > hitWallImage.rows) {
		posCroppedCamera.y = hitWallImage.rows - (croppedImage.rows + shiftCropped * 2) - 1;
		hitDownWall = true;
	}
//if hit left wall stop from causing an error
	if (posCroppedCamera.x < 0) {
		posCroppedCamera.x = 0;
		hitLeftWall = true;
	}
//if hit top wall from from causing an error
	if (posCroppedCamera.y < 0) {
		posCroppedCamera.y = 0;
		hitUpWall = true;
	}
}
bool checkBouncedBackLeft(bool hitLeftWall) {
	if ((posCroppedCamera.x > shiftCropped) && (hitLeftWall == true)) {
		posCroppedCamera.x = 0;
		prePosCroppedCamera.x = shiftCropped;
		matchLocMoving.x = shiftCropped;
		return false;
	}
	return hitLeftWall;
}
bool checkBouncedBackRight(bool hitRightWall, Mat checkImage) {
	if ((posCroppedCamera.x < checkImage.cols - (croppedImage.cols + shiftCropped * 2) - 51) &&
		(hitRightWall == true)) {
		posCroppedCamera.x = checkImage.cols - (croppedImage.cols + shiftCropped * 2) - shiftCropped - 1;
	prePosCroppedCamera.x = checkImage.cols - (croppedImage.cols + shiftCropped * 2) - 1;
	matchLocMoving.x = shiftCropped;
	return false;
}
return hitRightWall;
}
bool checkBouncedBackUp(bool hitUpWall) {
	if ((posCroppedCamera.y > shiftCropped) && (hitUpWall == true)) {
		posCroppedCamera.y = 0;
		prePosCroppedCamera.y = shiftCropped;
		matchLocMoving.y = shiftCropped;
		return false;
	}
	return hitUpWall;
}
bool checkBouncedBackDown(bool hitDownWall, Mat checkImage) {
	if ((posCroppedCamera.y < checkImage.rows - (croppedImage.rows + shiftCropped * 2) - shiftCropped - 1) && (hitDownWall == true)) {
		posCroppedCamera.y = checkImage.rows - (croppedImage.rows + shiftCropped * 2) - shiftCropped - 1;
		prePosCroppedCamera.y = checkImage.rows - (croppedImage.rows + shiftCropped * 2) - 1;
		matchLocMoving.y = shiftCropped;
		return false;
	}
	return hitDownWall;
}
void drawBlueRectangle(Mat imageDrawBlue) {
	Point firstPoint;
	Point secondPoint;
	firstPoint = posCroppedCamera;
	firstPoint = checkPointWithinBounds(firstPoint, imageDrawBlue);
	secondPoint.x = firstPoint.x + croppedImage.cols + shiftCropped * 2;
	secondPoint.y = firstPoint.y + croppedImage.rows + shiftCropped * 2;
	secondPoint = checkPointWithinBounds(secondPoint, imageDrawBlue);
//draw blue rectangle around
	rectangle(imageDrawBlue, firstPoint, secondPoint, Scalar(255, 0, 0), 2, 8, 0);
}
Point checkPointWithinBounds(Point checkPoint, Mat sizeImage) {
	if (checkPoint.x > sizeImage.cols) {
		checkPoint.x = sizeImage.cols;
	}
	else if (checkPoint.x <= 0) {
		checkPoint.x = 0;
	}
	if (checkPoint.y > sizeImage.rows) {
		checkPoint.y = sizeImage.rows;
	}
	else if (checkPoint.y <= 0) {
		checkPoint.y = 0;
	}
	return checkPoint;
}
Point drawGreenRectangle(Mat imageDrawGreen) {
	Point firstCorner;
	Point secondCorner;
	Point center;
	firstCorner.x = posCroppedCamera.x + matchLocMoving.x;
	firstCorner.y = posCroppedCamera.y + matchLocMoving.y;
	firstCorner = checkPointWithinBounds(firstCorner, imageDrawGreen);
	secondCorner.x = firstCorner.x + croppedImage.cols;
	secondCorner.y = firstCorner.y + croppedImage.rows;
	secondCorner = checkPointWithinBounds(secondCorner, imageDrawGreen);
	center.x = firstCorner.x + (secondCorner.x - firstCorner.x) / 2;
	center.y = firstCorner.y + (secondCorner.y - firstCorner.y) / 2;
	rectangle(imageDrawGreen, firstCorner, secondCorner, Scalar(0, 255, 0), 2, 8, 0);
	line(imageDrawGreen, center, Point(imageDrawGreen.cols / 2,(imageDrawGreen.rows) / 2), Scalar(0, 255, 0), 3, 1, 0);
	return center;
}
void resetValues() {
	cout << "matchLocMoving.x: " << matchLocMoving.x << "\tmatchLocMoving.y: " << matchLocMoving.y << endl;
	posCroppedCamera.x = matchLocMoving.x - shiftCropped;
	posCroppedCamera.y = matchLocMoving.y - shiftCropped;
	calibrate = true;
	hitLeftWall = false;
	hitRightWall = false;
	hitUpWall = false;
	hitDownWall = false;
//matchLocMoving.x = shiftCropped;
//matchLocMoving.y = shiftCropped;
//prePrePosCroppedCamera.x = 0;
//prePrePosCroppedCamera.y = 0;
//prePosCroppedCamera.x = 0;
//prePosCroppedCamera.y = 0;
}
//get cropped camera (area to search, surrounded by blue rectangle)
void getCroppedCamera(Mat fullImage) {
	int width = croppedImage.cols + shiftCropped * 2;
	int height = croppedImage.rows + shiftCropped * 2;
	Point topLeftPoint;
	topLeftPoint = checkPointWithinBounds(posCroppedCamera, fullImage);
	if ((topLeftPoint.x + width < fullImage.cols) && (topLeftPoint.y + height < fullImage.rows)) {
		croppedCameraROI = Rect(topLeftPoint.x, topLeftPoint.y, width, height);
		Mat imgPanelRoi(fullImage, croppedCameraROI);
		imgPanelRoi.copyTo(croppedCamera);
	}
}
bool doTemplateMatching(Mat largeImage) {
	if ((!(croppedCamera.empty())) && (croppedImage.cols < largeImage.cols) && (croppedImage.rows < largeImage.rows)) {
		matchTemplate(largeImage, croppedImage, resultMoving, CV_TM_CCOEFF_NORMED);
		return true;
	} else {
		return false;
	}
	return false;
}
/*
Mat getVideoFeed() {
return cv_ptr->image;
}
*/
Mat getTemplateResults() {
	return resultMoving;
}
};
int main(int argc2, char** argv2)
{
	argc = argc2;
	argv = argv2;
	if (doOnce == false) {
		multiplyTwoNumbers(5, 2);
		doOnce = true;
		controls.Init("-1", 0, argc, argv);
		ros::init(argc, argv, "image_converter");
	}
	if (!initialize())
	{
		return 1;
	}
// try {
// linebuffered( false );
// echo( false );
// while (true)
// {
// cout << "Zzz..." << flush;
// if (iskeypressed( 500 ) && cin.get() == '\n') break;
// }
// echo();
// linebuffered();
// }
// catch (...) { }
//ros::NodeHandle node;
	ImageConverter ic;
	ros::spin();
	return 0;
}



