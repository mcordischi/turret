#include "AbstractCameraControl.h"
#include "AbstractTracker.h"
#include "opencv2/core/core.hpp"

#ifndef __HOR_TRACKER_H_
#define __HOR_TRACKER_H_

#define INITIAL_STATE 0

#define RH_STATE 0
#define LF_STATE 1

#define RH_STATE_MOVE 6
#define LF_STATE_MOVE 4


class HorizontalTracker : public AbstractTracker {

    private:
        AbstractCameraControl* controller;
        int state;
    public:

        HorizontalTracker(AbstractCameraControl*);

        //Returns the next desired frame
        cv::Mat* getNextFrame();

};

#endif //__HOR_TRACKER_H_
