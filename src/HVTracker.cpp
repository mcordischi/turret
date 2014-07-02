#include "HVTracker.h"


HVTracker::HVTracker(AbstractCameraControl* c):HorizontalTracker(c){
    verticalState = UP_STATE;
}


void  HVTracker::nextStepOnTrack(){
    if(!controller->isReady()) return;

    if (!controller->moveStep(state)){
        //change state
        state = (state == RH_STATE) ? LF_STATE : RH_STATE;

        //Move vertically
        if(!controller->moveStep(verticalState)){
            verticalState = (verticalState == UP_STATE) ? DW_STATE : UP_STATE;
            controller->moveStep(verticalState);
        }
    }
}
