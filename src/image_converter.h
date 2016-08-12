#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include "Header1.h"
#include "qt/controller.h"
using namespace std;
using namespace cv;


void CallBackFunc(int event, int x, int y, int flags, void* param);

class ImageConverter {
	ros::NodeHandle nh_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
	
public:
	ImageConverter(ros::NodeHandle node) { 
		nh_ = node;
		image_transport::ImageTransport it_(nh_);
	
		// Subscribe to input video feed and publish output video feed
		image_sub_ = it_.subscribe("/ardrone/image_raw", 1,
			&ImageConverter::imageCb, this);
		image_pub_ = it_.advertise("/image_converter/output_video", 1);
		cv::namedWindow("Drone Camera");
	}

	~ImageConverter() {
		cv::destroyWindow("Drone Camera");
	}

	void setMouseLeftClickDown(int Rectanglex1, int Rectangley1);
	void setMouseLeftRelease(int Rectanglex2, int Rectangley2);
	void setLost(bool lostSetTo);
	Mat getCroppedImage();
	Mat getCameraFeed();
	Mat getMatchingResults();
	void imageCb(const sensor_msgs::ImageConstPtr& msg);
};

#endif
