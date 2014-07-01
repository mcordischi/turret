#ifndef SQUAREOBJECTSELECTOR_H
#define SQUAREOBJECTSELECTOR_H

#include "ObjectSelector.h"
#include <vector>
#include <opencv2/core/core.hpp>


#define DEFAULT_WIDE_RATIO 0.4

class squareObjectSelector : public ObjectSelector
{
public:
    squareObjectSelector(double wideRatio);
    squareObjectSelector();
    std::vector<cv::Mat> getObjects(cv::Mat * src, cv::Point2f p);
private:
    double wideRatio;
};


#endif // SQUAREOBJECTSELECTOR_H
