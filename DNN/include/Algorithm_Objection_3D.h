//
// Created by mzc on 2020/9/20.
//

#ifndef DNN_YOLO_ALGORITHM_OBJECTION_3D_H
#define DNN_YOLO_ALGORITHM_OBJECTION_3D_H
#include "include/include.h"

class Algorithm_Objection_3D {
private:
    vector<array<int,3>> Origin_Data;//初始数据、目标点3D信息,中间处理保存数据
    vector<vector<array<int,3>>> ClassAllPoint;//每次迭代的类别点
    vector<int> Origin_Distance;
    int Width,Height;//目标框宽和高
    int Epoch;//迭代轮次
    int Size_Rest_Data;
public:
    Algorithm_Objection_3D(vector<array<int,3>> Data,int height,int width);
    int StartPointinit();
    void FindAllClassPoint(int index);
    bool CheckFinal();
    int ReportDistance(array<int,3> arr,array<int,3> arr1);
    array<int,3> Center_Point{0,0,0};//目标的中心店
    vector<array<int,3>> Objection_3D;
};


#endif //DNN_YOLO_ALGORITHM_OBJECTION_3D_H
