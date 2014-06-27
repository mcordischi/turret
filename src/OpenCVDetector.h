#include "AbstractTracker.h"
#include "AbstractCameraControl.h"
#include "AbstractDetector.h"

#ifndef __OCV_DETECTOR_H_
#define __OCV_DETECTOR_H_

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"


class OpenCVDetector : public AbstractDetector{

    public:
/*
        OpenCVDetector(AbstractTracker*,AbstractCameraControl*);*/

        OpenCVDetector();

        bool identifyItem(cv::Mat* frame, cv::Point2f &result);
        void setTarget(cv::Mat*);
/*
        bool identifyItem(char* picPath);
        bool identifyItem(cv::Mat item);
private:
    AbstractTracker* move;
    AbstractCameraControl* control;
    */
};


#endif //__OCV_DETECTOR_H_
