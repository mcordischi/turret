#include "testPrecision.h"

void testPrecision(CameraControl* controller, char* path, int iterations, bool display){

     controller->startCoordinates();
     wait(60);
     cv::Mat* initialImage = controller->getFrame();

    for(int i=0;i<iterations;i++){
        int yMove= (i%4 +1) * 10; //MAX = 40
        int xMove= (i%6 +1)* 20;  //MAX = 120
        std::cout << "UP :" << yMove;
        controller->move(CAM_UP,yMove);
        wait(yMove/5);
         std::cout << "RH:" << xMove;
        controller->move(CAM_RH,xMove);
        wait(xMove/10);
        std::cout << "DW:";
        controller->move(CAM_DW,yMove);
        wait(yMove/5);
        std::cout << "LF:";
        controller->move(CAM_LF,xMove);
        wait(xMove/10);
    }


     cv::Mat* finalImage = controller->getFrame();

     if (display){
         //Display images
        cv::namedWindow( "0", CV_WINDOW_AUTOSIZE );
        cv::imshow( "0", *initialImage );

        cv::namedWindow( "1", CV_WINDOW_AUTOSIZE );
        cv::imshow( "1", *finalImage );
     }

    //Save images
    char auxURL[100];
    sprintf((char*)&auxURL,"%s%i_precision_test_0.jpg",path,iterations);

     cv::imwrite(auxURL,*initialImage);

    sprintf((char*)&auxURL,"%s/%i_precision_test_1.jpg",path,iterations);

     cv::imwrite(auxURL,*finalImage);

     cv::waitKey(0);

}


