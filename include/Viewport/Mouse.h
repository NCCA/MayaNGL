#pragma once

#include "Viewport/SimpleMath.h"


class Mouse
{
    private:
        typedef ngl::Vec2 Position;
        typedef ngl::Vec2 Direction;

    private:
        Position m_previous;
        Position m_current;
        Direction m_direction;
        float m_velocity;

    public:
        static constexpr float slowdown = 0.15f;

    public:
        explicit Mouse() = default;

        GET_MEMBER(m_direction,Direction)
        GET_MEMBER(m_velocity,Velocity)

        void setAnchor(float x_, float y_);
        void setTransform(float x_, float y_);
        void reset(float x_=0.f, float y_=0.f);

        ~Mouse() noexcept = default;
};
