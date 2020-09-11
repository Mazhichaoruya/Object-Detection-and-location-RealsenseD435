cmake_minimum_required(VERSION 3.17)
project(DNN_YOLO)

set(CMAKE_CXX_STANDARD 17)
# opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#[[boost]]
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
# eigen
include_directories( "/usr/include/eigen3/" )
# Find librealsense2 installed package
find_package(realsense2 REQUIRED)
# pcl
find_package( PCL  REQUIRED  )
#include_directories( ${PCL_INCLUDE_DIRS} )
#add_definitions( ${PCL_DEFINITIONS} )
#include_directories( "/usr/include/librealsense2/" )
add_executable(DNN_YOLO  src/Main.cpp include/Position_Transform.h src/Position_Transform.cpp src/Realsense_config.cpp src/Objection.cpp include/Objection.h) ##Yolov3_tiny.cpp  set_D435.cpp rs-dnn.cpp rs-pointcloud.cpp src/Realsense_config.cpp src/GetCameraReferance.cpp src/Test.cpp
target_link_libraries(DNN_YOLO ${OpenCV_LIBS} ${Boost_LIBRARIES} ${realsense2_LIBRARY}   )#${PCL_LIBRARIES}
