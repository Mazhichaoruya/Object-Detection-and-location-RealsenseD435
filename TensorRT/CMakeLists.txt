cmake_minimum_required(VERSION 3.17)
project(Yolov5_trt)

set(CMAKE_CXX_STANDARD 17)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)

set(CUDA_NVCC_PLAGS ${CUDA_NVCC_PLAGS};-std=c++11;-g;-G;-gencode;arch=compute_30;code=sm_30)
# eigen
include_directories( "/usr/include/eigen3/" )
# Find librealsense2 installed package
find_package(realsense2 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)
# include and link dirs of cuda and tensorrt, you need adapt them if yours are different
# cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)
# tensorrt
include_directories(/usr/include/x86_64-linux-gnu/)
link_directories(/usr/lib/x86_64-linux-gnu/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

cuda_add_library(myplugins SHARED ${PROJECT_SOURCE_DIR}/yololayer.cu)
target_link_libraries(myplugins nvinfer cudart)
add_subdirectory(./dlib dlib_build)
find_package(OpenCV)
include_directories(OpenCV_INCLUDE_DIRS)
add_executable(Yolov5_trt src/main.cpp src/Position_Transform.cpp src/Realsense_config.cpp src/Objection.cpp  src/Algorithm_Objection_3D.cpp  src/Visualizer.cpp)
#target_link_libraries(Yolov5_trt nvinfer)
#target_link_libraries(Yolov5_trt cudart)
#target_link_libraries(Yolov5_trt myplugins)
target_link_libraries(Yolov5_trt nvinfer cudart myplugins ${OpenCV_LIBS} ${realsense2_LIBRARY}  dlib::dlib)

add_definitions(-O2 -pthread)