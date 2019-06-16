#pragma once

#include "Viewport/Inverse.h"


template<typename CAM>
Inverse<CAM>::Inverse( const CAM &cam_ )
                       :
                       cam(cam_),
                       current(update()),
                       original(current),
                       shadow(current.m_x,0.f,current.m_z)
{;}

template<typename CAM>
vc::Direction Inverse<CAM>::update()
{
    current = cam.getPosition() - cam.getLookAt();
    current.normalize();
    return current;
}

template<typename CAM>
void Inverse<CAM>::reset()
{
    current =  update();
    original = current;
    shadow = vc::Direction(current.m_x,0.f,current.m_z);
}
