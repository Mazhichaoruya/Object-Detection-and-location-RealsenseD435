// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "include.h"
rs2::pipeline pipes;///生成Realsense管道，用来封装实际的相机设备
rs2::stream_profile dprofile;
rs2::stream_profile cprofile;
Eigen::Matrix<float,3,3> MTR;//相机坐标旋转矩阵
Eigen::Vector3f V_T;//平移向量T
Eigen::Matrix<float,3,3> Inner_Transformation_Depth,InnerTransformation_Color;// 相机内参
int Realsense_config(){
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);

    /// Create librealsense context for managing devices
    rs2::context ctx;
    auto devs = ctx.query_devices();                  ///获取设备列表
    int device_num = devs.size();
    std::cout<<"device num: "<<device_num<<std::endl;///设备数量

    ///我只连了一个设备，因此我查看第0个设备的信息
    /// 当无设备连接时此处抛出rs2::error异常
    rs2::device dev = devs[0];
    ///设备编号，每个设备都有一个不同的编号, 如果连接了多个设备，便可根据此编号找到你希望启动的设备
    char serial_number[100] = {0};
    strcpy(serial_number, dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
    printf("serial_number: %s\n",serial_number);

    ///设置从设备管道获取的深度图和彩色图的配置对象
    rs2::config cfg;
    ///配置彩色图像流：分辨率640*480，图像格式：BGR， 帧率：30帧/秒
    ///默认配置任意一个设备，若要配置指定的设备可以根据设备在设备列表里的序列号进行制定:
    ///int indx = 0; ///表示第0个设备
    ///cfg.enable_stream(RS2_STREAM_COLOR,indx, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_COLOR,640, 480, RS2_FORMAT_BGR8, 30);
    ///配置深度图像流：分辨率640*480，图像格式：Z16， 帧率：30帧/秒
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);

    pipes.start(cfg); ///根据给定的配置启动相机管道

    rs2::frameset data;
    data = pipes.wait_for_frames();///等待一帧数据，默认等待5s

    rs2::depth_frame depth       = data.get_depth_frame(); ///获取深度图像数据
    rs2::video_frame color       = data.get_color_frame();  ///获取彩色图像数据
    dprofile =  depth.get_profile();
    cprofile =  color.get_profile();
    return EXIT_SUCCESS;
}
int Get_referance() //try
{
    ///获取彩色相机内参
    rs2::video_stream_profile cvsprofile(cprofile);
    rs2_intrinsics color_intrin =  cvsprofile.get_intrinsics();
    std::cout<<"\ncolor intrinsics: "<<endl;
    InnerTransformation_Color<<color_intrin.fx,0,color_intrin.ppx,0,color_intrin.fy,color_intrin.ppy,0,0,1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout<<InnerTransformation_Color(i,j)<<"\t";
        }
        std::cout<<std::endl;
    }
    std::cout<<"distortion model: "<<color_intrin.model<<std::endl;///畸变模型

    ///获取深度相机内参
    rs2::video_stream_profile dvsprofile(dprofile);
    rs2_intrinsics depth_intrin =  dvsprofile.get_intrinsics();
    std::cout<<"\ndepth intrinsics: "<<endl;
    Inner_Transformation_Depth<<depth_intrin.fx,0,depth_intrin.ppx,0,depth_intrin.fy,depth_intrin.ppy,0,0,1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout<<Inner_Transformation_Depth(i,j)<<"\t";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"distortion model: "<<depth_intrin.model<<std::endl;///畸变模型

    ///获取深度相机相对于彩色相机的外参，即变换矩阵: P_color = R * P_depth + T
    rs2_extrinsics extrin = dprofile.get_extrinsics_to(cprofile);
    std::cout<<"\nextrinsics of depth camera to color camera: \nrotaion: "<<std::endl;
    MTR<<extrin.rotation[0],extrin.rotation[1],extrin.rotation[2],extrin.rotation[3],extrin.rotation[4],extrin.rotation[5],extrin.rotation[6],extrin.rotation[7],extrin.rotation[8];
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            std::cout<<MTR(i,j)<<"\t";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"translation: ";
    V_T<<extrin.translation[0],extrin.translation[1],extrin.translation[2];
    for(int i=0;i<3;i++)
        std::cout<<V_T(i)<<"\t";
    std::cout<<std::endl;
    pipes.stop();

    return EXIT_SUCCESS;
}