#pragma once

#include "Inverse.h"


template<typename CAM>
Inverse<CAM>::Inverse( const CAM &cam_ )
                       :
                       cam(cam_),
                       current(calc_current()),
                       original(current),
                       shadow(calc_shadow())
{;}

template<typename CAM>
mc::Direction Inverse<CAM>::calc_current()
{
    current = cam.get_eye() - cam.get_target();
    current.normalize();
    return current;
}

template<typename CAM>
mc::Direction Inverse<CAM>::calc_shadow()
{
    auto shd = mc::Direction(current.m_x,0.f,current.m_z);
    if (mc::round(shd,6) == mc::Direction::zero())
        shd = cam.get_up().cross(current);
    return shd;
}

template<typename CAM>
void Inverse<CAM>::reset()
{
    current = calc_current();
    original = current;
    shadow = calc_shadow();
}

