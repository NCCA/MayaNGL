#pragma once

#include "Viewport/Mouse.h"


class Camera
{

    private:
        typedef ngl::Vec3 Position;
        typedef ngl::Vec3 Direction;
        typedef ngl::Mat4 Translation;
        typedef ngl::Mat4 Rotation;

    public:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

    private:
        const Mouse &mouse;

    private:
        Position m_position;
        Position m_lookAt;
        Direction m_inverse;
        Direction m_refInverse;
        View m_currentView;

    private:
        Direction updateInverse();

    public:
        explicit Camera(const Mouse &mouse_);

        GET_MEMBER(m_position,Position)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_currentView,CurrentView)

        Rotation Tra;
        Rotation Rot;

        void pan();
        void dolly();
        void track();
        void reset(Position &&pos_={28.f,21.f,28.f},View panel_=View::PERSPECTIVE);
        void focusOn(const Position &target_);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
