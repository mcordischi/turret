#ifndef __ABS_TRACKER_H_
#define __ABS_TRACKER_H_

#include "opencv2/core/core.hpp"
#include "utils.h"

class AbstractTracker {
    public:

        //Returns the next desired frame
        virtual void nextStepOnTrack() = 0;

        //Returns the next frame on a tracking state
        virtual void nextStepOnDetect(Coordinates_t) = 0;

};

#endif //__ABS_TRACKER_H_
