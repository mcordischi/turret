#include "AbstractCameraControl.h"
#include "FoscamCameraControl.h" //Using constants
#include "utils.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>


#ifndef __TEST_PRECISION_H_
#define __TEST_PRECISION_H_

//Test the camerma precision moving it for a while
void testPrecision(AbstractCameraControl* controller, char* path, int iterations,bool display);

#endif //__TEST_PRECISION_H_
