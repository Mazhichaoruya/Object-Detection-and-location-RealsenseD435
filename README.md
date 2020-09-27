# Object-Detection-and-location-RealsenseD435
I just Update the C++ Version this time,Python Version will be updated when I have free time.maybe next week.
## ROS version [ROS-Object-Detection-2Dto3D-RealsenseD435](https://github.com/Mazhichaoruya/ROS-Object-Detection-2Dto3D-RealsenseD435.git)
## Requirements
### C++ Version
Ubuntu18.04 OR 16.04  
[Opencv 4.x](https://github.com/opencv/opencv.git)  
C++ 11_std At least,I used the C++ 17 std  
Eigen3 :in absolutely Path /usr/local/eigen3  
Cmake>= 3.17  
PCL lib>=1.7.1  
[Intel Realsense SDK >=2.0 ](https://github.com/IntelRealSense/librealsense.git)  
[Yolov3 by Darknet](https://pjreddie.com/darknet/yolo/)    
[Dlib](http://dlib.net/) #I had pushed on this Github repository on :Object-Detection-and-location-RealsenseD435/C++/dlib.zip,uzip it on this Path.
You can DownLoad the lastest version too.
### Python Version
pyrealsense2.x  
Opencv-python   
numpy  
with same as C++ version,the SDK of realsense D435 must have been installed
## How to use
### C++
```Bash
git clone https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435.git
cd  Object-Detection-and-location-RealsenseD435/Yolo_model
wget https://pjreddie.com/media/files/yolov3.weights ;wget https://pjreddie.com/media/files/yolov3-tiny.weights
cd Object-Detection-and-location-RealsenseD435/C++/;uzip dlib.zip
```
For avoiding unreasonable troubles,I used the absolute path,so you have to change the path in src/main.cpp   
on line 25-27
```cpp
String yolo_tiny_model ="/home/mzc/code/CLionProjects/DNN435/Yolo_model/yolov3.weights";
String yolo_tiny_cfg =  "/home/mzc/code/CLionProjects/DNN435/Yolo_model/yolov3.cfg";
String classname_path="/home/mzc/code/CLionProjects/DNN435/Yolo_model/object_detection_classes_yolov3.txt";
``` 
You can use your weight by Darknet or others supported by DNN too   
```
cd ..
mkdir bulid; cd build
cmake ..
make
./DNN_Yolo
```
Attention:Default parameter on line 251 and 252 in src/main.cpp   
```cpp
    net.setPreferableBackend(DNN_BACKEND_OPENCV);// DNN_BACKEND_INFERENCE_ENGINE DNN_BACKEND_CUDA
    net.setPreferableTarget(DNN_TARGET_CPU);//DNN_TARGET_CUDA
```
if you have IntelCore CPU you can chose "DNN_BACKEND_INFERENCE_ENGINE"to accelerate youe model--Openvino;<br>
But you should make sure your CPU is Intel and the Contrib of Opencv has been installed.  
If you have GPU(From Nvidia),You can Think about The Cuda acceleration.Before this you should reinstall Your Opencv(Version Most>4.2) with This:[OpenCV_DNN](https://medium.com/@sb.jaduniv/how-to-install-opencv-4-2-0-with-cuda-10-1-on-ubuntu-20-04-lts-focal-fossa-bdc034109df3)  
Open the Cuda Setting when CMake.
## Python
When you install the essential job ,just run the main.py,the model will start to work.
```
cd Python   
python3 main.py
```
## Example
Old_version:  
![Old_version](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/Gif/Old_version.gif)  
The windows show the Classname and Position in Camera coordinate system of Object  
New version on 9-21:
RGBD and Center position:  
![RGBD](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/Gif/RGBD.gif)
Point Cloud of Objects: 
![PointCloud](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/Gif/PointCloud.gif) 
## To be continue
Find some faster Models of Object  
About Cuda Accelerate ,This Blog Front. 
For C++ Version Just change two parameters On on line 251,252 in src/main.cpp 
Take it to rospack and run it on ROS robot with slam
