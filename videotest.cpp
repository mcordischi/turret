#include <opencv/highgui.h>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat frame;
    namedWindow("video", 1);
    VideoCapture cap;
    int r = cap.open("http://192.168.1.200/videostream.cgi?user=admin&pwd=31415LAS&resolution=32&dummy=.mjpg");
    while ( cap.isOpened() )
    {
        cap >> frame;
        if(frame.empty()) break;

        imshow("video", frame);
        if(waitKey(30) >= 0) break;
    }
	
    return r;
}
