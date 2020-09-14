//
// Created by mzc on 2020/9/10.
//
#include "../include/Objection.h"
#include "../include/include.h"

using namespace std;
using namespace cv;
Objection::Objection(Rect Box, string name){
    Aera_Objection.x=(Box.x<0 ? 0:Box.x);//起始点越界检测
    Aera_Objection.y=(Box.y<0 ? 0:Box.y);
    Aera_Objection.height=(Box.y<0 ? Box.height+Box.y:Box.height); //起始点越界修正目标的宽度和高度
    Aera_Objection.width=(Box.x<0 ? Box.width+Box.y:Box.width);
    Aera_Objection.height=(Aera_Objection.height+Aera_Objection.y>479 ? 479-Aera_Objection.y:Aera_Objection.height);//目标框大小越界检测
    Aera_Objection.width=(Aera_Objection.width+Aera_Objection.x>639 ? 639-Aera_Objection.x:Aera_Objection.width);
    Classname=name;
    CheckCenterPoint();
    Transform_ImgtoCam();
    ostringstream ss;
    if (Enable= true)
        ss << "("<<static_cast<int>(Point_Camera.at(0))<<","<<static_cast<int>(Point_Camera.at(1))<<","<<static_cast<int>(Point_Camera.at(2)) <<")";
    else
        ss<<"Null";
    putText(color_mat, ss.str(), Point(Point_img.at(1), Point_img.at(0)), 0, 0.3, Scalar(0, 255, 0));
}
void Objection::CheckCenterPoint() {
    array<float,2> Pre_point;
   Pre_point.at(0)=Aera_Objection.x+Aera_Objection.width/2 ;
   Pre_point.at(1)=Aera_Objection.y+Aera_Objection.height/2;
   DealRect();
//   cout<<Classname<<": Point_img:"<<Point_img.at(0)<<"  "<<Point_img.at(1)<<endl;
   if (Enable== false) {
   Point_img.at(0)=Pre_point.at(0);
   Point_img.at(1)=Pre_point.at(1);
   }
   ////
}
void Objection::Transform_ImgtoCam() {
    Position_Transform Objection_center(Point_img);
    Point_Camera=Objection_center.Report_PCL();
    if(Point_Camera.at(2)<=200||Point_Camera.at(2)>=6000)
       Enable= false;
    else Enable=true;
}
void Objection::DealRect() {
    Mat Object_Area=color_mat(Aera_Objection);//截取目标范围的彩色
    cv::cvtColor(Object_Area,Object_Area,COLOR_BGR2GRAY);
    Object_Area.convertTo(Object_Area,CV_8U);
    auto Num_Obj = connectedComponentsWithStats(Object_Area,labels,Stats,centroids,8,CV_16U);
    if (Num_Obj==2){//正常状态 仅仅检测到一个连通区域 为检测目标
        int sum_row=0,sum_col=0,sum_number=0;
        for (int i = 0; i < labels.rows; ++i) {
            for (int j = 0; j < labels.cols; ++j) {
                if (labels.at<uchar>(i,j)==1)
                {
                    sum_row+=i;
                    sum_col+=j;
                    sum_number++;
                }
            }
        }
        Point_img.at(1)=sum_col/sum_number+Aera_Objection.x;
        Point_img.at(0)=sum_row/sum_number+Aera_Objection.y;
        Enable= true;
//        cout<<Stats.at<int>(1,CC_STAT_AREA)<<" "<<labels.rows*labels.cols<<endl;
    } else{
       cout<<"Error Object_num::"<<Num_Obj<<endl;
       Enable= false;
    }
}