#pragma once

#include "Viewport/Inverse_Def.hpp"
#include "Viewport/Mouse.h"


class Camera
{
    public:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

    private:
        const Mouse &mouse;

    private:
        vc::Position m_position;
        vc::Position m_lookAt;
        Inverse<Camera> m_inverse;
        View m_currentView;
        vc::Transform m_transform;

    public:
        explicit Camera( const Mouse &mouse_ );

        GET_MEMBER(m_position,Position)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_currentView,CurrentView)
        GET_MEMBER(m_transform,Transform)

        void pan();
        void dolly();
        void track();
        void reset(vc::Position &&pos_={28.f,21.f,28.f},View panel_=View::PERSPECTIVE);
        void focusOn(const vc::Position &target_);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};

