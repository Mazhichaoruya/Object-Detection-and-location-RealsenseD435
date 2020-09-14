# Objection-Detection-and-location-RealsenseD435

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
### Python Version
pyrealsense2.x  
Opencv-python   
numpy  
with same as C++ version,the SDK of realsense D435 must have been installed
## How to use
### C++
```Bash
git clone https://github.com/Mazhichaoruya/Objection-Detection-and-location-RealsenseD435.git
cd  Objection-Detection-and-location-RealsenseD435/Yolo_model
wget https://pjreddie.com/media/files/yolov3.weights ;wget https://pjreddie.com/media/files/yolov3-tiny.weights
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
Attention:Default parameter on line 228 in src/main.cpp   
```cpp
    net.setPreferableBackend(DNN_BACKEND_OPENCV);// DNN_BACKEND_INFERENCE_ENGINE 
```
if you have IntelCore CPU you can chose "DNN_BACKEND_INFERENCE_ENGINE"to accelerate youe model--Openvino;<br>
But you should make sure your CPU is Intel and the Contrib of Opencv has been installed.
## Python
When you install the essential job ,just run the main.py,the model will start to work.
```
cd Python   
python3 main.py
```
## Example
![gif](https://github.com/Mazhichaoruya/Objection-Detection-and-location-RealsenseD435/blob/master/Gif/C%2B%2B.gif)  
The windows show the Classname and Position in Camera coordinate system of Objection
## To be continue
I will continue to slove the problems of loss targets' depth information when one Box have two or more targets  
Find some faster Models of Objection and Try to use cuda acceleration   
Take it to rospack and run it on ROS robot with slam