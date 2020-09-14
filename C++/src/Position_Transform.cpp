//
// Created by mzc on 2020/8/22.
//

#include "../include/Position_Transform.h"
Eigen::Vector3f Position_Transform::Depth_PixtoCamera(Eigen::Vector2f Pix_P) {
    Eigen::Vector3f Pix_3D ;
    Pix_3D<<Pix_P(0),Pix_P(1),1.00;
    Depth_cam=Depth_inner.inverse()*Pix_3D*1.00;//Depthmate.at<uint16_t>(Pix_P(0),Pix_P(1))
//    cout<<"origin"<<Depthmate.at<uint16_t>(Pix_P(0),Pix_P(1))<<endl;
    return  Depth_cam;
}

void Position_Transform::get_camera_referance() {
    CD_Rotation=MTR;
    CD_Transation=V_T;
    Depth_inner=Inner_Transformation_Depth;
    Color_inner=InnerTransformation_Color;
}


Eigen::Vector2f Position_Transform::Get_DepthToColorImg(Eigen::Vector3f Pix_P) {
    Color_cam = CD_Rotation * Pix_P + CD_Transation;
    Eigen::Vector3f Color_3 = Color_inner*Color_cam;
//    cout<<"C3:"<<Color_3(2)<<endl;
    Depth_Pix_Transform<< (Color_3(0)/Color_cam(2)<479 ? Color_3(0)/Color_cam(2):479 ) ,(Color_3(1)/Color_cam(2)<639? Color_3(1)/Color_cam(2): 639);//限制坐标宽度不要越界
    return Depth_Pix_Transform;
}
std::array<float, 3> Position_Transform::Report_PCL() {
    Depth_cam = Depth_PixtoCamera(RGB_Pix_position);
    Eigen::Vector2f X_Y=Get_DepthToColorImg(Depth_cam);
    std::array<float,3> PCL;
    Eigen::Vector3f Image_Pix;
    Image_Pix<<X_Y(0),X_Y(1),1.00;
    Image_Pix=Color_inner.inverse()*Image_Pix*Depthmate.at<uint16_t>(static_cast<int>(Depth_Pix_Transform(0)),static_cast<int>(Depth_Pix_Transform(1)));
    PCL.at(0)=Image_Pix(0);
    PCL.at(1)=Image_Pix(1);
    PCL.at(2)=Image_Pix(2);
    PCL_Position<<PCL.at(0),PCL.at(1),PCL.at(2);
    return PCL ;
}

Position_Transform::Position_Transform(std::array<int,2> RGB_pix) {
    RGB_Pix_position(0)=RGB_pix.at(0);
    RGB_Pix_position(1)=RGB_pix.at(1);
    get_camera_referance();//初始化相机参数 初始化深度图Depthmate
}
Position_Transform::~Position_Transform() {
//    std::cout<<"Point destructor "<<std::endl;
}