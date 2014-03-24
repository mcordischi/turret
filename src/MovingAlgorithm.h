#include"CameraControl.h"
#include "opencv2/core/core.hpp"

#ifndef __MOVING_ALG_H_
#define __MOVING_ALG_H_

#define INITIAL_STATE 0

#define RH_STATE 0
#define LF_STATE 1

#define RH_STATE_MOVE 6
#define LF_STATE_MOVE 4


class MovingAlgorithm {

    private:
        CameraControl* controller;
        int state;
    public:

        MovingAlgorithm(CameraControl*);

        //Returns the next desired frame
        cv::Mat* getNextFrame();

};

#endif //__MOVING_ALG_H_
