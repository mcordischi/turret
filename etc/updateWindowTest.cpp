#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "utils.h"
#include "FoscamCameraControl.h"


using namespace std;
using namespace cv;


int main(){
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

    cout << "-- LA CAMERA INFO (from config.txt)--\n";
    cout << "URL: " << url << "\n";
    cout << "user: " << user<< "\n";
    cout << "pwd: " << pwd << "\n";

    cout << "Initializing controller..." ;
    AbstractCameraControl* controller;
    controller= new FoscamCameraControl(url,user,pwd);
    Mat* src;
    cout << "\tOK\n";


    cout << "Getting frames";
    src = controller->getFrame();
    /// Create Window
    namedWindow( "Source", CV_WINDOW_AUTOSIZE );
    //waitKey(0);
    imshow( "Source", *src );
    waitKey(1);
    while(true){
        delete src;
        src = controller->getFrame();
        imshow("Source",*src);
        updateWindow("Source");
        waitKey(1);
       // cout << ".";
    }
    cout << "halt";
    waitKey(0);
}
