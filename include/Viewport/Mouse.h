#pragma once

#include <ngl/Vec2.h>
#include "Viewport/SimpleMath.h"


class Mouse
{
    private:
        typedef ngl::Vec2 Position;
        typedef ngl::Vec2 Direction;

    private:
        Position m_start;
        Position m_previous;
        Position m_current;
        Direction m_direction;

    public:
        static constexpr float slowdown = 0.15f;
        float velocity;

    public:
        explicit Mouse() = default;

        GET_MEMBER(m_direction,Direction)

        void setAnchor(float x_, float y_);
        void setTransform(float x_, float y_);
        void reset(float x_=0.f, float y_=0.f);

        ~Mouse() noexcept = default;
};
