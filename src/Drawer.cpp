#include "Drawer.h"

using namespace cv;

void Drawer::drawAim(Mat* frame, Point2f center){
    double radius = 20.0;
    Scalar lineColor(0,0,255);

    //draw a circle
    circle(*frame,center,(int)radius,lineColor,2,8,0);

    //And a cross
    Point2f a(center.x-radius*2,center.y);
    Point2f b(center.x+radius*2,center.y);
    Point2f c(center.x,center.y-radius*2);
    Point2f d(center.x,center.y+radius*2);
    line(*frame,a,b,lineColor,1);
    line(*frame,c,d,lineColor,1);
}

void Drawer::drawRectangle(Mat* frame, std::vector<Point2f> edges){
   // polylines(frame,edges,edges.size,0,true,lineColor,2,8,0);
    Scalar lineColor(0,255,0);
    int thickness = 4;
    line( *frame, edges[0] , edges[1] , lineColor, thickness );
    line( *frame, edges[1] , edges[2] , lineColor, thickness );
    line( *frame, edges[2] , edges[3] , lineColor, thickness );
    line( *frame, edges[3] , edges[0] , lineColor, thickness );


}
