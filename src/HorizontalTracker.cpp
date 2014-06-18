#include "HorizontalTracker.h"
#include <cstdlib>

HorizontalTracker::HorizontalTracker(AbstractCameraControl* controller){
    this->controller = controller;
    state = INITIAL_STATE;
}


cv::Mat*  HorizontalTracker::getNextFrameOnTrack(){

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


cv::Mat* HorizontalTracker::getNextFrameOnDetect(Coordinates_t coord){

    cv::Mat* result = controller->getFrame();


    //You cant move in two directions right now, just move in a random axis
    int rand = rand %2;
    if(rand){
        if (coord.x > 0) controller->move(CAM_RH,coord.x);
        else controller->move(CAM_LF,-coord.x);
    }else{
        if (coord.y>0) controller->move(CAM_UP,coord.y);
        else controller->move(CAM_DW,-coord.y);
    }
  return result;
}

