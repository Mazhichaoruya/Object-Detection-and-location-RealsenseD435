# Object-Detection-and-location-RealsenseD435
I just Update the C++ Version this time,Python Version will be updated when I have free time.maybe next week.
## ROS version [ROS-Object-Detection-2Dto3D-RealsenseD435](https://github.com/Mazhichaoruya/ROS-Object-Detection-2Dto3D-RealsenseD435.git)
## Object Dection with SLAM ,With SLAM(SC-Lego-LOAM)  :[Perception-of-Autonomous-mobile-robot](https://github.com/Mazhichaoruya/Perception-of-Autonomous-mobile-robot)
## Requirements
### DNN Version
Ubuntu18.04 OR 16.04  
[Opencv 4.x](https://github.com/opencv/opencv.git)  
C++ 11_std At least,I used the C++ 17 std  
Eigen3 :in absolutely Path /usr/local/eigen3  
Cmake>= 3.17  
PCL lib>=1.7.1  
[Intel Realsense SDK >=2.0 ](https://github.com/IntelRealSense/librealsense.git)  
[Yolov3/v4 by Darknet](https://pjreddie.com/darknet/yolo/)    
[Dlib](http://dlib.net/) #I had pushed on this Github repository on :Object-Detection-and-location-RealsenseD435/C++/dlib.zip,uzip it on this Path.
You can DownLoad the lastest version too.  
The config weight and cfg is Yolov4 based EfficentNetB0 ,it is a smaller model than yolov4_tiny but the result better.it can be run over 10FPS on CPU.
### Python Version
pyrealsense2.x  
Opencv-python   
numpy  
with same as C++ version,the SDK of realsense D435 must have been installed
### TenorRT version 
A Nvidia GPU is essential.I used Yolov5 by pytorch.
[tensorrtx](https://github.com/wang-xinyu/tensorrtx)  [Yolov5](https://github.com/ultralytics/yolov5)  
## How to use
### DNN
```Bash
git clone https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435.git
cd  Object-Detection-and-location-RealsenseD435/DNN/engine/
wget https://pjreddie.com/media/files/yolov3.weights ;wget https://pjreddie.com/media/files/yolov3-tiny.weights
cd Object-Detection-and-location-RealsenseD435/;uzip dlib.zip  
mv dlib /DNN
```
you Can change the engine path in src/main.cpp   
on line 25-27
```cpp
String yolo_tiny_model ="../engine/yolov3.weights";
String yolo_tiny_cfg =  "../engine/yolov3.cfg";
String classname_path="../engine/coco.names";
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
### TensorRT
At first,Go to [tensorrtx](https://github.com/wang-xinyu/tensorrtx)get the tensorrt engine(Before this you must had installed Ubuntu,CUDA,TensorRT).  
Then move the .enigine files to Yolov5-TensorRT-AGX_Xavier/engine
```cpp
#define VIDEO_TYPE (0) //0 means laptop camera ;1 means images,2 means Videos,3 means RealsenseD435;
#define NET x  // s m l x
```
You can change The Define type as you like，From src/main.cpp.  
```
cd TensorRT;
mkdir bulid; cd build
cmake ..
make -j12
./Yolov5_trt
```
## Example
DNN version on 9-21:
RGBD and Center position:  
![RGBD](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/Gif/RGBD.gif)
Point Cloud of Objects: 
![PointCloud](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/Gif/PointCloud.gif) 
[![Watch the video](https://github.com/Mazhichaoruya/Object-Detection-and-location-RealsenseD435/blob/master/input/VOC/2007_000042.jpg)](https://www.youtube.com/watch?v=HPi-RQgEvA0)
## Next
The ROS and SLAM version had been uploaded,But it is just a Beginning,I will do more jod form my Graduation Project，if you were Interested in this, welcome to follow me！

#深度相机目标检测DNN&TensorRT
## DNN版本
请参考英文注释操作编译，默认使用CPU模型，Opencv 4.x，当使用给予EfficentNetB0轻量级网络的Yolov4时，在CPU上可以超过10FPS。实测AMD R5 4600H 15FPS。
## TensorRT版本
仅支持N卡独显，C若使用CPU可以使用DNN模型，但是很难达到实时。
### 相关工作
[tensorrtx](https://github.com/wang-xinyu/tensorrtx)  
[Yolov5](https://github.com/ultralytics/yolov5)  
请参考如上工程的说明配置Cuda、TensorRT环境
### 使用
首先按照 [tensorrtx](https://github.com/wang-xinyu/tensorrtx)获得Tensorrt可使用的模型，(前提是已经安装了 Ubuntu,CUDA,TensorRT，Opencv等必要的库).  
移动 .enigine files 到 Yolov5-TensorRT-AGX_Xavier/engine目录下
```cpp
#define VIDEO_TYPE (0) //0 means laptop camera ;1 means images,2 means Videos,3 means RealsenseD435;
#define NET x  // s m l x
```
可以自行修改main.cpp的参数,NET表示s、m、l、x的模型类别，VIDEO_TYPE 表示输入数据类型，0-笔记本自带0号摄像头，1-图片，2-输入视频文件，3-深度相机（realsenseD435)  

```
cd TensorRT;
mkdir bulid; cd build
cmake ..
make -j12
./Yolov5_trt
```
## 实例视频    
[![Watch the video](https://github.com/Mazhichaoruya/Yolov5-TensorRT-AGX_Xavier/blob/master/input/VOC/2007_000042.jpg)](https://www.bilibili.com/video/BV1PD4y197on)
