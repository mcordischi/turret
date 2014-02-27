#include <opencv/highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	SurfFeatureDetector detector( 400 );    
    Mat frame;
    namedWindow("video", 1);
    VideoCapture cap;
    int r = cap.open("http://192.168.1.200/videostream.cgi?user=admin&pwd=31415LAS&resolution=32&dummy=.mjpg");
    while ( cap.isOpened() )
    {
        cap >> frame;
        if(frame.empty()) break;
        
		std::vector<KeyPoint> frame_keypoints;

		detector.detect( frame, frame_keypoints );

		//-- Draw keypoints
		Mat img_frame_keypoints;

		drawKeypoints( frame, frame_keypoints, img_frame_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT );


        imshow("video", img_frame_keypoints);
        if(waitKey(30) >= 0) break;
    }
	
    return r;
}

