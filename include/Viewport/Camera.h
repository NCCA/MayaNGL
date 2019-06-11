#pragma once

#include "Viewport/Mouse.h"
#include <ngl/Mat3.h>


class Camera
{
    private:
        typedef ngl::Vec3 Position;
        typedef ngl::Vec3 Direction;

    public:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

    private:
        const Mouse &mouse;

    private:
        Position m_position;
        Position m_lookAt;
        Direction m_upVector;
        Direction m_inverseDirection;
        View m_currentView;


    public:
        explicit Camera(const Mouse &mouse_);

        GET_MEMBER(m_position,Position)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_upVector,UpVector)
        GET_MEMBER(m_currentView,CurrentView)

        void pan();
        void dolly();
        void track();
        void reset(Position &&pos_={28.f,21.f,28.f},View view_=View::PERSPECTIVE,Direction &&up_=ngl::Vec3::up());
        void focusOn(const Position &target_);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
