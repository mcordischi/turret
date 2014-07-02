#ifndef _FACE_DETECTOR_H_
#define _FACE_DETECTOR_H_

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "AbstractDetector.h"

class FaceDetector : public AbstractDetector
{
public:
    FaceDetector();
    FaceDetector(cv::Point2f);
    virtual bool identifyItem(cv::Mat* frame, cv::Point2f &result);
private:
    cv::CascadeClassifier face_cascade;
    cv::Point2f lPoint;
};

#endif // _FACE_DETECTOR_H_
