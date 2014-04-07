#include "AbstractTracker.h"
#include "AbstractCameraControl.h"
#include "AbstractDetector.h"

#ifndef __OCV_DETECTOR_H_
#define __OCV_DETECTOR_H_


class OpenCVDetector : public AbstractDetector{
    private:
        AbstractTracker* move;
        AbstractCameraControl* control;

    public:

        OpenCVDetector(AbstractTracker*,AbstractCameraControl*);

        bool identifyItem(char* picPath);
};


#endif //__OCV_DETECTOR_H_
