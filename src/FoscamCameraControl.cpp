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
    moveStep(CAM_CENTER);
    isCoordinates = true;
    coordinates.x = 0;
    coordinates.y = 0;
}


    //Moves the camera in a specific direction and stops
bool FoscamCameraControl::move(int dir, int degree){

    if (!checkMovement(dir,degree)){
        std::cout << "Movement denied - dir:" << dir << " degree:" << degree << "\n";
        return false;
    }

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
        updateCoordinates(dir,degree);
        return true;
    }
    curl_easy_cleanup(curl);
    return false;
}

// Like move, but with a default degree.
// Comment: the onestep parameter without degree moves the camera approx. 10 degree,
// now the step is setted by software (not the camera) and uses the degree param.
bool FoscamCameraControl::moveStep(int dir){
    move(dir,STEP_DEGREE);
}

//Checks wheter the movement is possible considering the angle
bool FoscamCameraControl::checkMovement(int dir, int degree){
    if(isCoordinates)
        switch(dir){
            case CAM_UP: return ((coordinates.y+degree) <= MAX_UP);
            case CAM_DW: return ((coordinates.y-degree) >= MAX_DW);
            case CAM_RH: return ((coordinates.x+degree) <= MAX_RH);
            case CAM_LF: return ((coordinates.x-degree) >= MAX_LF);
            case CAM_CENTER: return true;
            case CAM_STOP: return true;
            default: std::cout << "Warning, strange command";
        }
    return true;
}

void FoscamCameraControl::updateCoordinates(int dir, int degree){
    //Only work is coordinate work is activated
    if(isCoordinates)
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
bool FoscamCameraControl::move(Coordinates_t coord){
    int x = coord.x - coordinates.x;
        if(x>0) move(x,CAM_LF);
         else   move(x,CAM_RH);

    return FAILURE;
}

    //Return the X and Y position of the camera
Coordinates_t FoscamCameraControl::getCoordinates(){
    return coordinates;
}

    //Returns the lastest frame obtained from the camera
cv::Mat* FoscamCameraControl::getFrame(){
    //Stablish Connection
    CURL* connection;
    CURLcode connection_result;
    //Curl response is saved in a file
    FILE* imageFile;
    void* charImage = malloc(sizeof(char) * 320000);

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
