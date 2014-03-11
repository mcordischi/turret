#include "CameraControl.h"
#include <iostream>

using namespace std;

int main(){
    CameraControl* control;
    control = new CameraControl("http://192.168.1.200","admin:31415LAS");

    bool result = control->move(CAM_UP,10);

    cout << result;
}

