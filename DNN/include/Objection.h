//
// Created by mzc on 2020/9/10.
//

#ifndef DNN_YOLO_OBJECTION_H
#define DNN_YOLO_OBJECTION_H
#include "include.h"
#include "./dlib/clustering.h"
#include <dlib/gui_widgets.h>
#include <dlib/image_transforms.h>
using namespace std ;
using namespace cv;
//using namespace dlib;
//typedef dlib::matrix<int ,3,1> sample_type;
//typedef dlib::radial_basis_kernel<sample_type> kernel_type;
class Objection {
private:
    Rect Aera_Objection_R,Area_Objection_D;
    array<int,2> Point_img;
    vector <array<int ,3>> Objection_PCL;
    vector <array<int,2>> Objection_DepthPoint;
    array<int ,3> Point_Camera;
//    vector<sample_type> samples;//For K-means
//    vector<sample_type> initial_centers;
public:
    vector <array<int ,3>> Real_Point;
    bool Enable;
//    Mat labels,Stats,centroids;
    string Classname;
    int ClassID;
    Objection(Rect Box,int ID);
    void CheckStartPoint();
    void Transform_ImgtoCam();
    void DealRect();//处理对象区域范围
    Rect Area_limit(Rect Box);
    float Get_Area_Depth(Rect Box);

};


#endif //DNN_YOLO_OBJECTION_H
