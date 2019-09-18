#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


template<typename CAM = class Camera>
class Inverse
{
    friend class Camera;
    template <class Camera> friend class Focus;

    const CAM &cam;

    mc::Direction current;
    mc::Direction original;
    mc::Direction shadow;

    Inverse( const CAM &cam_ );

    mc::Direction calcCurrent();
    mc::Direction calcShadow();
    void reset();

    ~Inverse() noexcept = default;
};
