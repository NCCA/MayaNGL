#pragma once

#include "MayaNGL/Common.h"


template<typename CAM = class Camera>
class Inverse
{
    friend class Camera;
    template <class Camera> friend class Focus;

    const CAM &cam;

    vc::Direction current;
    vc::Direction original;
    vc::Direction shadow;

    Inverse( const CAM &cam_ );

    vc::Direction calcCurrent();
    vc::Direction calcShadow();
    void reset();

    ~Inverse() noexcept = default;
};
