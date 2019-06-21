#pragma once

#include "Viewport/Common.h"


struct LookAt
{
    vc::Position eye = {28.f,21.f,28.f};
    vc::Position target = vc::Position::zero();
    vc::Direction up = vc::Direction::up();

    float calcDist() const
    {
        return (target-eye).length();
    }

    vc::Direction calcDirection() const
    {
        auto dir = target-eye;
        dir.normalize();
        return dir;
    }
};