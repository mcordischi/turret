#include "ObjectSelector.h"
#include <limits>

ObjectSelector::ObjectSelector(){
    this->thresh = DEFAULT_THRESH;
}


ObjectSelector::ObjectSelector(int thresh) : thresh(thresh), maxRet((unsigned)-1)
{

}

ObjectSelector::ObjectSelector(int thresh, int maxRet) : thresh(thresh), maxRet((unsigned)maxRet)
{

}

#include <queue>

class TestedContour
{
public:
    TestedContour(int id, int dist) : dist(dist), id(id) {}
    int Id(){return id;}
    bool isIn(){return id<0;}
    bool operator<(const TestedContour & r) const {return dist>r.dist;}
private:
    int dist;
    int id;
};

std::vector<cv::Mat> ObjectSelector::getObjects(cv::Mat * src, cv::Point2f p)
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat canny_output;
    cv::Mat src_gray;
    cvtColor( *src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, cv::Size(3,3) );


    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    std::priority_queue<TestedContour> results;

    for( int i = 0; i< contours.size(); i++ )
        results.push(TestedContour(i,-pointPolygonTest(contours[i],p,true)));

    std::vector<cv::Mat> retVal;

    for(int i = 0; i < maxRet && !results.empty(); i++)
    {
        TestedContour c = results.top(); results.pop();
        cv::Rect br = boundingRect( contours[c.Id()] );
        cv::Mat mask = cv::Mat::zeros(src->size(), CV_8UC1);
        std::vector<std::vector<cv::Point> > hull;
        hull.push_back(std::vector<cv::Point>());
        convexHull(contours[c.Id()],hull[0],1,true);
        drawContours( mask, hull, 0, cv::Scalar(255), CV_FILLED );
        cv::Mat tmp = cv::Mat::zeros(br.size(), CV_8UC3);
        src->copyTo(tmp, mask);
        retVal.push_back(cv::Mat(tmp,br));
    }
    return retVal;
}
