#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "AbstractDetector.h"

class FaceDetector : public AbstractDetector
{
public:
    FaceDetector();
    virtual bool identifyItem(cv::Mat* frame, cv::Point2f &result);
private:
    cv::CascadeClassifier face_cascade;
    cv::Point lPoint;
};

#endif // FACEDETECTOR_H
