#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
#include "utils.h"
#include "AbstractCameraControl.h"
//#include "AlarmTriggerer.h"

#include <ctime>

#ifndef __FOSCAM_CONTROL_H_
#define __FOSCAM_CONTROL_H_


//Movement Constants
#define STEP_DEGREE 4 //CHECK
#define MAX_UP 60
#define MAX_DW -60
#define MAX_LF -150
#define MAX_RH 150
#define CENTER_DELAY 70 //Center function delay in seconds - 55< real < 70
#define VER_SPEED 10  //Vertical speed in deg/sec - real ~ 12
#define HOR_SPEED 10  //Horizontal speed in deg/sec - real ~ 10


class FoscamCameraControl : public AbstractCameraControl {

private:
//  alarmComand* ac;
    char* cameraURL;
    char* user;
    char* pwd;
    void* charImage;
    bool isCoordinates;
    Coordinates_t currentPosition;
    Coordinates_t desiredPosition;
    int lastDirection;
    double lastMove; //secs
    cv::Mat* frame;

    void changeDesiredPosition(int dir, int degree);

    //Updates coordinates and returns current position
    Coordinates_t updatePosition();

    //curl GET request
    bool sendMovementRequest(int direction, int degree);


public:

    FoscamCameraControl(char* url, char* user, char* pwd);
//    CameraControl(alarmCommand*, char* url, char* login);

    //Moves the camera in a specific direction and stops
    bool move(int direction, int degree);

    //Returns true if the camera is still and in position
    bool isReady();

    //Moves the camera in a specific direction, only one step
    bool moveStep(int dir);

    //Moves the camera to a position
    bool move(Coordinates_t);

    //Moves the camera to a position, asuming current camera center is (0,0)
    bool moveRelative(Coordinates_t);

    //Stops movement
    bool stop();

    //Sets the camera in the centre position and enables the coordinates mode
    bool startCoordinates();

    //Return the X and Y position of the camera
   // Coordinates_t getPosition();

    //Returns the lastest frame obtained from the camera
    cv::Mat* getFrame();

    //Triggers the alarm
    bool triggerAlarm();

    //Checks wheter the movement is possible considering the angle
    bool checkMovement(int dir, int degree);

};


#endif // __FOSCAM_CONTROL_H_
