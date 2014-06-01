#include <iostream>
#include "AbstractCameraControl.h"
#include "FoscamCameraControl.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include <QCoreApplication>
#define TEST
#ifndef TEST

using namespace cv;
using namespace std;

Mat * src;
void click_callback(int, int, int, int, void*);

int selectorExample( int, char**)
{

  AbstractCameraControl * cam = new FoscamCameraControl("192.168.1.200", "admin", "31415LAS");
  src = cam->getFrame();

  /// Create Window
  namedWindow( "Source", CV_WINDOW_AUTOSIZE );
  imshow( "Source", *src );
  setMouseCallback("Source", click_callback);

  waitKey(0);
  return(0);
}
/*
#include "ObjectSelector.h"

void click_callback(int event, int x, int y, int, void*)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        ObjectSelector sel(50);
        vector<cv::Mat> r = sel.getObjects(src,Point2d(x,y));
        namedWindow( "0", CV_WINDOW_AUTOSIZE );
        namedWindow( "1", CV_WINDOW_AUTOSIZE );
        namedWindow( "2", CV_WINDOW_AUTOSIZE );
        if(r.size()>0)
            imshow( "0", r[0] );
        if(r.size()>1)
            imshow( "1", r[1] );
        if(r.size()>2)
            imshow( "2", r[2] );
    }
}
*/
#endif
