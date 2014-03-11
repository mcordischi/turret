#include "CameraControl.h"
#include <iostream>

int main(){
    CameraControl* control;
    control = new CameraControl("http://192.168.1.200","admin","31415LAS");


    for(long int i=0;i<100000;i++);

    bool result = control->move(CAM_UP,40);

    std::cout << result;
}

