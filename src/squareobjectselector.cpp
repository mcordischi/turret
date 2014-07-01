#include "squareobjectselector.h"

squareObjectSelector::squareObjectSelector(double wide)
{
    this->wide = wide;
}

std::vector<cv::Mat> squareObjectSelector::getObjects(cv::Mat * src, cv::Point2d p)
{
	// The return vector
    std::vector<cv::Mat> retVal;
    
    // The rectangle dimensions.
    double x = p.x+wide/2.0;
    double y = p.y+wide/2.0;
    double w = p.x+wide/2.0;
    double h = p.y+wide/2.0;
    cv::Rect br(x<0?0:x,y<0?0:y,w>src->size().width?src->size().width:w,h>src->size().height?src->size().height:h);

	// A new image is push with the info at rectangle position of source.
    retVal.push_back(cv::Mat(src,br));

    return retVal;
}
