#include "FoscamCameraControl.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
//#include "AlarmTriggerer.h"
#include <sys/timeb.h> //time in milliseconds


FoscamCameraControl::FoscamCameraControl(/*AlarmTriggerer* at,*/ char* url, char* user, char* pwd){
 //   this.alarmTriggerer = at;
    this->cameraURL = url;
    this->user = user;
    this->pwd = pwd;
    charImage = malloc(sizeof(char) * 320000);
}
/*  DEPRECATED: it is better to request a snapshot that start a streaming. Delete on next code review
	// Start camera Streaming
    CURL* curl;
    //Connect to camera
    curl = curl_easy_init();
    if(!curl){
        std::cout << "No curl";
        std::exit(1);
    }


    char auxURL[100];
    sprintf((char*)&auxURL, "%s:%s",user,pwd);
    curl_easy_setopt(curl,CURLOPT_USERPWD,auxURL);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

 //   moveStep(CAM_CENTRE); //Wait!?

    //get the camera
    sprintf((char*)&auxURL, "%s/videostream.cgi?user=%s&pwd=%s&resolution=32&dummy=.mjgp",cameraURL,user,pwd);
    cvCamera= new cv::VideoCapture(auxURL);
   curl_easy_cleanup(curl);

}
*/


bool FoscamCameraControl::startCoordinates(){
    timeb tb;
    ftime(&tb);
    lastMove =0;

    move(CAM_CENTER,0);
}


    //Moves the camera in a specific direction and stops
bool FoscamCameraControl::move(int dir, int degree){

    //If the movement is less than a step, avoid moving to reduce errors
    if(dir != CAM_CENTER && degree < STEP_DEGREE) return true;
    updatePosition();
    std::cout << "Position=(" << actualPosition.x << "," << actualPosition.y << ")" << std::endl;

    //TEMP - If camera not ready, don't move
    if (!isReady()){
        std::cout << "Camera on movement\n" ;
        return false;
    }

    if (!checkMovement(dir,degree)){
        std::cout << "Movement denied - dir:" << dir << " degree:" << degree << "\n";
        return false;
    }
    std::cout << "MOVE: " << dir << "," << degree << std::endl ;
    char url[75];

    sprintf((char*)&url, "%s/decoder_control.cgi?command=%i&onestep=1&degree=%i",cameraURL, dir, degree);

    CURL* curl;
    curl = curl_easy_init();
    if(!curl){
        std::cout << "No curl";
        std::exit(1);
    }


    char auxURL[100];
    sprintf((char*)&auxURL, "%s:%s",user,pwd);
    curl_easy_setopt(curl,CURLOPT_USERPWD,auxURL);



	curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    //Testing no stdout
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, NULL);
//    curl_easy_setopt(connection, CURLOPT_WRITEDATA, imageFile);


    CURLcode result = curl_easy_perform(curl);
    if (result == CURLE_OK){
        // wait
//        int maxj = 1;
//        if (dir == CAM_CENTER) maxj=100; //wait more!
//        for(int j=0;j<maxj;j++)
//            for(long int i=0; i<MOVE_DELAY*degree*maxj;i++);

        curl_easy_cleanup(curl);
        changeDesiredPosition(dir,degree);
        return true;
    }
    curl_easy_cleanup(curl);
    return false;
}


bool FoscamCameraControl::isReady(){
    updatePosition();
    //Done this way because cameraControl does not support correctly 2 dimension movements.
    switch (lastDirection){
        case CAM_UP: return (actualPosition.y == desiredPosition.y);
        case CAM_DW: return (actualPosition.y == desiredPosition.y);
        case CAM_RH: return (actualPosition.x == actualPosition.x);
        case CAM_LF: return (actualPosition.x == actualPosition.x);
        case CAM_CENTER: return (difftime(time(NULL),lastMove) > CENTER_DELAY);
        default: return true;
    }
    return true;
}

// Like move, but with a default degree.
// Comment: the onestep parameter without degree moves the camera approx. 10 degree,
// now the step is setted by software (not the camera) and uses the degree param.
bool FoscamCameraControl::moveStep(int dir){
    move(dir,STEP_DEGREE);
}

//Checks wheter the movement is possible considering the angle
bool FoscamCameraControl::checkMovement(int dir, int degree){
    if(dir==CAM_CENTER) return true;
    updatePosition(); //Update actualPosition
    if(isCoordinates)
        switch(dir){
            case CAM_UP: return ((actualPosition.y+degree) <= MAX_UP);
            case CAM_DW: return ((actualPosition.y-degree) >= MAX_DW);
            case CAM_RH: return ((actualPosition.x+degree) <= MAX_RH);
            case CAM_LF: return ((actualPosition.x-degree) >= MAX_LF);
            case CAM_STOP: return true;
            default: std::cout << "Warning, strange command : DIR="<< dir << " DEG=" << degree  <<"\n";
        }
    return true;
}

void FoscamCameraControl::changeDesiredPosition(int dir, int degree){
    if (dir == CAM_CENTER) isCoordinates=true;
    //Only work is coordinate work is activated
    if(isCoordinates)
        switch(dir){
            case CAM_UP: desiredPosition.y= actualPosition.y + degree;
                         break;
            case CAM_DW: desiredPosition.y =actualPosition.y- degree;
                         break;
            case CAM_RH: desiredPosition.x = actualPosition.x + degree;
                         break;
            case CAM_LF: desiredPosition.x =actualPosition.x - degree;
                         break;
            case CAM_CENTER: actualPosition.x = 0;
                             actualPosition.y = 0;
                             desiredPosition.x = 0;
                             desiredPosition.y = 0;
                             break;
            case CAM_STOP: desiredPosition.x = actualPosition.x;
                           desiredPosition.y = actualPosition.y;
                           break;
            default: std::cout << "Warning, strange command : DIR="<< dir << " DEG=" << degree  <<"\n";
        }
}

    //Moves the camera to a position
    //WARNING: it has a time waiting problem
bool FoscamCameraControl::move(Coordinates_t coord){
    bool result = true;
    updatePosition();
    //Only move in one direction
    if ( rand() %2 == 1){
        int x = coord.x - actualPosition.x;
        if(x>0) result &= move(CAM_RH,x);
         else   result &= move(CAM_LF,x);
    }else{
        int y = coord.y - actualPosition.y;
        if(y>0) result &= move(CAM_UP,y);
        else  result &= move(CAM_DW,y);
    }
    return result;
}


//Moves the camera to a position, asuming current camera center is (0,0)
bool FoscamCameraControl::moveRelative(Coordinates_t coord){
    Coordinates_t newC;
    updatePosition();
    newC.x = coord.x + actualPosition.x;
    newC.y = coord.y + actualPosition.y;
    if ( abs(coord.x) > abs(coord.y)){
        //Move only x
        newC.y = actualPosition.y;
    }else{
        //Move only y
        newC.x = actualPosition.x;
    }
    std::cout << "relToAbs: " << coord.x << "," << coord.y << ") -> (" << newC.x << "," << newC.y << ")\n" ;
    return move(newC);
}



//Updates and returns the actual position of the camera
Coordinates_t FoscamCameraControl::updatePosition(){

    if(lastDirection==CAM_CENTER){
        actualPosition.x = 0;
        actualPosition.y = 0;
        return actualPosition;
    }

    //get actualTime and compare it to lastMove
    timeb tb;
    ftime(&tb);
    int now, diff;
    do{
        now = tb.millitm + (tb.time & 0xfffff) * 1000;
        diff = now - lastMove;
        std::cout << ".";
        if (diff ==0) wait(1);
    } while (diff==0);
    std::cout << "diff=" << diff << "= " << now << "-"  << lastMove << std::endl;

    //Update the actualCoordinates
    switch(lastDirection){
        case CAM_UP: actualPosition.y = (abs(desiredPosition.y - actualPosition.y) < diff * VER_SPEED ) ?
                                desiredPosition.y : actualPosition.y + diff * VER_SPEED;
                     break;
        case CAM_DW: actualPosition.y = (abs(actualPosition.y - desiredPosition.y) < diff * VER_SPEED ) ?
                                desiredPosition.y : actualPosition.y - diff * VER_SPEED;
                     break;
        case CAM_RH: actualPosition.x = (abs(desiredPosition.x - actualPosition.x) < diff * HOR_SPEED ) ?
                                desiredPosition.x : actualPosition.x + diff * HOR_SPEED;
                     break;
        case CAM_LF: actualPosition.x = (abs(actualPosition.x - desiredPosition.x) < diff * HOR_SPEED ) ?
                                desiredPosition.x : actualPosition.x - diff * HOR_SPEED;
                     break;
        default : break;
    }
    lastMove = now;
    return actualPosition;
}

    //Returns the lastest frame obtained from the camera
cv::Mat* FoscamCameraControl::getFrame(){
    //Stablish Connection
    CURL* connection;
    CURLcode connection_result;
    //Curl response is saved in a file
    FILE* imageFile;

    connection = curl_easy_init();
    if (connection){
        imageFile = fopen("img.jpg", "wb");
        if (imageFile == NULL){ std::cout << "File handling error"; }

        char url[75];
        sprintf((char*)&url, "%s/snapshot.cgi?user=%s&pwd=%s",cameraURL, user, pwd);
        curl_easy_setopt(connection, CURLOPT_URL, url);


//        curl_easy_setopt(connection, CURLOPT_URL, "http://192.168.1.200/snapshot.cgi?user=admin&pwd=31415LAS");
        //MUst create a writefunction to improve performance and avoid file handilng
        curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(connection, CURLOPT_WRITEDATA, imageFile);

        connection_result = curl_easy_perform(connection);
        if( connection_result )
            std::cout << "Error perform";

        curl_easy_cleanup(connection);
        fclose(imageFile);

        //Read image from file
        cv::Mat* image = new cv::Mat(cv::imread("img.jpg",1 ));
        return image;
    }
    return new cv::Mat();
}

//Triggers the alarm
bool FoscamCameraControl::triggerAlarm(){
        //TODO How?
        return FAILURE;
    }


//Stops movement
bool FoscamCameraControl::stop(){
    return (this->move(CAM_STOP,0));
}
