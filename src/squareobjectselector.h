#ifndef SQUAREOBJECTSELECTOR_H
#define SQUAREOBJECTSELECTOR_H

#include "AbstractCameraControl.h"
#include <vector>
#include <opencv2/core/core.hpp>

class squareObjectSelector : public ObjectSelector
{
public:
    squareObjectSelector(double wide = 50);
    virtual std::vector<cv::Mat> getObjects(cv::Mat * src, cv::Point2d p);
private:
    double wide;
};

#endif // OBJECTSELECTOR_H


#endif // SQUAREOBJECTSELECTOR_H
