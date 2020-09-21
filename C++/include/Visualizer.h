//
// Created by mzc on 2020/9/21.
//
#ifndef DNN_YOLO_VISUALIZER_H
#define DNN_YOLO_VISUALIZER_H
#include <dlib/gui_widgets.h>
#include <dlib/image_transforms.h>
#include "include.h"
//using namespace dlib;
class Visualizer {
private:
    std::vector<dlib::perspective_window::overlay_dot> PointsAll;
//    std::vector<std::vector<std::array<int ,3>>> Vec_Obj;
public:
    Visualizer(std::vector<Objection> Object_vec );
    std::vector<dlib::perspective_window::overlay_dot> Report_PCLOneMat();
};


#endif //DNN_YOLO_VISUALIZER_H
