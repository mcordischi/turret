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

using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2 ){
        std::cout << "Usage: tracker <target image>\n"
                  << "               -t [-i iterations] [-p outputImagePath] [--display]\n  ";
    }

    //TODO Read from .config file
    char* url = "192.168.1.200";
    char* user = "admin";
    char* pwd = "31415LAS";
    CameraControl* controller= new CameraControl(url,user,pwd);

    bool test = false;
    if (strcmp(argv[1], "-t") == 0)  test= true;


    if (!test){
        //normal program
        char* picPath= argv[1];


        MovingAlgorithm* mover = new MovingAlgorithm(controller);
        SearchingAlgorithm* searcher= new SearchingAlgorithm(mover,controller);

        searcher->identifyItem(picPath);

    }
    else{
        //testPresicion program
        int iterations = 5;
        char* outputPath = "./";
        bool display = false;

        for(int args = 2; args<argc; args++){
            if (strcmp(argv[args], "-i")){
                args++;
                iterations = (int) *argv[args];
            } else
                if(strcmp(argv[args], "-p")){
                    args++;
                    outputPath = argv[args];
                } else
                if(strcmp(argv[args], "--display")){
                    display = true;
                }
                else  cout<< "Wrong usage. Check parameters";
        }

               testPrecision(controller,outputPath,iterations,display);
    }
    //TODO delete temp images

    return 0;

}

