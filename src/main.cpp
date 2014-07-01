#include "AbstractCameraControl.h"
#include "AbstractTracker.h"
#include "AbstractDetector.h"
#include "utils.h"
#include "testPrecision.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

#include "FoscamCameraControl.h"
#include "HorizontalTracker.h"
#include "OpenCVDetector.h"
#include "Drawer.h"
#include "squareobjectselector.h"

using namespace std;
using namespace cv;

Mat* src;
vector<Mat> r;
bool cont;
//int selectorExample( int, char**);
void click_callback(int, int, int, int, void*);
void startTracking( Mat* target);
cv::Mat* loadImage(char* picPath);


AbstractTracker* tracker;
AbstractDetector* detector;
AbstractCameraControl* controller;
Drawer* drawer;

int main(int argc, char* argv[]){
    cont = false;

    //Check Parameters
    if(argc > 8 ){
        std::cout << "Usage: tracker [image]\n"
                  << "               [-t] [-i iterations] [-p outputImagePath] [--display]\n  ";
        exit(EXIT_FAILURE);
    }

    bool test = false;
    bool imageByParam = false;

    for (int i =1 ; i< argc; i++)
        if (strcmp(argv[1], "-t") == 0){  test= true; break;}
    if (!test && argc > 1) imageByParam = true;

    char* url;// = "http://192.168.1.200";
    char* user;// = "admin";
    char* pwd;// = "31415LAS";

    int configParams = 0;
    // Read from config file
    ifstream configFile("config.txt");
    if(!configFile.is_open()){
        cout << "ERROR, No config.txt!";
        exit(EXIT_FAILURE);
    }

    std::string line;
	while( std::getline(configFile, line) )
	{
	  std::istringstream is_line(line);
	  std::string key;
	  if( std::getline(is_line, key, '=') )
	  {
		std::string value;
		if( std::getline(is_line, value) )
		{
			char * cval = (char*)malloc((value.length() + 1) * sizeof(char));
			strcpy(cval, value.c_str());
			if(strcmp(key.c_str(), "url") == 0)
				url=cval;
			else if(strcmp(key.c_str(), "user") == 0)
				user=cval;
			else if(strcmp(key.c_str(), "pwd") == 0)
				pwd=cval;
		}
		configParams++;
	  }
	}
    configFile.close();
    if (configParams < REQUIRED_PARAMS){
        cout << "Error: Missing parameters in config file";
        exit(EXIT_FAILURE);
    }

    //Print camera info
    cout << "--CAMERA INFO (from config.txt)--\n";
    cout << "URL: " << url << "\n";
    cout << "user: " << user<< "\n";
    cout << "pwd: " << pwd << "\n";

    //Create camera controller
    controller= new FoscamCameraControl(url,user,pwd);
    cout << "Connected" << endl;

    //startCoordinates
    controller->startCoordinates();
    wait(75);


    if (!test){
        //normal program
        tracker = new HorizontalTracker(controller);
        detector= new OpenCVDetector();
        drawer = new Drawer();

        // Create Window
        namedWindow( "Source", CV_WINDOW_AUTOSIZE );

        Mat* target;
        if(imageByParam){
            cout << "Image by parameter" << endl;
            char* picPath= argv[1];
            target = loadImage(picPath);
        } else{
            cout << "Image by selection" << endl;
            src = controller->getFrame();
            imshow( "Source", *src );
            setMouseCallback("Source", click_callback);
            do{
                //wait(1);
                waitKey(100);
            }while (!cont); // 'q' : quit
            target = &r[0];
        }
        //Display target
        namedWindow("Target", CV_WINDOW_AUTOSIZE);
        imshow( "Target", *target );
        waitKey(1);

        startTracking(target);
        return 0;
    }
    else{
        //testPrecision program
        cout << "---Testing precision---\n" ;
        int iterations = 5;
        char* outputPath = "./";
        bool display = false;

        for(int args = 2; args<argc; args++){
            if (strcmp(argv[args], "-i" ) == 0){
                args++;
                iterations = atoi(argv[args]);
            } else {
                if(strcmp(argv[args], "-p") == 0){
                    args++;
                    outputPath = argv[args];
                       } else {
                if(strcmp(argv[args], "--display") == 0 ){
                    display = true;
                }
                else  cout<< "Wrong usage. Check parameters";
                }
            }
        }

        cout << "Number of iterations: "<<  iterations << "\n";
        cout << "output Path: " << outputPath  << "\n";


            cout << "Starting test...\n" ;
               testPrecision(controller,outputPath,iterations,display);
    }
    //TODO delete temp images

    return 0;

}



#include "ObjectSelector.h"

void click_callback(int event, int x, int y, int, void*)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
//        ObjectSelector sel(50);
        squareObjectSelector sel;
        r = sel.getObjects(src,Point2d(x,y));
        namedWindow( "Target", CV_WINDOW_AUTOSIZE );
        //namedWindow( "1", CV_WINDOW_AUTOSIZE );
        //namedWindow( "2", CV_WINDOW_AUTOSIZE );
//        if(r.size()>0)
//            imshow( "Target", r[0] );
//        waitKey(1);
   /*     if(r.size()>1)
            imshow( "1", r[1] );
        if(r.size()>2)
            imshow( "2", r[2] );*/
        cout << "Looking for r[0]" << endl;
        cont = true;
    }
}



//Tracking
void startTracking( Mat* target){
    char key = 'a';
    namedWindow( "Source", CV_WINDOW_AUTOSIZE );

    detector->setTarget(target);

    while(key != 113){ //'q' : quit
        int matchCount = 0;
        Point2f targetPosition;
        targetPosition.x = targetPosition.y = 0;


        Mat* frame ;
        //Grab a number of frames, analyze them
        for (int i = 0 ; i < TRACK_WINDOW;i++){
            frame = controller->getFrame();
             Point2f c;
             bool match = detector->identifyItem(frame,c);
             if ( match ){ //Match
                 matchCount++;
                 targetPosition.x += c.x;
                 targetPosition.y += c.y;
            }
              }


        //Item found?
        if (matchCount >= MIN_MATCH_WINDOW){
            //getMeanPosition
            targetPosition.x /= (double)matchCount;
            targetPosition.y /= (double)matchCount;

            coordinates_t gradPos;
            //get relative position in degrees, with Angle of view
            gradPos.x = (int)(targetPosition.x / (double)frame->cols * (double)HOR_AOV - HOR_AOV/2);
            gradPos.y = VER_AOV/2 - (int)(targetPosition.y / (double)frame->rows * (double)VER_AOV);


            //draw image
            cout << "FOUND "<< gradPos.x << "," << gradPos.y << endl ;
            cout << "\t" << targetPosition.x<< "," << targetPosition.y<< endl;
            cout << "\t" << (double)frame->cols << "," << (double)frame->rows << endl;
            cout << "\t" << targetPosition.x /(double)frame->cols << "," << targetPosition.y/(double)frame->rows << endl;


            drawer->drawAim(frame,targetPosition);

            //Notify Tracker
            tracker->nextStepOnDetect(gradPos);
        } else{
            if(matchCount >0) cout << matchCount <<"/" << TRACK_WINDOW << endl;
            tracker->nextStepOnTrack();
        }

        //display image
        imshow( "Source", *frame );
        updateWindow("Source");
        key = waitKey(20);
    }


}


//Image loader
cv::Mat* loadImage(char* picPath){
    cv::Mat* mTarget = new cv::Mat(imread( picPath, CV_LOAD_IMAGE_GRAYSCALE ));
    if (! mTarget->data){ std::cout << "Error: no picture" ;}
    return mTarget;
}

