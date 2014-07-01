#include "facedetector.h"


FaceDetector::FaceDetector(): lPoint(0,0)
{
    cv::String face_cascade_name = "res/haarcascade_frontalface_alt.xml";
    face_cascade.load( face_cascade_name );

}

bool FaceDetector::identifyItem(cv::Mat *frame, cv::Point2f &result)
{
    std::vector<cv::Rect> faces;
    cv::Mat frame_gray;

    cvtColor( *frame, frame_gray, cv::COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

    if(faces.empty())
        return false;


    cv::Point nPoint = cv::Point( faces[0].x + faces[0].width/2, faces[0].y + faces[0].height/2 );

    // This is for selecting the nearest point to the last founded.
    double nDiff = cv::norm(nPoint-lPoint);
    for ( size_t i = 1; i < faces.size(); i++ )
    {
        cv::Point cPoint( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        double cDiff = cv::norm(cPoint-lPoint);
        if(cDiff<nDiff)
        {
            nPoint = cPoint;
            nDiff = cDiff;
        }
    }
    result = nPoint;
    return true;
}
