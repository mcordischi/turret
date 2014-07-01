#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <vector>

/**
 * Drawer class
**/
class Drawer{
    public:
        void drawAim(cv::Mat*, cv::Point2f);
        void drawRectangle(cv::Mat*, std::vector<cv::Point2f>);
        void drawKp(cv::Mat*);
};



#endif //_DRAWER_H_
