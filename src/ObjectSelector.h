#ifndef OBJECTSELECTOR_H
#define OBJECTSELECTOR_H

#include "AbstractCameraControl.h"
#include <vector>
#include <opencv2/core/core.hpp>

#define DEFAULT_THRESH 50

class ObjectSelector
{
public:
    ObjectSelector();
    ObjectSelector(int thresh);
    ObjectSelector(int thresh, int maxRet);
    virtual std::vector<cv::Mat> getObjects(cv::Mat * src, cv::Point2f p);
private:
    int thresh;
    unsigned int maxRet;
};

#endif // OBJECTSELECTOR_H
