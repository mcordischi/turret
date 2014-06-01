#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
#include "utils.h"
#include "AbstractCameraControl.h"
//#include "AlarmTriggerer.h"

#ifndef __FOSCAM_CONTROL_H_
#define __FOSCAM_CONTROL_H_

//Camera codes
#define CAM_UP 0
#define CAM_DW 2
#define CAM_LF 6
#define CAM_RH 4
#define CAM_STOP 1
#define CAM_CENTER 25

//Movement Constants
#define STEP_DEGREE 10 //CHECK
#define MAX_UP 60
#define MAX_DW -60
#define MAX_LF -150
#define MAX_RH 150
#define MOVE_DELAY 100000

//Camera returning values
#define SUCCESS 1
#define FAILURE 0
/* Moved to utils.h
typedef struct{
    int x;
    int y;
}Coordinates_t;
*/


class FoscamCameraControl : public AbstractCameraControl {

private:
//  alarmComand* ac;
    char* cameraURL;
    char* user;
    char* pwd;
//    CURL *curl;
    bool isCoordinates;
    Coordinates_t coordinates;
//    cv::VideoCapture* cvCamera;

    void updateCoordinates(int dir, int degree);

public:

    FoscamCameraControl(char* url, char* user, char* pwd);
//    CameraControl(alarmCommand*, char* url, char* login);

    //Moves the camera in a specific direction and stops
    bool move(int direction, int degree);


    //Moves the camera in a specific direction, only one step
    bool moveStep(int dir);

    //Moves the camera to a position
    bool move(Coordinates_t);

    //Stops movement
    bool stop();

    //Sets the camera in the centre position and enables the coordinates mode
    bool startCoordinates();

    //Return the X and Y position of the camera
    Coordinates_t getCoordinates();

    //Returns the lastest frame obtained from the camera
    cv::Mat* getFrame();

    //Triggers the alarm
    bool triggerAlarm();

    //Checks wheter the movement is possible considering the angle
    bool checkMovement(int dir, int degree);

};


#endif // __FOSCAM_CONTROL_H_