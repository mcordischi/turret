/* WARNING: this file must be changed for compilation
 * in not *nix OS.
 */

#include<ctime>

#ifndef __UTILS_H_
#define __UTILS_H_

//System constants
#define REQUIRED_PARAMS 3   //config.txt parameters
#define MIN_GOOD_MATCHES 4 //number of good matches to find object
#define HOR_AOV 60 //Horizontal angle of view
#define VER_AOV 60 //Vertical angle of view


//Coordinates type, used in CameraControl
typedef struct{
    int x;
    int y;
}Coordinates_t;



/* Waits for a given period of time*/
void wait(int seconds);

#endif //__UTILS_H_
