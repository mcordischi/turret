#include "HorizontalTracker.h"
#include <cstdlib>

HorizontalTracker::HorizontalTracker(AbstractCameraControl* controller){
    this->controller = controller;
    state = RH_STATE;
}


void  HorizontalTracker::nextStepOnTrack(){
    if(!controller->isReady()) return;

    if (!controller->moveStep(state)){
        //change state
        state = (state == RH_STATE) ? LF_STATE : RH_STATE;
        controller->moveStep(state);
    }
}


void HorizontalTracker::nextStepOnDetect(Coordinates_t coord){

    if (!controller->isReady()) return;

    //You cant move in two directions right now, just move in the farthest axis
    if(abs(coord.x) > abs(coord.y)){
        if (coord.x > 0) controller->move(CAM_RH,coord.x);
        else controller->move(CAM_LF,-coord.x);
    }else{
        if (coord.y>0) controller->move(CAM_UP,coord.y);
        else controller->move(CAM_DW,-coord.y);
    }
}

