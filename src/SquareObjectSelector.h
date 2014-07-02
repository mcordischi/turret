#ifndef _SQUARE_OBJECT_SELECTOR_H_
#define _SQUARE_OBJECT_SELECTOR_H_

#include "ObjectSelector.h"
#include <vector>
#include <opencv2/core/core.hpp>


#define DEFAULT_WIDE_RATIO 0.4

class SquareObjectSelector : public ObjectSelector
{
public:
    SquareObjectSelector(double wideRatio);
    SquareObjectSelector();
    std::vector<cv::Mat> getObjects(cv::Mat * src, cv::Point2f p);
private:
    double wideRatio;
};


#endif // _SQUARE_OBJECT_SELECTOR_H_
