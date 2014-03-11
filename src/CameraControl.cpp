#include "CameraControl.h"
//#include "AlarmTriggerer.h"
#include "curl/curl.h"
#include <iostream>
#include <cstdlib>


CameraControl::CameraControl(/*AlarmTriggerer* at,*/ char* url, char* login){
 //   this.alarmTriggerer = at;
    this->cameraURL = url;

    //Connect to camera
    curl = curl_easy_init();
    if(!curl){
        std::cout << "No curl";
        std::exit(1);
    }

    curl_easy_setopt(curl,CURLOPT_USERPWD,login);

}

    //Moves the camera in a specific direction and stops
bool CameraControl::move(int dir, int degree){
    char url[75];

    sprintf((char*)&url, "%s/decoder_control.cgi?command=%i&onestep=1&degree=%i",cameraURL, dir, degree);

	curl_easy_setopt(curl, CURLOPT_URL, url);

    CURLcode result = curl_easy_perform(curl);
    return (result == CURLE_OK);
}


    //Moves the camera to a position
bool CameraControl::move(Coordinates_t coord){
    //TODO
    return FAILURE;
}

    //Return the X and Y position of the camera
Coordinates_t CameraControl::getCoordinates(){
    //TODO
    Coordinates_t result;
    return result;
}

    //Returns the lastest frame obtained from the camera
cv::Mat* CameraControl::getFrame(){
    //TODO
    return NULL;
}

//Triggers the alarm
bool CameraControl::triggerAlarm(){
        //TODO
        return FAILURE;
    }


