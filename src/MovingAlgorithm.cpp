#include "MovingAlgorithm.h"

MovingAlgorithm::MovingAlgorithm(CameraControl* controller){
    this->controller = controller;
    state = INITIAL_STATE;
}


cv::Mat*  MovingAlgorithm::getNextFrame(){

    cv::Mat* result = new cv::Mat(); // controller->getFrame();

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
