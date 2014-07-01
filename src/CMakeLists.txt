cmake_minimum_required(VERSION 2.8)
project( Turret )

find_package( OpenCV REQUIRED )
find_package(CURL)

add_executable( main main.cpp AbstractDetector.cpp Drawer.cpp FoscamCameraControl.cpp HorizontalTracker.cpp ObjectSelector.cpp squareObjectSelector.cpp OpenCVDetector.cpp testPrecision.cpp utils.cpp FaceDetector.cpp)

if(WIN32)
  target_link_libraries( main wsock32 ws2_32)
endif()

target_link_libraries( main ${OpenCV_LIBS} )
target_link_libraries( main ${CURL_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../res/haarcascade_frontalface_alt.xml DESTINATION ${CMAKE_BINARY_DIR}/res)
