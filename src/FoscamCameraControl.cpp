#include "FoscamCameraControl.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"
//#include "AlarmTriggerer.h"


FoscamCameraControl::FoscamCameraControl(/*AlarmTriggerer* at,*/ char* url, char* user, char* pwd){
 //   this.alarmTriggerer = at;
    this->cameraURL = url;
    this->user = user;
    this->pwd = pwd;
    charImage = malloc(sizeof(char) * 320000);
}

bool FoscamCameraControl::startCoordinates(){
    lastMove = getNow();

    move(CAM_CENTER,0);
}


    //Moves the camera in a specific direction and stops
bool FoscamCameraControl::move(int dir, int degree){

    //If the movement is less than a step, avoid moving to reduce errors
    if(dir != CAM_CENTER && degree < STEP_DEGREE) return true;

       //TEMP - If camera not ready, don't move
    if (!isReady()){
        std::cout << "Camera on movement\n" ;
        return false;
    }

    if (!checkMovement(dir,degree)){
        std::cout << "Movement denied - dir:" << dir << " degree:" << degree << "\n";
        return false;
    }

    return sendMovementRequest(dir,degree);
}


bool FoscamCameraControl::isReady(){
    //Update current position before continuing
    updatePosition();
    std::cout << "Position=(" << currentPosition.x << "," << currentPosition.y << ")" << std::endl;
    //Done this way because cameraControl does not support correctly 2 dimension movements.
    switch (lastDirection){
        case CAM_UP: return (currentPosition.y == desiredPosition.y);
        case CAM_DW: return (currentPosition.y == desiredPosition.y);
        case CAM_RH: return (currentPosition.x == desiredPosition.x);
        case CAM_LF: return (currentPosition.x == desiredPosition.x);
        case CAM_CENTER: return ((getNow()-lastMove) > CENTER_DELAY); //TODO Check
        default: return true;
    }
    return true;
}

// Like move, but with a default degree.
// Comment: the onestep parameter without degree moves the camera approx. 10 degree,
// now the step is setted by software (not the camera) and uses the degree param.
bool FoscamCameraControl::moveStep(int dir){
    return move(dir,STEP_DEGREE);
}

//Checks wheter the movement is possible considering the angle
bool FoscamCameraControl::checkMovement(int dir, int degree){
    if(dir==CAM_CENTER) return true;
    updatePosition(); //Update currentPosition
    if(isCoordinates)
        switch(dir){
            case CAM_UP: return ((currentPosition.y+degree) <= MAX_UP);
            case CAM_DW: return ((currentPosition.y-degree) >= MAX_DW);
            case CAM_RH: return ((currentPosition.x+degree) <= MAX_RH);
            case CAM_LF: return ((currentPosition.x-degree) >= MAX_LF);
            case CAM_STOP: return true;
            default: std::cout << "Warning, strange command : DIR="<< dir << " DEG=" << degree  <<"\n";
        }
    return true;
}

void FoscamCameraControl::changeDesiredPosition(int dir, int degree){

    //Set last move to NOW
    lastMove = getNow();//now
    //set lastDirection
    lastDirection = dir;

    if (dir == CAM_CENTER) isCoordinates=true;
    //Only work is coordinate work is activated
    if(isCoordinates)
        switch(dir){
            case CAM_UP: desiredPosition.y= currentPosition.y + degree;
                         break;
            case CAM_DW: desiredPosition.y =currentPosition.y- degree;
                         break;
            case CAM_RH: desiredPosition.x = currentPosition.x + degree;
                         break;
            case CAM_LF: desiredPosition.x =currentPosition.x - degree;
                         break;
            case CAM_CENTER: currentPosition.x = 0;
                             currentPosition.y = 0;
                             desiredPosition.x = 0;
                             desiredPosition.y = 0;
                             break;
            case CAM_STOP: desiredPosition.x = currentPosition.x;
                           desiredPosition.y = currentPosition.y;
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
        int x = coord.x - currentPosition.x;
        if(x>0) result &= move(CAM_RH,x);
         else   result &= move(CAM_LF,x);
    }else{
        int y = coord.y - currentPosition.y;
        if(y>0) result &= move(CAM_UP,y);
        else  result &= move(CAM_DW,y);
    }
    return result;
}


//Moves the camera to a position, asuming current camera center is (0,0)
bool FoscamCameraControl::moveRelative(Coordinates_t coord){
    Coordinates_t newC;
    updatePosition();
    newC.x = coord.x + currentPosition.x;
    newC.y = coord.y + currentPosition.y;
    if ( abs(coord.x) > abs(coord.y)){
        //Move only x
        newC.y = currentPosition.y;
    }else{
        //Move only y
        newC.x = currentPosition.x;
    }
    std::cout << "relToAbs: " << coord.x << "," << coord.y << ") -> (" << newC.x << "," << newC.y << ")\n" ;
    return move(newC);
}



//Updates and returns the current position of the camera
Coordinates_t FoscamCameraControl::updatePosition(){

    if(lastDirection==CAM_CENTER){
        currentPosition.x = 0;
        currentPosition.y = 0;
        return currentPosition;
    }

    //get currentTime and compare it to lastMove
    double now, diff;
    do{
        now = getNow();
        diff = now - lastMove;
        std::cout << ".";
        if (diff ==0) wait(1);
    } while (diff==0);
    std::cout << "diff=" << diff << "= " << now << "-"  << lastMove << std::endl;

    //Update the currentCoordinates
    switch(lastDirection){
        case CAM_UP: if (abs(desiredPosition.y-currentPosition.y) < diff * VER_SPEED){ //if movement needed < movement done
                             //Movement finished
                             currentPosition.y = desiredPosition.y;
                             //TODO perform forced move on X
                             }
                    break;
        case CAM_DW:  if (abs(desiredPosition.y-currentPosition.y) < diff * VER_SPEED){
                             currentPosition.y = desiredPosition.y;
                             }
                    break;
        case CAM_RH:  if (abs(desiredPosition.x-currentPosition.x) < diff * HOR_SPEED){
                             currentPosition.x = desiredPosition.x;
                             }
                    break;

        case CAM_LF:  if (abs(desiredPosition.x-currentPosition.x) < diff * HOR_SPEED){
                             currentPosition.x = desiredPosition.x;
                             }
                    break;

    }


    /* DELETE on next code review
    switch(lastDirection){
        case CAM_UP: currentPosition.y = (abs(desiredPosition.y - currentPosition.y) < diff * VER_SPEED ) ?
                                desiredPosition.y : currentPosition.y + diff * VER_SPEED;
                     break;
        case CAM_DW: currentPosition.y = (abs(currentPosition.y - desiredPosition.y) < diff * VER_SPEED ) ?
                                desiredPosition.y : currentPosition.y - diff * VER_SPEED;
                     break;
        case CAM_RH: currentPosition.x = (abs(desiredPosition.x - currentPosition.x) < diff * HOR_SPEED ) ?
                                desiredPosition.x : currentPosition.x + diff * HOR_SPEED;
                     break;
        case CAM_LF: currentPosition.x = (abs(currentPosition.x - desiredPosition.x) < diff * HOR_SPEED ) ?
                                desiredPosition.x : currentPosition.x - diff * HOR_SPEED;
                     break;
        default : break;
    }
    lastMove = now;*/
    return currentPosition;
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


//curl GET request
bool FoscamCameraControl::sendMovementRequest(int dir, int degree){
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


