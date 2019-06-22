#pragma once

#include "Viewport/LookAt/LookAt.h"
#include "Viewport/Camera/Inverse_Def.hpp"
#include "Viewport/Mouse/Mouse.h"
#include "Viewport/Camera/Focus.h"


class Camera : Focus<Camera>
{
    private:
        friend class Focus<Camera>;

    public:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

    private:
        const Mouse &mouse;

    private:
        LookAt m_lookAt;
        Inverse<Camera> m_inverse;
        View m_currentView;

        vc::Translation m_track;
        vc::Translation m_dolly;
        vc::Rotation m_pan;

    public:
        explicit Camera( const Mouse &mouse_,
                         const LookAt &lookAt_ );

        GET_MEMBER(m_lookAt.eye,Eye)
        GET_MEMBER(m_lookAt.target,Target)
        GET_MEMBER(m_lookAt.up,Up)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_currentView,CurrentView)

        template<typename SEL>
        void focus(const SEL &select_)
        {
            Focus<Camera>::focus(select_);
        }

        vc::Transform computeTransform();
        void pan();
        void dolly();
        void track();
        void reset(const LookAt &lookAt_, View panel_=View::PERSPECTIVE);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
