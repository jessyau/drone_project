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


using namespace std;
using namespace cv;



//This rectangle is the for the area selected using the mouse
int Rectanglex1;
int Rectangley1;
int Rectanglex2;
int Rectangley2;

//Boolean variable to search only after an image has been selecte
bool searchImg = false;
bool mouseUp;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//This function uses the mouse clicks to setup the search image
void CallBackFunc(int event, int x, int y, int flags, void* userdata) {

	//Where the mouses' left button is clicked store the x, and y points into the rectangle 	variables
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
	double PrevMatchLocX = 0;
	double PrevMatchLocY = 0;

	int posCroppedCameraX = 0;
	int posCroppedCameraY = 0;
	int shiftCropped = 50;
	int prePosCroppedCameraX = 0;
	int prePosCroppedCameraY = 0;
	int prePrePosCroppedCameraX = 0;
	int prePrePosCroppedCameraY = 0;

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;

	int OriginalMatchLocMovingX = 0;
	int OriginalMatchLocMovingY = 0;

	bool lost = true;
	bool freezeMoveImage = false;
	bool firstCapture = true;
	double distance = 0;

	bool calibrate = true;

	int changeInX = 0;
	int changeInY = 0;

	bool hitLeftWall = false;
	bool hitRightWall = false;
	bool hitUpWall = false;
	bool hitDownWall = false;

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

	//set a callback duty for any rodent event
	setMouseCallback("Drone Camera", CallBackFunc, NULL);

	
	if (mouseUp == true) {

		cout << "x1 y1 (" << Rectanglex1 << ", " << Rectangley1 << ")" << endl;
		cout << "x2 y2 (" << Rectanglex2 << ", " << Rectangley2 << ")" << endl;		

		myROI = Rect(Rectanglex1, Rectangley1, abs(Rectanglex2 - Rectanglex1), 
						       abs(Rectangley2 - Rectangley1));


		Mat imgPanelRoi(cv_ptr->image, myROI);

		imgPanelRoi.copyTo(croppedImage);


		mouseUp = false;
		searchImg = true;
		


	}
	


/*
		if (searchImg == true) {
	
			//

			

			/// Create the result matrix
			int resultOriginal_cols = cv_ptr->image.cols - croppedImage.cols + 1;
			int resultOriginal_rows = cv_ptr->image.rows - croppedImage.rows + 1;

			resultOriginal.create(resultOriginal_cols, resultOriginal_rows, CV_32FC1);




			
			if (lost == true) {
				
				int rowsCamera = cv_ptr->image.rows;
				int rowsCropped = croppedImage.rows;
		
				if (croppedImage.type() == CV_8U) {
					//convert from CV_8U to CV_8UC3
					//cv::Mat fixImage;
					//cvtColor(croppedImage,fixImage,CV_GRAY2RGB);
					cvtColor(croppedImage, croppedImage, CV_GRAY2RGB);
					//croppedImage = fixImage.clone();
				}
			
				matchTemplate(cv_ptr->image, croppedImage, resultMoving, CV_TM_SQDIFF_NORMED);
				 
				
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

				croppedCameraROI = Rect(x1, y1, width, height);
				Mat imgPanelRoi(cv_ptr->image, croppedCameraROI);
				imgPanelRoi.copyTo(croppedCamera);

				
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

				
				
				imshow("Search Cropped Image", croppedCamera);

				cout << "\t      testing testing: " << endl;

				/// Do the Matching and Normalize

				
				croppedCamera.convertTo(croppedCamera, CV_8UC3, 1.0/255.0);
				matchTemplate(croppedCamera, croppedImage, resultMoving, CV_TM_SQDIFF_NORMED);
				
				cout << "\t      testing1 testing1: " << endl;

				rectangle(croppedCamera, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 8, 0);

			}

			/// Localizing the best match with minMaxLoc
			double minValMoving; double maxValMoving; Point minLocMoving; Point maxLocMoving;
			Point matchLocMoving;
			
			minMaxLoc(resultMoving, &minValMoving, &maxValMoving, &minLocMoving, &maxLocMoving, Mat());


			/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
			
			matchLocMoving = minLocMoving;


			distance = PrevMatchLocX - matchLocMoving.x;
			//distanceY = PrevMatchLocY - matchLocMoving.y;

				
				if (lost == true) {
					posCroppedCameraX = matchLocMoving.x - shiftCropped;
					posCroppedCameraY = matchLocMoving.y - shiftCropped;

					lost = false;
					calibrate = true;

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

					
					if ((posCroppedCameraX > 50) && (hitLeftWall == true)) {
						posCroppedCameraX = 0;
						prePosCroppedCameraX = 50;
						matchLocMoving.x = 50;
						hitLeftWall = false;
					}
					
					if ((posCroppedCameraX < cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 51) && (hitRightWall == true)) {
						posCroppedCameraX = cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 51;
						prePosCroppedCameraX = cv_ptr->image.cols - (croppedImage.cols + shiftCropped * 2) - 1;
						matchLocMoving.x = 50;
						hitRightWall = false;
					}

					if ((posCroppedCameraY > 50) && (hitUpWall == true)) {
						posCroppedCameraY = 0;
						prePosCroppedCameraY = 50;
						matchLocMoving.y = 50;
						hitUpWall = false;
					}

					if ((posCroppedCameraY < cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 51) && (hitDownWall == true)) {
						//posCroppedCameraY = cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 51;
						//prePosCroppedCameraY = cv_ptr->image.rows - (croppedImage.rows + shiftCropped * 2) - 1;
						//matchLocMoving.y = 50;
						hitDownWall = false;
					}





					PrevMatchLocX = matchLocMoving.x;
					PrevMatchLocY = matchLocMoving.y;

					
				}
				
				if (minValMoving > 0.3) {
					//lost = true;
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

				//if (matchLocMoving.x < OriginalMatchLocMovingX) {
					
					
				//}


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
				
				double distance = sqrt(abs(x1 - FRAME_WIDTH / 2) * abs(x1 - FRAME_WIDTH / 2) + 
					abs(y1 - FRAME_HEIGHT) * abs(y1 - FRAME_HEIGHT));

				if ((x1 - FRAME_WIDTH / 2) != 0) {
					double angle = atan2((y1 + (y2 - y1) / 2 - FRAME_HEIGHT / 2), 
										 (x1 + (x2 - x1) / 2 - FRAME_WIDTH / 2)) / 3.15159265 * 180;
				
					cout << "\t      angle: " << angle << endl;

					//cout << "\t      x1: " << x1 << endl;
					//cout << "\t      y1: " << y1 << endl;

				}
				//draw green rectangle around object
				rectangle(cv_ptr->image, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 8, 0);
				line(cv_ptr->image, Point(x1 + (x2 - x1) / 2, y1 + (y2 - y1) / 2), Point(FRAME_WIDTH / 2, FRAME_HEIGHT / 2), Scalar(0, 255, 0), 3, 1, 0);

			circle(resultMoving, matchLocMoving, 15, Scalar(255, 255, 255), 2, 8, 0);
			


			imshow("Results Moving", resultMoving);

			imshow("Search Images", croppedImage);
			


		}
*/

    // Update GUI Window
    cv::imshow("Drone Camera", cv_ptr->image);
    cv::waitKey(3);
    
    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());


	
  }
};



int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
