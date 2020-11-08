//
// Created by mzc on 2020/8/22.
//

#include "../include/Position_Transform.h"
Eigen::Vector3f Position_Transform::Color_PixtoCamera(Eigen::Vector2f Pix_P){
    Eigen::Vector3f Pix_3D ;
    Pix_3D<<Pix_P(0),Pix_P(1),1.00;
    Color_cam=Color_inner.inverse()*Pix_3D*1.00;//Depthmate.at<uint16_t>(Pix_P(0),Pix_P(1))
//    cout<<"origin"<<Depthmate.at<uint16_t>(Pix_P(0),Pix_P(1))<<endl;
    return  Color_cam;
}

void Position_Transform::Get_camera_referance() {
    CD_Rotation=MTR;
    CD_Transation=V_T;
    Depth_inner=Inner_Transformation_Depth;
    Color_inner=InnerTransformation_Color;
}


Eigen::Vector2f Position_Transform::Get_DepthToColorImg(Eigen::Vector3f Pix_P) {
    Depth_cam = CD_Rotation.inverse() *(Pix_P - CD_Transation);
    Eigen::Vector3f D_m = Depth_inner*Depth_cam;
//    cout<<"C3:"<<Color_3(2)<<endl;
    Depth_Pix_Transform<< (D_m(0)/Color_cam(2)<(WidthCam-1) ? D_m(0)/Color_cam(2):(WidthCam-1) ) ,(D_m(1)/Color_cam(2)<(HeightCam-1)? D_m(1)/Color_cam(2): (HeightCam-1));//限制坐标宽度不要越界
    return Depth_Pix_Transform;
}
std::array<int,2> Position_Transform::Report_Depth_Pix(){
    Color_cam = Color_PixtoCamera(RGB_Pix_position);
    Eigen::Vector2f X_Y=Get_DepthToColorImg(Color_cam);
    Depth_Pix={static_cast<int>(Depth_Pix_Transform(0)),static_cast<int>(Depth_Pix_Transform(1))};
    return Depth_Pix;
}
std::array<int, 3> Position_Transform::Report_PCL() {
    Eigen::Vector3f Image_Pix;
    Image_Pix<<Depth_Pix.at(0),Depth_Pix.at(1),1.00;
    Image_Pix=Depth_inner.inverse()*Image_Pix*Depthmate.at<uint16_t>(Depth_Pix.at(1),Depth_Pix.at(0));
    PCL_Position.at(0)=Image_Pix(0);
    PCL_Position.at(1)=Image_Pix(1);
    PCL_Position.at(2)=Image_Pix(2);
    return PCL_Position;
}

Position_Transform::Position_Transform(std::array<int,2> Pix,bool flag) {
    if (flag)//flag==true means RGB_Pix
    {
        RGB_Pix_position(0)=Pix.at(0);
        RGB_Pix_position(1)=Pix.at(1);
        Depth_Pix=Report_Depth_Pix();
    }
    else{
        Depth_Pix=Pix;
//        Depth_Pix.at(0)=Pix.at(0)<(WidthCam-1) ? Pix.at(0):(WidthCam-1) ;// false means Depth_Pix
//        Depth_Pix.at(1)=Pix.at(1)<(HeightCam-1) ? Pix.at(1):(HeightCam-1) ;// false means Depth_Pix
    }
    Get_camera_referance();//初始化相机参数 初始化深度图Depthmate
}
Position_Transform::~Position_Transform() {
//    std::cout<<"Point destructor "<<std::endl;
}