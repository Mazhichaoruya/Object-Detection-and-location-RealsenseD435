//
// Created by mzc on 2020/9/21.
//

#include "../include/Visualizer.h"
Visualizer::Visualizer(std::vector<Objection> Object_vec) {
    for (auto Obj:Object_vec) {
        dlib::rgb_pixel color=dlib::colormap_jet(Obj.ClassID,0,80);
        for (auto obj:Obj.Real_Point) {
            dlib::vector<int> Point(obj[0], obj[1], obj[2]);
            PointsAll.push_back(dlib::perspective_window::overlay_dot(Point, color));
        }
    }
}
std::vector<dlib::perspective_window::overlay_dot> Visualizer::Report_PCLOneMat() {
    return PointsAll;
}