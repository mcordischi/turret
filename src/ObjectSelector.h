#ifndef OBJECTSELECTOR_H
#define OBJECTSELECTOR_H

#include "AbstractCameraControl.h"
#include <vector>
#include <opencv2/core/core.hpp>


class ObjectSelector
{
public:
    ObjectSelector(int thresh);
    ObjectSelector(int thresh, int maxRet);
    std::vector<cv::Mat> getObjects(cv::Mat * src, cv::Point2d p);
private:
    int thresh;
    unsigned int maxRet;
};

#endif // OBJECTSELECTOR_H
