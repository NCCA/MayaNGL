#pragma once

#include "MayaNGL/Common/Common.h"


class Mouse
{
    private:
        typedef ngl::Vec2 Position;
        typedef ngl::Vec2 Direction;

    private:
        float m_velocity;
        Position m_previous;
        Position m_current;
        Direction m_direction;
        Direction m_drag;

    public:
        explicit Mouse() = default;

        GET_MEMBER(m_drag,Drag)

        void setAnchor(float x_, float y_);
        void setTransform(float x_, float y_);
        void reset(float x_=0.f, float y_=0.f);

        ~Mouse() noexcept = default;
};
