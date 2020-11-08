//
// Created by mzc on 2020/8/22.
//
#include "include.h"
#ifndef DNN_YOLO_POSITION_TRANSFORM_H
#define DNN_YOLO_POSITION_TRANSFORM_H

class Position_Transform{
private:
    Eigen::Matrix<float,3,3> CD_Rotation;//相机坐标系旋转矩阵
    Eigen::Vector3f CD_Transation;//平移向量
    Eigen::Matrix<float,3,3> Depth_inner,Color_inner;//相机内参数
    Eigen::Vector3f Depth_cam,Color_cam;//
    Eigen::Vector2f RGB_Pix_position;//RGB像素坐标
    std::array<int, 3> PCL_Position ;//RGB相机坐标系坐标
    Eigen::Vector2f Depth_Pix_Transform;
    std::array<int,2> Depth_Pix;
public:
    Position_Transform(std::array<int,2> Pix,bool flag);//构造函数
    void Get_camera_referance();//获取相机内参数和转换矩阵
    Eigen::Vector3f Color_PixtoCamera(Eigen::Vector2f Pix_P); //深度相机像素坐标转深度相机坐标系
    Eigen::Vector2f Get_DepthToColorImg(Eigen::Vector3f Pix_P);//深度图与RGB图完成像素匹配
    std::array<int,3> Report_PCL();//返回对应点的相机极坐标系3D位置
    std::array<int,2> Report_Depth_Pix();
    ~Position_Transform();
};
#endif //DNN_YOLO_POSITION_TRANSFORM_H
