#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


template<typename CAM = class Camera>
class Focus
{
    friend class Camera;

    CAM &camera;

    Focus(CAM &camera_);

    template<typename SEL>
    void onSelected(const SEL &select_);

    mc::Position track(float dist_, mc::Position &axis_);
    mc::Rotation pan(float dist_, const mc::Position &target_pos_);
    float dolly(const mc::Position &target_pos_, const mc::Size<> &target_size_);
};

