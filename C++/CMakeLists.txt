cmake_minimum_required(VERSION 3.17)
project(DNN_YOLO)

set(CMAKE_CXX_STANDARD 17)
# opencv
set(OpenCV_DIR "/usr/local/share/opencv4")
find_package(OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#[[boost]]
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
# eigen
include_directories( "/usr/include/eigen3/" )
# Find librealsense2 installed package
find_package(realsense2 REQUIRED)
# pcl
#set(PCL_DIR "/usr/lib/x86_64-linux-gnu/")
#find_package(PCL REQUIRED  COMPONENTS common io)
#include_directories(${PCL_INCLUDE_DIRS})#${PCL_INCLUDE_DIRS} "/usr/include/pcl-1.8/"
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions( ${PCL_DEFINITIONS} )
#list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")#解决方案，否则编译器不通过
#include_directories( "/usr/include/librealsense2/" )
add_subdirectory(./dlib dlib_build)
add_executable(DNN_YOLO  src/Main.cpp include/Position_Transform.h src/Position_Transform.cpp src/Realsense_config.cpp src/Objection.cpp include/Objection.h src/Algorithm_Objection_3D.cpp include/Algorithm_Objection_3D.h src/Visualizer.cpp include/Visualizer.h) ##Yolov3_tiny.cpp  set_D435.cpp rs-dnn.cpp rs-pointcloud.cpp src/Realsense_config.cpp src/GetCameraReferance.cpp src/Test.cpp
target_link_libraries(DNN_YOLO ${OpenCV_LIBS} ${Boost_LIBRARIES} ${realsense2_LIBRARY}  dlib::dlib )#${PCL_LIBRARIES} ${PCL_LIBRARIES} ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES}
