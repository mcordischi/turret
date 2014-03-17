#include "CameraControl.h"
#include "MovingAlgorithm.h"
#include <iostream>

int main(){
    CameraControl* control;
    control = new CameraControl("http://192.168.1.200","admin","31415LAS");


  //  for(long int i=0;i<100000;i++);

    MovingAlgorithm* move = new MovingAlgorithm(control);


    for( int i =0 ; i<10;i++){
        for(long long int j= 0; j<999999;j++)
            float fd = i*32.2;
        move->getNextFrame();
        std::cout<< "PUM!\n";
    }

//    bool result = control->move(CAM_UP,40);

//    std::cout << result;
}

