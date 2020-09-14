import pyrealsense2 as rs
import numpy as np
import os
import time
# Configure depth and color streams
class Realsense_para:
    def __init__(self,ci,di,ex):
        self.color_inner_matirx=np.mat(np.array([[ci.fx,0,ci.ppx],[0,ci.fy,ci.ppy],[0,0,1]]))
        self.depth_inner_matrix=np.mat(np.array([[di.fx,0,di.ppx],[0,di.fy,di.ppy],[0,0,1]]))
        self.color_to_depth_rotation=np.mat(np.array(ex.rotation).reshape(3,3))##相机转换矩阵 旋转矩阵
        self.color_to_depth_translation=np.mat(np.array(ex.translation))###平移矩阵
    def refresh_mat(self):
        self.frames = pipeline.wait_for_frames()
        self.depth = self.frames.get_depth_frame()
        self.color = self.frames.get_color_frame()
        hole_filling = rs.hole_filling_filter()
        self.depth = hole_filling.process(self.depth)
        # depth_profile = depth.get_profile()
        # color_profile = color.get_profile()
        # print(depth_profile)
        # print(color_profile)
        self.depthmat=np.asanyarray(self.depth.get_data())
        self.colormat=np.asanyarray(self.color.get_data())



pipeline = rs.pipeline()
# 创建 config 对象：
config = rs.config()
# config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
# Start streaming
pipeline.start(config)
sensor = pipeline.get_active_profile().get_device().query_sensors()[1]
sensor.set_option(rs.option.enable_auto_exposure, True)
frames = pipeline.wait_for_frames()
depth = frames.get_depth_frame()
color = frames.get_color_frame()
depth_profile = depth.get_profile()
color_profile = color.get_profile()
print(depth_profile)
print(color_profile)
cvsprofile = rs.video_stream_profile(color_profile)
dvsprofile = rs.video_stream_profile(depth_profile)
color_intrin = cvsprofile.get_intrinsics()
depth_intrin = dvsprofile.get_intrinsics()
extrin = depth_profile.get_extrinsics_to(color_profile)
# print(type(color_intrin))
D435_para=Realsense_para(color_intrin,depth_intrin,extrin)
# D435_para.refresh_mat()
# print(D435_para.color_to_depth_translation)
###获取相机内参数并且保存为矩阵####
# print(color_to_depth_rotation,color_to_depth_translation)
pipeline.stop()##获取参数后停止图像流