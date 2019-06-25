#pragma once

#include "Viewport/Common.h"


template<typename CAM = class Camera>
class Focus
{
    friend class Camera;

    CAM &camera;

    Focus(CAM &camera_);

    template<typename SEL>
    void onSelected(const SEL &select_);

    vc::Position track(float dist_, vc::Position &axis_);
    vc::Rotation pan(float dist_, const vc::Position &target_pos_);
    float dolly(const vc::Position &target_pos_, const vc::Size<> &target_size_);
};

