#include "opencv2/core/core.hpp"

#ifndef __ABS_TRACKER_H_
#define __ABS_TRACKER_H_

class AbstractTracker {

    public:

        //Returns the next desired frame
        virtual cv::Mat* getNextFrame() = 0;

};

#endif //__ABS_TRACKER_H_
