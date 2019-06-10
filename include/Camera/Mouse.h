#pragma once

#include <ngl/Vec2.h>
#include "Camera/SimpleMath.h"


class Mouse
{
    private:
        typedef ngl::Vec2 Position;
        typedef ngl::Vec2 Direction;

    private:
        Position m_startingPosition;
        Position m_previousPosition;
        Position m_currentPosition;
        Direction m_direction;

    public:
        static constexpr float slowdown = 0.15f;
        float velocity;

    public:
        explicit Mouse() = default;

        GET_MEMBER(m_direction,Direction)

        void setAnchor(float x_, float y_);
        void setTransform(float x_, float y_);
        void reset();

        ~Mouse() noexcept = default;
};
