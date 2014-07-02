/* WARNING: this file must be changed for compilation
 * in not *nix OS.
 */

#include<ctime>

#ifndef __UTILS_H_
#define __UTILS_H_

//System constants
#define REQUIRED_PARAMS 3   //config.txt parameters

//DETECTOR CONSTANTS
#define MIN_KEY_POINTS 4 // min number of kp to accept the target image, MUST be >=4
#define MIN_GOOD_MATCHES_RATIO 0.25

//TRACKING CONSTANTS
#define TRACK_WINDOW 5 //Number of frames to analyze at once
#define MIN_MATCH_WINDOW 3 //Min number of frame matches in order to accept the target has been found


//CAMERA CONSTANTS
#define HOR_AOV 60 //Horizontal angle of view
#define VER_AOV 60 //Vertical angle of view


//Coordinates type, used in CameraControl
typedef struct{
    int x;
    int y;
}coordinates_t,Coordinates_t; //Don't use the one with capital C



/* Waits for a given period of time*/
void wait(int seconds);

/* Gets the current time*/
double getNow();

#endif //__UTILS_H_
