//object recognition for AR Drone


#include "stdafx.h"
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <opencv\highgui.h>
#include <opencv\cv.h>

//This constant is for supressing a warning message
#define _CRT_SECURE_NO_WARNINGS

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


/*
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
*/

//This function uses the mouse clicks to setup the search image
void CallBackFunc(int event, int x, int y, int flags, void* userdata) {

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
	}
}



//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;

//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";


int main(int argc, char* argv[])
{
	//some boolean variables for different functionality within this
	//program
	bool trackObjects = false;
	bool useMorphOps = false;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat cameraFeedCropped;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	


	Mat gray;
	Mat tmpImage;
	vector<Vec3f> circles;



	int x = 0, y = 0;
	//create slider bars for HSV filtering
	
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop

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

	while (1){
		//store image to matrix
		capture.read(cameraFeed);

		

		//subtract(cameraFeedPrevious, movement, cameraFeed);

		//cameraFeed = cameraFeed > 1;

		//imshow("current", imageF);
		//imshow("movement", cameraFeed);
		//imshow("previous", movement);







		//set a callback duty for any rodent event
		setMouseCallback(windowName, CallBackFunc, NULL);

		if (mouseUp == true) {
			myROI = Rect(Rectanglex1, Rectangley1, abs(Rectanglex2 - Rectanglex1), abs(Rectangley2 - Rectangley1));


			Mat imgPanelRoi(cameraFeed, myROI);

			imgPanelRoi.copyTo(croppedImage);


			mouseUp = false;
			searchImg = true;

			//PrevMatchLocX = Rectanglex1;
			//PrevMatchLocY = Rectangley1;

		}

		if (searchImg == true) {
	
			//

			

			/// Create the result matrix
			int resultOriginal_cols = cameraFeed.cols - croppedImage.cols + 1;
			int resultOriginal_rows = cameraFeed.rows - croppedImage.rows + 1;

			resultOriginal.create(resultOriginal_cols, resultOriginal_rows, CV_32FC1);




			
			if (lost == true) {
				/// Do the Matching and Normalize
				matchTemplate(cameraFeed, croppedImage, resultMoving, CV_TM_SQDIFF_NORMED);

				
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
				Mat imgPanelRoi(cameraFeed, croppedCameraROI);
				imgPanelRoi.copyTo(croppedCamera);

				
				//make sure the 'search area' does not exceded the cameras image
				x1 = PrevMatchLocX;
				y1 = PrevMatchLocY;
				x2 = x1 + croppedImage.cols;
				y2 = y1 + croppedImage.rows;
				if (x2 > cameraFeed.cols) {
					x2 = cameraFeed.cols;
				}

				if (y2 > cameraFeed.rows) {
					y2 = cameraFeed.rows;
				}

				
				
				imshow("Search Cropped Image", croppedCamera);

				/// Do the Matching and Normalize
				matchTemplate(croppedCamera, croppedImage, resultMoving, CV_TM_SQDIFF_NORMED);

				rectangle(croppedCamera, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 8, 0);

			}

			/// Localizing the best match with minMaxLoc
			double minValMoving; double maxValMoving; Point minLocMoving; Point maxLocMoving;
			Point matchLocMoving;
			
			minMaxLoc(resultMoving, &minValMoving, &maxValMoving, &minLocMoving, &maxLocMoving, Mat());


			cout << "Level of certainity: " << minValMoving;



			/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
			
			matchLocMoving = minLocMoving;



			cout << "\t      matchLocMoving.x: " << matchLocMoving.x << endl;
			cout << "\t     OriginalMatchLocMovingX: " << OriginalMatchLocMovingX << endl;
			cout << "\t      matchLocMoving.y: " << matchLocMoving.y << endl;

			distance = PrevMatchLocX - matchLocMoving.x;
			//distanceY = PrevMatchLocY - matchLocMoving.y;

			cout << "\t      distance: " << distance << endl;


				
				if (lost == true) {
					posCroppedCameraX = matchLocMoving.x - shiftCropped;
					posCroppedCameraY = matchLocMoving.y - shiftCropped;
					cout << "\t      first time, matchLovMovingX: " << matchLocMoving.x << endl;
					lost = false;
					calibrate = true;

					//store original matchLocMoving values to be used as a reference
					OriginalMatchLocMovingX = matchLocMoving.x;
					OriginalMatchLocMovingY = matchLocMoving.y;

					
				}
				else {

					//this is to keep it stable when tracking an object
					//store previous camera position in prePrePosCroppedCamera
					prePrePosCroppedCameraX = prePosCroppedCameraX;
					prePrePosCroppedCameraY = prePosCroppedCameraY;

					//calculate previous position
					prePosCroppedCameraX = matchLocMoving.x + posCroppedCameraX;
					prePosCroppedCameraY = matchLocMoving.y + posCroppedCameraY;

					if (calibrate == false) {
						posCroppedCameraX = posCroppedCameraX + prePosCroppedCameraX - prePrePosCroppedCameraX;
						posCroppedCameraY = posCroppedCameraY + prePosCroppedCameraY - prePrePosCroppedCameraY;
						
					} else {

					}
					calibrate = false;

					if (posCroppedCameraX + croppedImage.cols + shiftCropped * 2 > cameraFeed.cols) {
						posCroppedCameraX = cameraFeed.cols - (croppedImage.cols + shiftCropped * 2) - 1;

					}
					if (posCroppedCameraY + croppedImage.rows + shiftCropped * 2 > cameraFeed.rows) {
						posCroppedCameraY = cameraFeed.rows - (croppedImage.rows + shiftCropped * 2) - 1;
					}
					if (posCroppedCameraX < 0) {
						posCroppedCameraX = 0;
						calibrate = true;
					}
					if (posCroppedCameraY < 0) {
						posCroppedCameraY = 0;
						
					}


					double testing = (abs(Rectanglex2 - Rectanglex1) + shiftCropped * 2) / 2;


					cout << "\t      prePrePosCroppedCameraX: " << prePrePosCroppedCameraX << endl;
					cout << "\t      prePosCroppedCameraX: " << prePosCroppedCameraX << endl;
					cout << "\t      posCroppedCameraX: " << posCroppedCameraX << endl;


					PrevMatchLocX = matchLocMoving.x;
					PrevMatchLocY = matchLocMoving.y;

					
				}
				
				if (minValMoving > 0.3) {
					//lost = true;
				}
				

				x1 = posCroppedCameraX;
				y1 = posCroppedCameraY;


				if (x1 > cameraFeed.cols) {
					x1 = cameraFeed.cols;
				}
				else if (x1 <= 0) {
					x1 = 0;
				}


				if (y1 > cameraFeed.rows) {
					y1 = cameraFeed.rows;
				}
				else if (y1 <= 0) {
					y1 = 0;
				}

				x2 = x1 + croppedImage.cols + shiftCropped * 2;
				y2 = y1 + croppedImage.rows + shiftCropped * 2;

				if (x2 > cameraFeed.cols) {
					x2 = cameraFeed.cols;
				} else if (x2 <= 0) {
					x2 = 0;
				}

				if (y2 > cameraFeed.rows) {
					y2 = cameraFeed.rows;
				} else if (y2 <= 0) {
					y2 = 0;
				}


				//draw blue rectangle around 
				rectangle(cameraFeed, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 2, 8, 0);

				//if (matchLocMoving.x < OriginalMatchLocMovingX) {
					
					
				//}


				x1 = posCroppedCameraX + matchLocMoving.x;
				y1 = posCroppedCameraY + matchLocMoving.y;




				if (x1 > cameraFeed.cols) {
					x1 = cameraFeed.cols;
				} else if (x1 <= 0) {
					x1 = 0;
				}
				if (y1 > cameraFeed.rows) {
					y1 = cameraFeed.rows;
				} else if (y1 <= 0) {
					y1 = 0;
				}

				x2 = x1 + croppedImage.cols;
				y2 = y1 + croppedImage.rows;

				if (x2 > cameraFeed.cols) {
					x2 = cameraFeed.cols;
				} else if (x2 <= 0) {
					x2 = 0;
				} 

				if (y2 > cameraFeed.rows) {
					y2 = cameraFeed.rows;
				} else if (y2 <= 0) {
					y2 = 0;
				}
				
				

				//draw green rectangle around object
				rectangle(cameraFeed, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 8, 0);


			circle(resultMoving, matchLocMoving, 15, Scalar(255, 255, 255), 2, 8, 0);
			


			imshow("Results Moving", resultMoving);

			imshow("Search Images", croppedImage);
			


		}

		//show frames 
		//imshow(windowName2, threshold);

		imshow(windowName, cameraFeed);


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}






	return 0;
}
