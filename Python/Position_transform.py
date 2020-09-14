import numpy as np
from realsenseconfig import D435_para
class Position_tansform:
    def __init__(self,RGB_pix_Position):
        # self.Depth_cam,self.Color_cam=np.mat(np.array([0.0,0.0,0.0]))
        self.RGB_Pix_POS=RGB_pix_Position;
    def Report_PCL(self):
        pix_3d=np.mat(np.append(self.RGB_Pix_POS,[1.00]))
        self.Depth_cam=D435_para.depth_inner_matrix.I*pix_3d.T #D435_para.depthmat[int(self.RGB_Pix_POS[0]),int(self.RGB_Pix_POS[1])]
        self.Color_cam=D435_para.color_to_depth_rotation*self.Depth_cam+D435_para.color_to_depth_translation.T
        Color3=D435_para.color_inner_matirx*self.Color_cam
        # print(Color3[2,0],self.Color_cam[2,0])
        if Color3[0,0]/Color3[2,0]<479:
            P1=Color3[0,0]/Color3[2,0]
        else:
            P1=479
        if Color3[1,0]/Color3[2,0]<639:
            P2=Color3[1,0]/Color3[2,0]
        else:
            P2=639
        self.Depth_pix=np.array([P1,P2])
        Image_pix=np.mat(np.append(self.Depth_pix,[1.00]))
        PCL=D435_para.color_inner_matirx.I*Image_pix.T*D435_para.depthmat[int(P1),int(P2)]
        self.PCL=[int(PCL[0,0]),int(PCL[1,0]),int(PCL[2,0])]
        # print(self.PCL)
        return self.PCL


