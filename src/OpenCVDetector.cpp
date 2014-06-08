#include "OpenCVDetector.h"
#include "utils.h"

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

OpenCVDetector::OpenCVDetector(AbstractTracker* move,AbstractCameraControl* control){
    this->move = move;
    this->control = control;
}


bool OpenCVDetector::identifyItem(char* picPath)
{

    Mat mTarget = imread( picPath, CV_LOAD_IMAGE_GRAYSCALE );
    if (! mTarget.data){ std::cout << "Error: no picture" ; return false;}
    return identifyItem(mTarget);
}

bool OpenCVDetector::identifyItem(cv::Mat mTarget)
{
    //startCoordinates -- Already in main
   // control->startCoordinates();
   // wait(60);

    //Detect the keypoints using SURF Detector
    int minHessian = 500;

    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> kpTarget;

    detector.detect( mTarget, kpTarget );

    //Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    Mat des_object;

    extractor.compute( mTarget, kpTarget, des_object );

    FlannBasedMatcher matcher;

    //namedWindow("Capture");

    std::vector<Point2f> tgt_corners(4);

    //Get the corners from the object
    tgt_corners[0] = cvPoint(0,0);
    tgt_corners[1] = cvPoint( mTarget.cols, 0 );
    tgt_corners[2] = cvPoint( mTarget.cols, mTarget.rows );
    tgt_corners[3] = cvPoint( 0, mTarget.rows );

    char key = 'a';
    int framecount = 0;
    bool found = false;
    bool track = true;
        namedWindow( "Source", CV_WINDOW_AUTOSIZE );

    while (!found && track)
    {
        Mat* frame;
        frame= move->getNextFrame();
        if(! frame->data) { std::cout << "Null frame\n" ; continue;}
//        cap >> frame;

/*      //Drop frames
        if (framecount < 5)
        {
            framecount++;
            continue;
        }
*/
        Mat des_image, img_matches;
        std::vector<KeyPoint> kpImage;
        std::vector<vector<DMatch > > matches;
        std::vector<DMatch > good_matches;
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        std::vector<Point2f> scene_corners(4);
        Mat H;
        Mat image;

        cvtColor(*frame, image, CV_RGB2GRAY);

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

        //Draw only "good" matches
        drawMatches( mTarget, kpTarget, image, kpImage, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        if (good_matches.size() >= MIN_GOOD_MATCHES)
        {

            cout << "ITEM FOUND!" ;

            //Stop camera from moving
            control->stop();

            //@Deprecated
            for( int i = 0; i < good_matches.size(); i++ )
            {
                //Get the keypoints from the good matches
                obj.push_back( kpTarget[ good_matches[i].queryIdx ].pt );
                scene.push_back( kpImage[ good_matches[i].trainIdx ].pt );
            }

            H = findHomography( obj, scene, CV_RANSAC );

            perspectiveTransform( tgt_corners, scene_corners, H);

            //Draw lines between the corners (the mapped object in the scene image )
            line( img_matches, scene_corners[0] + Point2f( mTarget.cols, 0), scene_corners[1] + Point2f( mTarget.cols, 0), Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1] + Point2f( mTarget.cols, 0), scene_corners[2] + Point2f( mTarget.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2] + Point2f( mTarget.cols, 0), scene_corners[3] + Point2f( mTarget.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3] + Point2f( mTarget.cols, 0), scene_corners[0] + Point2f( mTarget.cols, 0), Scalar( 0, 255, 0), 4 );




           // std::cout<<"**MATCH FOUND***\nDisplaying match, if you want to continue searching, press 'c'";

           //Show detected matches
            cout << "MATCH!" ;
            imshow( "Source", img_matches );
            updateWindow("Source");
            waitKey(1);


            //Get object's center position and move camera towards it
            Coordinates_t mean ;
            mean.x=0; mean.y = 0;
            for ( int i = 0 ; i< 4 ; i++){
                mean.x += (int)scene_corners[i].x;
                mean.y +=(int) scene_corners[i].y;
            }
            mean.x /= 4;
            mean.y /= 4;

            cout << " - Starting tracking" << endl;
            control->move(mean);
            wait(1);

            //key = waitKey(0);
            //if (key != 99 ) // 'c' : continue looking
            //    found = true;


        }
        imshow("Source",*frame);
        updateWindow("Source");
        waitKey(1);
    }
    return true;
}
