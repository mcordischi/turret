#include "AbstractCameraControl.h"
#include "AbstractTracker.h"
#include "AbstractDetector.h"
#include "utils.h"
#include "testPrecision.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

#include "FoscamCameraControl.h"
#include "HorizontalTracker.h"
#include "OpenCVDetector.h"


using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2 ){
        std::cout << "Usage: tracker <target image>\n"
                  << "               -t [-i iterations] [-p outputImagePath] [--display]\n  ";
        exit(EXIT_FAILURE);
    }

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

    cout << "--CAMERA INFO (from config.txt)--\n";
    cout << "URL: " << url << "\n";
    cout << "user: " << user<< "\n";
    cout << "pwd: " << pwd << "\n";

    AbstractCameraControl* controller= new FoscamCameraControl(url,user,pwd);

    bool test = false;
    if (strcmp(argv[1], "-t") == 0)  test= true;

//    for (int i  = 0 ; i< argc; i++) cout << i << " " << argv[i] << endl;


    if (!test){
        //normal program
        char* picPath= argv[1];


        AbstractTracker* tracker = new HorizontalTracker(controller);
        AbstractDetector* detector= new OpenCVDetector(tracker,controller);

        detector->identifyItem(picPath);

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

