#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "control.h"

using namespace cv;

void __inline__ adjust(Point2f center, int lx, int ly)
{
    static bool move_vertical;
    static int wait = 10;
	int offx = center.x - lx/2;
	int offy = center.y - ly/2;
	if(wait) { wait--; return; }
	if( (move_vertical || abs(offx) < 15) && abs(offy) > 15 )
	{
		move( (offx<0)?CAM_UP:CAM_DW , 10);
		wait = 5;
	}
	else if (abs(offx) > 15 )
	{
		move( (offx<0)?CAM_RH:CAM_LF , 10);
		wait = 5;
	}
	move_vertical != move_vertical;
}

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		std::cout << "Use: tracker <target_image>" << std::endl;
        return -1;
	}
	std::cout << "Load target: " << std::endl;
    Mat mTarget = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );

    if( !mTarget.data )
    {
        std::cout<< "Error reading target image." << std::endl;
        return -1;
    }
	std::cout << "\t\t\tDone" << std::endl;
    //Detect the keypoints using SURF Detector
    int minHessian = 500;
	
    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> kpTarget;
	
	std::cout << "Analize target image: " << std::endl;
    detector.detect( mTarget, kpTarget );

    //Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat des_object;

    extractor.compute( mTarget, kpTarget, des_object );
	std::cout << "\t\t\tDone" << std::endl;
	std::cout << "Creating macher: " << std::endl;
    FlannBasedMatcher matcher;
    std::cout << "\t\t\tDone" << std::endl;
	std::cout << "Init capture: " << std::endl;
    VideoCapture cap("http://192.168.1.200/videostream.cgi?user=admin&pwd=31415LAS&resolution=32&dummy=.mjpg");
	//VideoCapture cap("http://nidq.no-ip.org/videostream.cgi?user=admin&pwd=31415LAS&resolution=32&dummy=.mjpg");
	std::cout << "\t\t\tDone" << std::endl;
	
    namedWindow("Capture");

    std::vector<Point2f> tgt_corners(4);

    //Get the corners from the object
    tgt_corners[0] = cvPoint(0,0);
    tgt_corners[1] = cvPoint( mTarget.cols, 0 );
    tgt_corners[2] = cvPoint( mTarget.cols, mTarget.rows );
    tgt_corners[3] = cvPoint( 0, mTarget.rows );

    char key = 'a';
    while (key != 27)
    {
        Mat frame;
        
        for(int i = 0; i < 5; i++) cap >> frame;
        Mat show = frame.clone();
        Mat des_image, img_matches;
        std::vector<KeyPoint> kpImage;
        std::vector<vector<DMatch > > matches;
        std::vector<DMatch > good_matches;
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        std::vector<Point2f> scene_corners(4);
        Mat H;
        Mat image;

        cvtColor(frame, image, CV_RGB2GRAY);

        detector.detect( image, kpImage );
        extractor.compute( image, kpImage, des_image );

        matcher.knnMatch(des_object, des_image, matches, 2);

        for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
        {
            if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
            {
                good_matches.push_back(matches[i][0]);
            }
        }

        if (good_matches.size() >= 4)
        {
            for( int i = 0; i < good_matches.size(); i++ )
            {
                //Get the keypoints from the good matches
                obj.push_back( kpTarget[ good_matches[i].queryIdx ].pt );
                scene.push_back( kpImage[ good_matches[i].trainIdx ].pt );
            }

            H = findHomography( obj, scene, CV_RANSAC );

            perspectiveTransform( tgt_corners, scene_corners, H);
            
            Point2f center(0,0);
            for(int i = 0; i < 4; i++)
            {
				center.x += scene_corners[i].x;
				center.y += scene_corners[i].y;
			}
			center.x/=4; center.y/=4;

			circle( show, center, 5, Scalar( 255, 255, 0) );
			
			adjust(center, frame.cols, frame.rows);
        }
		line( show,  Point2f( show.cols/2, 0), Point2f( show.cols/2, show.rows), Scalar(0, 255, 0), 4 );
        line( show,  Point2f( 0, show.rows/2), Point2f( show.cols, show.rows/2), Scalar(0, 255, 0), 4 );
            
        //Show detected matches
        imshow( "Capture", show );

        key = waitKey(1);
    }
    return 0;
}
