#include "squareobjectselector.h"
#include <iostream>

squareObjectSelector::squareObjectSelector(double wideRatio)
{
    this->wideRatio = wideRatio;
}

squareObjectSelector::squareObjectSelector(){
    this->wideRatio = DEFAULT_WIDE_RATIO ;
}

std::vector<cv::Mat> squareObjectSelector::getObjects(cv::Mat * src, cv::Point2f p)
{
	// The return vector
    std::vector<cv::Mat> retVal;


    // The rectangle dimensions.

    double w = wideRatio * (double)src->cols ;
    double h = wideRatio * (double)src->rows ;
    double x = p.x-w/2.0; x = (x+w < src->cols) ? x : src->cols - w -1; x= x>0? x:0;
    double y = p.y-h/2.0; y = (y+h < src->rows) ? y : src->rows - h -1; y= y>0? y:0;
    cv::Rect br(x,y,w,h);
    //cv::Rect br(x<0?0:x,y<0?0:y,w>src->size().width?src->size().width:w,h>src->size().height?src->size().height:h);

// A new image is push with the info at rectangle position of source.
    cv::Mat result(*src,br);
    retVal.push_back(result);

    return retVal;
}
