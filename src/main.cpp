#include "CameraControl.h"
#include "MovingAlgorithm.h"
#include "SearchingAlgorithm.h"
#include "utils.h"
#include "testPrecision.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2 ){
        std::cout << "Usage: tracker <target image>\n"
                  << "               -t [-i iterations] [-p outputImagePath] [--display]\n  ";
    }

    char* url = "http://192.168.1.200";
    char* user = "admin";
    char* pwd = "31415LAS";
/*
    int configParams = 0;
    // Read from config file
    ifstream configFile;
    string input;
    configFile.open("config.txt");
    while(!configFile.eof()){
        configFile >> input;
        int pos = input.find("=");
        if( pos != string::npos ){
            string param = input.substr(0,pos-1);
            param.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

            string value = input.substr(pos+1,input.size());
            value.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

            switch(param){
                case "url" : url = value; configParams++;
                         break;
                case "user": user = value;configParams++;
                         break;
                case "pwd" : pwd = value;configParams++;
                        break;
                default:  cout << "Warning: No allowed value in config.txt file";
            }
        }
    }

    if (configParams < REQUIRED_PARAMS){
        cout << "Error: Missing parameters in config file";
    }
*/
    CameraControl* controller= new CameraControl(url,user,pwd);

    bool test = false;
    if (strcmp(argv[1], "-t") == 0)  test= true;

//    for (int i  = 0 ; i< argc; i++) cout << i << " " << argv[i] << endl;


    if (!test){
        //normal program
        char* picPath= argv[1];


        MovingAlgorithm* mover = new MovingAlgorithm(controller);
        SearchingAlgorithm* searcher= new SearchingAlgorithm(mover,controller);

        searcher->identifyItem(picPath);

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

