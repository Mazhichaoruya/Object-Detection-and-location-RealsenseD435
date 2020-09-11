//
// Created by mzc on 2020/8/7.
//

#ifndef DNN_YOLO_INCLUDE_H
#define DNN_YOLO_INCLUDE_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "cv-helpers.hpp"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <Eigen/Dense>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "Position_Transform.h"
#include "Objection.h"
int Realsense_config() ;//
int Get_referance();
extern Eigen::Matrix<float,3,3> MTR;//相机坐标旋转矩阵
extern Eigen::Vector3f V_T;//平移向量T
extern Eigen::Matrix<float,3,3> Inner_Transformation_Depth,InnerTransformation_Color;// 相机内参
extern cv::Mat Depthmate, color_mat;
extern std::vector<std::string> classNamesVec;
#endif //DNN_YOLO_INCLUDE_H
