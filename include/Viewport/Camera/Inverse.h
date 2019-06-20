#pragma once

#include "Viewport/Common.h"


template<typename CAM>
class Inverse
{
    friend CAM;

    const CAM &cam;

    vc::Direction current;
    vc::Direction original;
    vc::Direction shadow;

    explicit Inverse( const CAM &cam_ );

    vc::Direction calcCurrent();
    vc::Direction calcShadow();
    void reset();

    ~Inverse() noexcept = default;
};
