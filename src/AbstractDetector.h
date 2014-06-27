
#ifndef __ABS_DETECTOR_H_
#define __ABS_DETECTOR_H_

#include <opencv2/core/core.hpp>

/**
*	Image recognition Interface
**/
class AbstractDetector{
    protected:
        cv::Mat* target;
    public:

        //Sets the target
        void setTarget(cv::Mat*);

        //Identifies the target in the frame. If match returns true and the coordinates of the item's center
        virtual bool identifyItem(cv::Mat* frame, cv::Point2f &result) = 0;
        /*
        virtual bool identifyItem(char* picPath) = 0;
        virtual bool identifyItem(cv::Mat) = 0;
        */
};

#endif // __ABS_DETECTOR_H_
