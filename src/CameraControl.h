#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
//#include "AlarmTriggerer.h"

#ifndef __CONTROL_H_
#define __CONTROL_H_

#define CAM_UP 0
#define CAM_DW 2
#define CAM_LF 4
#define CAM_RH 6

#define CAM_CENTER 25

#define STEP_DEGREE 10 //CHECK

#define MAX_UP 60
#define MAX_DW 60
#define MAX_LF 150
#define MAX_RH 150

#define MOVE_DELAY 100000

#define SUCCESS 1
#define FAILURE 0

typedef struct{
    int x;
    int y;
}Coordinates_t;

#endif


class CameraControl {

private:
//  alarmComand* ac;
    char* cameraURL;
    CURL *curl;
    Coordinates_t coordinates;
    cv::VideoCapture* cvCamera;

    void updateCoordinates(int dir, int degree);

public:

    CameraControl(char* url, char* user, char* pwd);
//    CameraControl(alarmCommand*, char* url, char* login);

    //Moves the camera in a specific direction and stops
    bool move(int direction, int degree);

    //Moves the camera in a specific direction, only one step
    bool moveStep(int direction);

    //Moves the camera to a position
    bool move(Coordinates_t);

    //Return the X and Y position of the camera
    Coordinates_t getCoordinates();

    //Returns the lastest frame obtained from the camera
    cv::Mat* getFrame();

    //Triggers the alarm
    bool triggerAlarm();

    //Checks wheter the movement is possible considering the angle
    bool checkMovement(int dir, int degree);

};
