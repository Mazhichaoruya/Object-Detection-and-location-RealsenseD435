# Objection-Detection-and-location-RealsenseD435

##Requirements
Ubuntu18.04 OR 16.04;
[Opencv 4.x](https://github.com/opencv/opencv.git)
C++ 11_std At least,I used the C++ 17 std;
Eigen3 :in absolutely Path /usr/local/eigen3;
Cmake>= 3.17;
PCL lib>=1.7.1;
[Intel Realsense SDK >=2.0 ](https://github.com/IntelRealSense/librealsense.git)
[Yolov3 by Darknet](https://pjreddie.com/darknet/yolo/)

##How to use
```Bash
git clone https://github.com/Mazhichaoruya/Objection-Detection-and-location-RealsenseD435.git
cd  Objection-Detection-and-location-RealsenseD435/C++/Yolo_model
wget https://pjreddie.com/media/files/yolov3.weights ;wget https://pjreddie.com/media/files/yolov3-tiny.weights
cd ..
mkdir bulid; cd build
cmake ..
make
./DNN_Yolo
```
You can use your weight by Darknet or others supported by DNN .
Attention:Default parameter on line 228 in src/main.cpp
```cpp
    net.setPreferableBackend(DNN_BACKEND_OPENCV);// DNN_BACKEND_INFERENCE_ENGINE 
```
if you have IntelCore CPU you can chose "DNN_BACKEND_INFERENCE_ENGINE"to accelerate youe model--Openvino;`
But you should make sure your CPU is Intel and the Contrib of Opencv has been installed.
##Example
![image](https://github.com/Mazhichaoruya/Objection-Detection-and-location-RealsenseD435/C++/output.gif)   
The windows show the Classname and Position in Camera coordinate system of Objection
