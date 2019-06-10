#pragma once

#include "Camera/Mouse.h"
#include <ngl/Mat3.h>
#include <ngl/Mat4.h>
#include <QKeyEvent>
#include <QMouseEvent>


class Camera
{
    private:
        typedef ngl::Vec3 Position;
        typedef ngl::Vec3 Direction;

    private:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

        Position m_position;
        Position m_lookAt;
        Direction m_upVector;
        Direction m_inverseDirection;
        View m_currentView;
        Mouse m_mouse;

    private:
        void pan();
        void dolly();
        void track();
        void refocus();
        void focusOn(const Position &target_);
        void front();
        void side();
        void top();

    public:
        explicit Camera();

        GET_MEMBER(m_position,Position)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_upVector,UpVector)

        void keyPress(QKeyEvent *event_);
        void mousePress(QMouseEvent *event_);
        void mouseMove(QMouseEvent *event_);

        ~Camera() noexcept = default;
};
