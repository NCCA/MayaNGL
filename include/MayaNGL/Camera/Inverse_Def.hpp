#pragma once

#include "Inverse.h"


template<typename CAM>
Inverse<CAM>::Inverse( const CAM &cam_ )
                       :
                       cam(cam_),
                       current(calcCurrent()),
                       original(current),
                       shadow(calcShadow())
{;}

template<typename CAM>
mc::Direction Inverse<CAM>::calcCurrent()
{
    current = cam.getEye() - cam.getTarget();
    current.normalize();
    return current;
}

template<typename CAM>
mc::Direction Inverse<CAM>::calcShadow()
{
    auto shd = mc::Direction(current.m_x,0.f,current.m_z);
    if (mc::round(shd,6) == mc::Direction::zero())
        shd = cam.getUp().cross(current);
    return shd;
}

template<typename CAM>
void Inverse<CAM>::reset()
{
    current = calcCurrent();
    original = current;
    shadow = calcShadow();
}

