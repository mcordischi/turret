#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <opencv2/core/core.hpp>
#include <vector>

/**
 * Drawer class
**/
class Drawer{
    public:
        void drawAim(cv::Mat*, cv::Point2f);
        void drawRectangle(cv::Mat*, std::vector<cv::Point2f>);
};



#endif //_DRAWER_H_
