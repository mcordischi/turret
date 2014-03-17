#include "CameraControl.h"
#include <iostream>
#include <cstdlib>


CameraControl::CameraControl(/*AlarmTriggerer* at,*/ char* url, char* user, char* pwd){
 //   this.alarmTriggerer = at;
    this->cameraURL = url;
    this->user = user;
    this->pwd = pwd;

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




    //Moves the camera in a specific direction and stops
bool CameraControl::move(int dir, int degree){

    if (!checkMovement(dir,degree)){
        std::cout << "Movement denied - Cannot move that direction anymore";
        return false;
    }

    char url[75];

    sprintf((char*)&url, "%s/decoder_control.cgi?command=%i&onestep=1&degree=%i",cameraURL, dir, degree);

    CURL* curl;
    curl = curl_easy_init();

    char auxURL[100];
    sprintf((char*)&auxURL, "%s:%s",user,pwd);
    curl_easy_setopt(curl,CURLOPT_USERPWD,auxURL);



	curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    CURLcode result = curl_easy_perform(curl);
    if (result == CURLE_OK){
        // wait
//        int maxj = 1;
//        if (dir == CAM_CENTER) maxj=100; //wait more!
//        for(int j=0;j<maxj;j++)
//            for(long int i=0; i<MOVE_DELAY*degree*maxj;i++);

        curl_easy_cleanup(curl);
        updateCoordinates(dir,degree);
        return true;
    }
    curl_easy_cleanup(curl);
    return false;
}

// Like move, but with a given degree.
// Comment: the onestep parameter without degree moves the camera approx. 10 degree,
// now the step is setted by software (not the camera) and uses the degree param.
bool CameraControl::moveStep(int dir){
    move(dir,STEP_DEGREE);
}

//Checks wheter the movement is possible considering the angle
bool CameraControl::checkMovement(int dir, int degree){
    switch(dir){
        case CAM_UP: return ((coordinates.y+degree) <= MAX_UP);
        case CAM_DW: return ((coordinates.y-degree) >= MAX_DW);
        case CAM_RH: return ((coordinates.x+degree) <= MAX_RH);
        case CAM_LF: return ((coordinates.x-degree) >= MAX_LF);
        case CAM_CENTER: return true;
        default: std::cout << "Warning, strange command";
        }
    return true;
}

void CameraControl::updateCoordinates(int dir, int degree){
        switch(dir){
            case CAM_UP: coordinates.y += degree;
                         break;
            case CAM_DW: coordinates.y -= degree;
                         break;
            case CAM_RH: coordinates.x += degree;
                         break;
            case CAM_LF: coordinates.x -= degree;
                         break;
            case CAM_CENTER: coordinates.x = 0;
                             coordinates.y = 0;
                             break;
            default: std::cout << "Warning, strange command";
        }
}

    //Moves the camera to a position
bool CameraControl::move(Coordinates_t coord){
    //TODO
    return FAILURE;
}

    //Return the X and Y position of the camera
Coordinates_t CameraControl::getCoordinates(){
    return coordinates;
}

    //Returns the lastest frame obtained from the camera
cv::Mat* CameraControl::getFrame(){
    cv::Mat* frame;
    (*cvCamera) >> (*frame);
    return frame;
}

//Triggers the alarm
bool CameraControl::triggerAlarm(){
        //TODO How?
        return FAILURE;
    }


