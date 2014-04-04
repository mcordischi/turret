#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
#include "utils.h"

/**
* Camera Control Interface
**/
class AbstractCameraControl {

public:

    //Moves the camera in a specific direction and stops
    virtual bool move(int direction, int degree) = 0;


    //Moves the camera in a specific direction, only one step
    virtual bool moveStep(int dir) = 0 ;

    //Moves the camera to a position
    virtual bool move(Coordinates_t) = 0;

    //Stops movement
    virtual bool stop() = 0 ;

    //Sets the camera in the centre position and enables the coordinates mode
    virtual bool startCoordinates() = 0 ;

    //Return the X and Y position of the camera
    virtual Coordinates_t getCoordinates() = 0;

    //Returns the lastest frame obtained from the camera
    virtual cv::Mat* getFrame() = 0;

    //Triggers the alarm
    virtual bool triggerAlarm() = 0;

    //Checks wheter the movement is possible considering the angle
    virtual bool checkMovement(int dir, int degree) = 0;

};
