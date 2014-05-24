#include "AbstractTracker.h"
#include "AbstractCameraControl.h"
#include "AbstractDetector.h"

#ifndef __OCV_DETECTOR_H_
#define __OCV_DETECTOR_H_


class  : public AbstractDetector{

    public:

        OpenCVDetector(AbstractTracker*,AbstractCameraControl*);

        bool identifyItem(char* picPath);
        bool identifyItem(cv::Mat item);
private:
    AbstractTracker* move;
    AbstractCameraControl* control;
};


#endif //__OCV_DETECTOR_H_
