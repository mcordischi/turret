#include "HorizontalTracker.h"

HorizontalTracker::HorizontalTracker(AbstractCameraControl* controller){
    this->controller = controller;
    state = INITIAL_STATE;
}


cv::Mat*  HorizontalTracker::getNextFrame(){

    cv::Mat* result = controller->getFrame();

    int direction;
    switch(state){
        case RH_STATE: direction = RH_STATE_MOVE;
                       break;
        case LF_STATE: direction = LF_STATE_MOVE;
    }

    if(!controller->moveStep(direction)){
        //change state and call again
        state = (state == RH_STATE) ? LF_STATE : RH_STATE;
        return result;
    //    return getNextFrame();
    }

    return result;
}
