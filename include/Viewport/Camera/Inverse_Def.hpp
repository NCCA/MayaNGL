#pragma once

#include "Viewport/Camera/Inverse.h"


template<typename CAM>
Inverse<CAM>::Inverse( const CAM &cam_ )
                       :
                       cam(cam_),
                       current(calcCurrent()),
                       original(current),
                       shadow(calcShadow())
{;}

template<typename CAM>
vc::Direction Inverse<CAM>::calcCurrent()
{
    current = cam.getEye() - cam.getTarget();
    current.normalize();
    return current;
}

template<typename CAM>
vc::Direction Inverse<CAM>::calcShadow()
{
    auto shd = vc::Direction(current.m_x,0.f,current.m_z);
    if (vc::round(shd,6) == vc::Direction::zero())
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
