
#ifndef __ABS_DETECTOR_H_
#define __ABS_DETECTOR_H_

#include <opencv2/core/core.hpp>

/**
*	Image recognition Interface
**/
class AbstractDetector{
    public:
        virtual bool identifyItem(char* picPath) = 0;
        virtual bool identifyItem(cv::Mat) = 0;
};

#endif // __ABS_DETECTOR_H_
