# Objection-Detection-and-location-RealsenseD435

## Requirements
Ubuntu18.04 OR 16.04;<br>
[Opencv 4.x](https://github.com/opencv/opencv.git)<br>
C++ 11_std At least,I used the C++ 17 std;<br>
Eigen3 :in absolutely Path /usr/local/eigen3;<br>
Cmake>= 3.17;<br>
PCL lib>=1.7.1;<br>
[Intel Realsense SDK >=2.0 ](https://github.com/IntelRealSense/librealsense.git)<br>
[Yolov3 by Darknet](https://pjreddie.com/darknet/yolo/)<br>

## How to use
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
You can use your weight by Darknet or others supported by DNN .<br>
Attention:Default parameter on line 228 in src/main.cpp<br>
```cpp
    net.setPreferableBackend(DNN_BACKEND_OPENCV);// DNN_BACKEND_INFERENCE_ENGINE 
```
if you have IntelCore CPU you can chose "DNN_BACKEND_INFERENCE_ENGINE"to accelerate youe model--Openvino;<br>
But you should make sure your CPU is Intel and the Contrib of Opencv has been installed.<br>
## Example
![gif](https://github.com/Mazhichaoruya/Objection-Detection-and-location-RealsenseD435/C++/output.gif)  <br>
The windows show the Classname and Position in Camera coordinate system of Objection
