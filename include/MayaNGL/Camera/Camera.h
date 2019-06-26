#pragma once

#include "MayaNGL/Mouse/Mouse.h"
#include "Inverse_Def.hpp"
#include "Focus_Def.hpp"


class Camera
{
    private:
        friend Focus<Camera>;

    private:
        const Mouse &mouse;

    private:
        mc::LookAt m_lookAt;
        Inverse<Camera> m_inverse;
        mc::CamView m_currentView;

        mc::Translation m_track;
        mc::Translation m_dolly;
        mc::Rotation m_pan;

    public:
        explicit Camera( const Mouse &mouse_,
                         const mc::LookAt &lookAt_ );

        GET_MEMBER(m_lookAt.eye,Eye)
        GET_MEMBER(m_lookAt.target,Target)
        GET_MEMBER(m_lookAt.up,Up)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_currentView,CurrentView)

        template<typename SEL>
        void focus(const SEL &select_);

        mc::Transform computeTransform() const;
        void track();
        void pan();
        void dolly();
        void reset(const mc::LookAt &lookAt_, mc::CamView panel_=mc::CamView::PERSPECTIVE);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
