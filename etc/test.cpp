#include "CameraControl.h"
#include "MovingAlgorithm.h"
#include <iostream>
#include <unistd.h>

int main(){
    CameraControl* control;
    control = new CameraControl("http://192.168.1.200","admin","31415LAS");


  //  for(long int i=0;i<100000;i++);

    MovingAlgorithm* move = new MovingAlgorithm(control);


    for( int i =0 ; i<10;i++){
        usleep(2000000);
        move->getNextFrame();
        std::cout<< "PUM!\n";
    }

//    bool result = control->move(CAM_UP,40);

//    std::cout << result;
}

