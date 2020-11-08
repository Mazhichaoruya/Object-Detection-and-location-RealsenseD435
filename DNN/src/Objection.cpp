//
// Created by mzc on 2020/9/10.
//
#include "../include/Objection.h"
#include "../include/include.h"
#include "include/Algorithm_Objection_3D.h"
#include "time.h"
using namespace std;
using namespace cv;
//using namespace dlib;
Objection::Objection(Rect Box, int ID){
    Aera_Objection_R=Area_limit(Box);//初始化RGB图像目标框
    ClassID=ID;
    Classname=classNamesVec[ID];//目标类别名称
    Position_Transform Start_point(array<int,2>{Aera_Objection_R.x,Aera_Objection_R.y}, true);//转换彩色图的坐标到深度图
    array<int,2> Start_pix=Start_point.Report_Depth_Pix();//转换
    Box.x=Start_pix.at(0);Box.y=Start_pix.at(1);//更新矩形框
    Area_Objection_D=Area_limit(Box);//越界限制下初始化深度图下的矩形区域
    DealRect();//处理矩形框 稀疏化 获取中心点 聚类
    CheckStartPoint();
    Transform_ImgtoCam();//将中心点坐标转换
    ostringstream ss;
    if (Enable==true)
        ss << "("<<static_cast<int>(Point_Camera.at(0))<<","<<static_cast<int>(Point_Camera.at(1))<<","<<static_cast<int>(Point_Camera.at(2)) <<")";
    else
        ss<<"Null";
    putText(color_mat, ss.str(), Point(Point_img.at(0), Point_img.at(1)), 0, 0.3, Scalar(0, 255, 0));
}
Rect  Objection::Area_limit(Rect Box) {
    Rect Obj;
    Obj.x=(Box.x<0 ? 0:Box.x);//起始点越界检测
    Obj.y=(Box.y<0 ? 0:Box.y);
    Obj.height=(Box.y<0 ? Box.height+Box.y:Box.height); //起始点越界修正目标的宽度和高度
    Obj.width=(Box.x<0 ? Box.width+Box.x:Box.width);
    Obj.height=(Obj.height+Obj.y>(HeightCam-1) ? (HeightCam-1)-Obj.y:Obj.height);//目标框大小越界检测
    Obj.width=(Obj.width+Obj.x>(WidthCam-1) ? (WidthCam-1)-Obj.x:Obj.width);
    return Obj;
}
void Objection::CheckStartPoint() {
    array<float,2> Pre_point;
   Pre_point.at(0)=Aera_Objection_R.x+Aera_Objection_R.width/2 ;
   Pre_point.at(1)=Aera_Objection_R.y+Aera_Objection_R.height/2;
//   cout<<Classname<<": Point_img:"<<Point_img.at(0)<<"  "<<Point_img.at(1)<<endl;
//   if (Enable== false) {
   Point_img.at(0)=Pre_point.at(0);
   Point_img.at(1)=Pre_point.at(1);
//   }
   ////
}
void Objection::Transform_ImgtoCam() {
//    Position_Transform Objection_center(Point_img,true);
//    Point_Camera=Objection_center.Report_PCL();
//    if(Point_Camera.at(2)<=200||Point_Camera.at(2)>=6000)
    if (Real_Point.size()>0)
       Enable= true;
    else Enable= false;
}
float Objection::Get_Area_Depth(Rect Box) {
    std::array<int,Stride*Stride> Arr_Box;
    int result;
    for (int i = Box.y; i < Box.y+Box.height; ++i)
        for (int j = Box.x; j < Box.x+Box.width; ++j)
        {
            if (Depthmate.at<uint16_t>(i, j) > 6000 || Depthmate.at<uint16_t>(i, j) < 200)//D435有效探测距离有限 0.2M-6M
                Arr_Box.at((i - Box.y)*Stride + (j - Box.x))= 0;
            else
                Arr_Box.at((i - Box.y)*Stride + (j - Box.x)) = Depthmate.at<uint16_t>(i, j);
        }
    sort(Arr_Box.begin(),Arr_Box.end());
    for (auto i:Arr_Box){    //最小池化
        if (i>200){
            result=i;
            break;
        }
    }
    ///////////////////////////////平均池化
//    std::array<int,Stride*2> Value;
//    int sub=0;
//    for (int i = 0; i < Stride*2; ++i) {
//        Value.at(i)=Arr_Box.at(Stride*Stride/2-Stride+1+i);
//        if (Value.at(i)==0)
//            sub++;//剔除无效的点
//    }
//    result=std::accumulate(Value.begin(),Value.end(),0.0);
//    if (result<200)
//        result=0;
//    else
//        result=result/(Value.size()-sub);
    /////////////////////////////////////
    return result  ;
}
void Objection::DealRect() {
    ///3D点聚类算法
    ///目标区域稀疏化 获取稀疏化之后的点云信息
    int height=0,width=0;
    Mat Object_Area_Depth = Depthmate(Area_Objection_D);//截取目标范围的深度图
//    Mat Object_Sparse_Depth=Mat::zeros(Size(Object_Area_Depth.cols/Stride,Object_Area_Depth.rows/Stride),CV_16U);
    for (int i = 0; i < Object_Area_Depth.rows-Stride; i += Stride) {
        height++;width=0;
        for (int j = 0; j < Object_Area_Depth.cols-Stride; j += Stride) {
            array<int, 2> Sparse_Point{(Area_Objection_D.x + j + Stride / 2)>(WidthCam-1) ? (WidthCam-1):(Area_Objection_D.x + j + Stride / 2), (Area_Objection_D.y + i + Stride / 2)>(HeightCam-1) ? (HeightCam-1):Area_Objection_D.y + i + Stride / 2};
            Rect Area_ele(Area_Objection_D.x + j, Area_Objection_D.y + i, Stride, Stride);
//            Object_Sparse_Depth.at<uint16_t>(i/Stride,j/Stride)=Get_Area_Depth(Area_ele);
            auto Depth_value = Get_Area_Depth(Area_ele);//稀疏化
//            if (Depth_value==0)
//                cout<<Depth_value<<endl;
//            cout<<"Position"<<Sparse_Point.at(1)<<" "<<Sparse_Point.at(0)<<endl;//测试
            Depthmate.at<uint16_t>(Sparse_Point.at(1), Sparse_Point.at(0)) = Depth_value;
//            if (Depth_value > 0) {
            Objection_DepthPoint.push_back(Sparse_Point);
            auto IP=Position_Transform(Sparse_Point, false).Report_PCL();
            Objection_PCL.push_back(IP);
            width++;
//                sample_type m;
//                m(0)=IP.at(0);
//                m(1)=IP.at(1);
//                m(2)=IP.at(2);
//                samples.push_back(m);
//            }
        }
    }
//    int Long=Objection_PCL.size();
//    cout<<"Test:"<<Long<<":"<<height*width<<endl;
clock_t StartTime,EndTime;
    StartTime=clock();
    Algorithm_Objection_3D This_Algorithm(Objection_PCL,height,width);
    EndTime=clock();
    auto Time=(double)(EndTime - StartTime) / CLOCKS_PER_SEC;
    Point_Camera=This_Algorithm.Center_Point;
    Real_Point=This_Algorithm.Objection_3D;
    //////Dlib 库KK-mean 聚类 失效
//    dlib::kcentroid<kernel_type> kc(kernel_type(0.1),0.5,samples.size()/2);
//    dlib::kkmeans<kernel_type> test(kc);
//    test.set_number_of_centers(2);
//    dlib::pick_initial_centers(2, initial_centers, samples, test.get_kernel());
//    test.train(samples,initial_centers);
//    int sum0=0,sum1=0,sum2=0,num=0;
//    for(auto i : samples){
//        array<int ,3> new_point;
//        if(test(i)==0) {
//            new_point.at(0)=i(0);
//            new_point.at(1)=i(1);
//            new_point.at(2)=i(2);
//            Real_Point.push_back(new_point);
//            cout<<samples.size()<<":i"<<num<<":"<<i(0)<<" "<<i(1)<<" "<<i(2)<<endl;
//            sum0+=new_point.at(0);
//            sum1+=new_point.at(1);
//            sum2+=new_point.at(2);
//            num++;
//        }
//    }
//    cout<<samples.size()<<":i"<<num<<endl;
///////////Test//////////////////////////////////
//    cout<<Classname<<":"<<Objection_PCL.size()<<endl;
//    for (int i = 0; i < Objection_PCL.size(); ++i) {
//        cout<<"opt"<<Objection_PCL[i].at(0)<<" "<<Objection_PCL[i].at(1)<<" "<<Objection_PCL[i].at(2)<<";";
//        cout<<Objection_DepthPoint[i].at(0)<<" "<<Objection_DepthPoint[i].at(1)<<";"<<endl;
//    }
///////////////////
//    cout<<samples[1]<<"index1"<<endl;
//    auto num0=test.get_kcentroid(0).dictionary_size();
//    auto num1=test.get_kcentroid(1).dictionary_size();
//    auto num2=test.get_kcentroid(2).dictionary_size();
//    Point_Camera.at(0)=sum0/num;
//    Point_Camera.at(1)=sum1/num;
//    Point_Camera.at(2)=sum2/num;
    ///连通区域算法 效果不好 舍弃
//    auto P0=test.get_kcentroid(0).get_kernel();
//    auto P1=test.get_kcentroid(1).get_kernel();
//    Mat Obj_ZO,OSD;
//    Object_Sparse_Depth.convertTo(OSD,CV_8U);
//    threshold(OSD, Obj_ZO,0, 255, THRESH_OTSU);
//    auto Num_Obj = connectedComponentsWithStats(Obj_ZO,labels,Stats,centroids,8,CV_16U);
//    cout<<"Object_Area_num=="<<Num_Obj<<endl;
//    Mat Object_Area=color_mat(Aera_Objection);//截取目标范围的彩色
//    cv::cvtColor(Object_Area,Object_Area,COLOR_BGR2GRAY);
//    Object_Area.convertTo(Object_Area,CV_8U);
//    auto Num_Obj = connectedComponentsWithStats(Object_Area,labels,Stats,centroids,8,CV_16U);
//    if (Num_Obj==2){//正常状态 仅仅检测到一个连通区域 为检测目标
//        int sum_row=0,sum_col=0,sum_number=0;
//        for (int i = 0; i < labels.rows; ++i) {
//            for (int j = 0; j < labels.cols; ++j) {
//                if (labels.at<uchar>(i,j)==1)
//                {
//                    sum_row+=i;
//                    sum_col+=j;
//                    sum_number++;
//                }
//            }
//        }
//        Point_img.at(1)=sum_col/sum_number+Aera_Objection.x;
//        Point_img.at(0)=sum_row/sum_number+Aera_Objection.y;
//        Enable= true;
//        cout<<Stats.at<int>(1,CC_STAT_AREA)<<" "<<labels.rows*labels.cols<<endl;
//    } else{
//       cout<<"Error Object_num::"<<Num_Obj<<endl;
//       Enable= false;
//    }

}