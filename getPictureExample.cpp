#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "curl/curl.h"

int main(){
    CURL* connection;
    CURLcode connection_result;
    FILE* imageFile;
    void* charImage = malloc(sizeof(char) * 320000);
//    cv::Mat * image ;

    connection = curl_easy_init();
    if (connection){
        imageFile = fopen("img.jpg", "wb");
        if (imageFile == NULL){ std::cout << "File handling error"; }


        curl_easy_setopt(connection, CURLOPT_URL, "http://192.168.1.200/snapshot.cgi?user=admin&pwd=31415LAS");
        //MUst create a writefunction to imporve performance and avoid file handilng
        curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(connection, CURLOPT_WRITEDATA, imageFile);

        connection_result = curl_easy_perform(connection);
        if( connection_result )
            std::cout << "Error perform";
        curl_easy_cleanup(connection);

        fclose(imageFile);

        cv::Mat* image = new cv::Mat(cv::imread("img.jpg",1 ));
/*
        cv::Mat image(640,480,CV_8UC1,charImage,sizeof(char));
i*/
        cv::namedWindow( "test", CV_WINDOW_AUTOSIZE );
        cv::imshow( "test", *image );

        cv::waitKey(0);
            }
    return 0;
}
