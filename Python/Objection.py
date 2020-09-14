import cv2
from realsenseconfig import D435_para
from Position_transform import Position_tansform
import numpy as np
class Rcet:
    def __init__(self,left,right,top,bottom):
        if left<0:
            self.left=0
        else:
            self.left=left
        if right>639:
            self.right=639
        else:
            self.right=right
        if top<0:
            self.top=0
        else:
            self.top=top
        if bottom>479:
            self.bottom=479
        else:
            self.bottom=bottom
class Objection:
    def CheckCenterPoint(self):
        self.prePoint=np.array([(self.Area.top+self.Area.bottom)/2,(self.Area.right+self.Area.left)/2])
        # # ret, self.binary = cv2.threshold(self.Object_Area_BW, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
        # self.num_labels, labels, stats, centers = cv2.connectedComponentsWithStats(self.Object_Area_BW, connectivity=8, ltype=cv2.CV_16U)
        # sum_row=0;sum_col=0;sum_P=0
        # if(self.num_labels==2):
        #     for i in range(labels.shape[0]):
        #       for j in range(labels.shape[1]):
        #           if labels[i,j]==1:
        #             sum_col+=j
        #             sum_row+=i
        #             sum_P=sum_P+1
        #     self.Point = [sum_row / sum_P + self.Area.top, sum_col / sum_P + self.Area.left]
        # else:
        #     print("NUM=",self.num_labels)
        self.Point=self.prePoint
    def __init__(self,rcet,classname):
        self.Area=rcet
        self.classname=classname
        self.Enable=False
        self.Object_Area=D435_para.colormat[self.Area.left:self.Area.right,self.Area.top:self.Area.bottom]
        # self.Object_Area_BW=self.Object_Area[:,:,0]*0.114+self.Object_Area[:,:,1]*0.587+self.Object_Area[:,:,2]*0.299
        # self.Object_Area_BW=cv2.cvtColor(self.Object_Area,cv2.COLOR_BGR2GRAY)
        self.CheckCenterPoint()
        self.Transform_ImgtoCam()
    def Transform_ImgtoCam(self):
        New_PT=Position_tansform(self.Point)
        self.PCL=New_PT.Report_PCL()


