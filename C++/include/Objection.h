//
// Created by mzc on 2020/9/10.
//

#ifndef DNN_YOLO_OBJECTION_H
#define DNN_YOLO_OBJECTION_H
#include "include.h"
using namespace std ;
using namespace cv;
class Objection {
private:

    Rect Aera_Objection;
    array<int,2> Point_img;
public:
    array<float,3> Point_Camera;
    bool Enable;
    string Classname;
    Objection(Rect Box,string name);
    void CheckCenterPoint();
    void Transform_ImgtoCam();
    void DealRect();//处理对象区域范围
};


#endif //DNN_YOLO_OBJECTION_H
