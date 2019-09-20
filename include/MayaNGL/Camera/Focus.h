#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


template<typename CAM = class Camera>
class Focus
{
    friend class Camera;

    CAM &camera;

    Focus(CAM &camera_);

    template<typename SEL>
    void on_selected(const SEL &select_);

    mc::Position track(float distance_, mc::Position &axis_);
    mc::Rotation pan(float distance_, const mc::Position &target_pos_);
    float dolly(const mc::Position &target_pos_, const mc::Size<> &target_size_);
};

