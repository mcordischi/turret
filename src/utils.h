/* WARNING: this file must be changed for compilation
 * in not *nix OS.
 */

#include<ctime>

#ifndef __UTILS_H_
#define __UTILS_H_

//System constants
#define REQUIRED_PARAMS 3

//Coordinates type, used in CameraControl
typedef struct{
    int x;
    int y;
}Coordinates_t;



/* Waits for a given period of time*/
void wait(int seconds);

#endif //__UTILS_H_
