#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
#include "utils.h"

#ifndef __ABS_CAM_CONTROL_H_
#define __ABS_CAM_CONTROL_H_



//Camera codes
//REDEFINE THEM IF NEEDED
#define CAM_UP 0
#define CAM_DW 2
#define CAM_LF 6
#define CAM_RH 4
#define CAM_STOP 1
#define CAM_CENTER 25

//Camera returning values
#define SUCCESS 1
#define FAILURE 0


/**
* Camera Control Interface
**/
class AbstractCameraControl {

public:

    //Moves the camera in a specific direction and stops
    virtual bool move(int direction, int degree) = 0;

    //Returns true if the camera is still and in position
    virtual bool isReady() = 0;

    //Moves the camera in a specific direction, only one step
    virtual bool moveStep(int dir) = 0 ;

    //Moves the camera to a position
    virtual bool move(Coordinates_t) = 0;

    //Moves the camera to a position, asuming current camera center is (0,0)
    virtual bool moveRelative(Coordinates_t) = 0;


    //Stops movement
    virtual bool stop() = 0 ;

    //Sets the camera in the centre position and enables the coordinates mode
    virtual bool startCoordinates() = 0 ;

    //Return the X and Y position of the camera
    //virtual Coordinates_t getCoordinates() = 0;

    //Returns the lastest frame obtained from the camera
    virtual cv::Mat* getFrame() = 0;

    //Triggers the alarm
    virtual bool triggerAlarm() = 0;

    //Checks wheter the movement is possible considering the angle
    virtual bool checkMovement(int dir, int degree) = 0;

};


#endif //__ABS_CAM_CONTROL_H_

