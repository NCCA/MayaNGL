#pragma once

#include "MayaNGL/Mouse/Mouse.h"
#include "Inverse_Def.hpp"
#include "Focus_Def.hpp"


class Camera
{
    private:
        friend Focus<Camera>;

    public:
        typedef std::array<mc::Direction,2> LocalHVAxis;

    private:
        const Mouse &mouse;

    private:
        mc::LookAt m_lookAt;
        Inverse<Camera> m_inverse;
        mc::CamView m_current_view;

        mc::Translation m_track;
        mc::Translation m_dolly;
        mc::Rotation m_pan;

    public:
        explicit Camera( const Mouse &mouse_,
                         const mc::LookAt &lookAt_ );

        GET_MEMBER(m_lookAt.eye,eye)
        GET_MEMBER(m_lookAt.target,target)
        GET_MEMBER(m_lookAt.up,up)
        GET_MEMBER(m_lookAt,lookAt)
        GET_MEMBER(m_current_view,current_view)
        GET_MEMBER(m_inverse.current,inv_direction)
        GET_MEMBER(m_inverse.shadow,inv_shadow)

        LocalHVAxis calc_local_hv_axis() const;
        mc::Transform compute_transform() const;

        template<typename SEL>
        void focus(const SEL &select_);

        void track();
        void pan();
        void dolly();
        void reset(const mc::LookAt &lookAt_, mc::CamView panel_ = mc::CamView::PERSPECTIVE);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
