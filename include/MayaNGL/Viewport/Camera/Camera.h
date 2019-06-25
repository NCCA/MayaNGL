#pragma once

#include "MayaNGL/Viewport/LookAt/LookAt.h"
#include "MayaNGL/Viewport/Camera/Inverse_Def.hpp"
#include "MayaNGL/Mouse/Mouse.h"
#include "MayaNGL/Viewport/Camera/Focus_Def.hpp"


class Camera
{
    private:
        friend Focus<Camera>;

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
        void focus(const SEL &select_);

        vc::Transform computeTransform();
        void track();
        void pan();
        void dolly();
        void reset(const LookAt &lookAt_, View panel_=View::PERSPECTIVE);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
