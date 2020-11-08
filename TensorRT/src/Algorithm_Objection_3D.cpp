//
// Created by mzc on 2020/9/20.
//
#include "Algorithm_Objection_3D.h"
Algorithm_Objection_3D::Algorithm_Objection_3D(vector<array<int,3>> Data,int height,int width){
    Origin_Data=Data;
//    Mid_Data=Origin_Data;//备份初始数据 用于寻找起始点
    Width=width;
    Height=height;
    Epoch=0;
    Size_Rest_Data=Origin_Data.size();
    for(auto i:Origin_Data)
        Origin_Distance.push_back(ReportDistance(i,{0,0,0}));
//    Copy_Distance=Origin_Distance;
    while (!CheckFinal()){
        auto Start_index=StartPointinit();
        if (Start_index<0){
            break;//遍历完成 强制终止
        }
        FindAllClassPoint(Start_index);
//        for(auto i:Mid_Data)
//            Copy_Distance.push_back(ReportDistance(i,{0,0,0}));
        Epoch++;
    }
    int sum0=0,sum1=0,sum2=0;
    for(auto i:Objection_3D){
            sum0+=i.at(0);
            sum1+=i.at(1);
            sum2+=i.at(2);
    }
    auto num=Objection_3D.size();
//    cout<<num<<endl;//排除BUG num==0，未找到任何一个有效的点
    if(num>0)
        Center_Point={sum0 / static_cast<signed int>(num),sum1 / static_cast<signed int>(num),sum2 / static_cast<signed int>(num)};
    else
        Center_Point={0,0,0};

}
int Algorithm_Objection_3D::StartPointinit() {
    auto Copy=Origin_Distance;
    sort(Copy.begin(),Copy.end());
    int Start_Distance=0;
    for (auto i:Copy){
        if (i>200){
            Start_Distance=i;
            break;
        }
    }
    if (Start_Distance==0)
        return -1;//遍历区域完成 触发终止条件
    auto iter=find(Origin_Distance.begin(),Origin_Distance.end(),Start_Distance);
    int index=distance(Origin_Distance.begin(),iter);
    return index;
}
void Algorithm_Objection_3D::FindAllClassPoint(int index) {
    vector<array<int,3>> This_Point;
    This_Point.push_back(Origin_Data[index]);//先将起始点载入
    Size_Rest_Data--;//剩余大小减一
    Origin_Distance.at(index)=0;//对应距离=0
    int indexS = index;
    while (index>=0) {//上半区域搜索
        int indexL, indexR;
        indexL = index;
        indexR = index;
        int sL = 1;
        while (indexL % Width > 0) {
            if (ReportDistance(Origin_Data[index], Origin_Data[index - sL]) < Distance_Limit) {
                index--;
                This_Point.push_back(Origin_Data[index]);
                Size_Rest_Data--;
                Origin_Distance.at(index)=0;//对应距离=0
                sL = 1;//步长重新初始化为1；
            } else {
                sL++;//寻找下一个点
            }
            indexL--;
        }
        indexL=index;//左边界
        int sR = 1;
        index = indexR;//重新初始化为本行初始点
        while (indexR % Width <Width-1) {
            if (ReportDistance(Origin_Data[index], Origin_Data[index + sR]) < Distance_Limit) {
                index++;
                This_Point.push_back(Origin_Data[index]);
                Size_Rest_Data--;
                Origin_Distance.at(index)=0;//对应距离=0
                sR = 1;//步长重新初始化为1；
            } else {
                sR++;//寻找下一个点
            }
            indexR++;
        }
        indexR=index;
        index=(indexL+indexR)/2;
        if (index<Width)
            break;
        if (ReportDistance(Origin_Data[index], Origin_Data[index-Width]) < Distance_Limit) {
            index=index-Width;
            This_Point.push_back(Origin_Data[index]);//载入下一行起始点
            Size_Rest_Data--;//剩余大小减一
            Origin_Distance.at(index)=0;//对应距离=0
        }
        else break;
    }
    index=indexS;//重新到起始点
    while (index<Origin_Data.size()) {//下半区域搜索
//        This_Point.push_back(Origin_Data[index]);
        int indexL, indexR;
//        indexS = index;
        if (index>=Origin_Data.size()-Width)
            break;
        if (ReportDistance(Origin_Data[index], Origin_Data[index+Width]) < Distance_Limit){
            index=index+Width;
            This_Point.push_back(Origin_Data[index]);//载入下一行起始点
            Size_Rest_Data--;
            Origin_Distance.at(index)=0;//对应距离=0
            indexL = index;
            indexR = index;
        }
        else break;
        int sL = 1;
        while (indexL % Width > 0) {
            if (ReportDistance(Origin_Data[index], Origin_Data[index - sL]) < Distance_Limit) {
                index--;
                This_Point.push_back(Origin_Data[index]);
                Size_Rest_Data--;
                Origin_Distance.at(index)=0;//对应距离=0
                sL = 1;//步长重新初始化为1；
            } else {
                sL++;//寻找下一个点
            }
            indexL--;
        }
        indexL=index;//左边界
        index = indexR;//重新初始化为本行初始点
        int sR = 1;
        while (indexR % Width < Width-1) {
            if (ReportDistance(Origin_Data[index], Origin_Data[index + sR]) < Distance_Limit) {
                index++;
                This_Point.push_back(Origin_Data[index]);
                Size_Rest_Data--;
                Origin_Distance.at(index)=0;//对应距离=0
                sR = 1;//步长重新初始化为1；
            } else {
                sR++;//寻找下一个点
            }
            indexR++;
        }
        indexR=index;
        index=(indexL+indexR)/2;
    }
    ClassAllPoint.push_back(This_Point);
}
bool Algorithm_Objection_3D::CheckFinal() {
//    int size=Mid_Data.size();
    int max_size=0;
    for(auto i:ClassAllPoint){
        if (i.size()>Origin_Data.size()/2) {
            Objection_3D = i;
            return true;//找到目标区域结束
        }
        if (max_size<i.size()) {
                max_size = i.size();
                Objection_3D = i;
        }
    }
    if (Size_Rest_Data<Origin_Data.size()/3) {
        return true;
    }
    return false;
}
int Algorithm_Objection_3D::ReportDistance(array<int, 3> arr,array<int,3> arr1) {
    arr.at(0)-=arr1.at(0);
    arr.at(1)-=arr1.at(1);
    arr.at(2)-=arr1.at(2);
    return arr.at(0)*arr.at(0)+arr.at(1)*arr.at(1)+arr.at(2)*arr.at(2);
//    return sqrt(arr.at(0)*arr.at(0)+arr.at(1)*arr.at(1)+arr.at(2)*arr.at(2));
}