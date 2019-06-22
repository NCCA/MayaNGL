#pragma once

#include "Viewport/LookAt/LookAt.h"
#include "Viewport/Camera/Inverse_Def.hpp"
#include "Viewport/Mouse/Mouse.h"
#include <ngl/Quaternion.h>


class Camera
{
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
        void focusOn(const SEL &select_)
        {
            auto &&selectables = select_.getAllSelectables();
            auto &&currently_selected = select_.getCurrentlySelected();

            if (currently_selected.empty())
                return;

            auto target_transform = selectables.at(currently_selected[0]).transform;
            vc::Position target_position(target_transform.m_30,target_transform.m_31,target_transform.m_32);
            vc::Size target_size(target_transform.m_00,target_transform.m_11,target_transform.m_22);

            auto axis = m_lookAt.target-target_position;
            float dist = axis.length();
            if (dist < ngl::EPSILON)
                return;

            auto translate = focus_track(axis,dist);
            auto rotate = focus_pan(target_position);
            auto zoom = focus_dolly(target_position,target_size);

            m_lookAt.eye -= translate + target_position;
            m_lookAt.eye = target_position + (m_lookAt.eye * rotate);
            m_lookAt.target = target_position;
            m_inverse.calcCurrent();
            m_lookAt.eye -= zoom * m_inverse.current;
        }

        vc::Transform computeTransform();
        void pan();
        void dolly();
        void track();
        void reset(const LookAt &lookAt_, View panel_=View::PERSPECTIVE);
        void front();
        void side();
        void top();

        vc::Position focus_track(vc::Position &axis_, float dist_);
        vc::Rotation focus_pan(const vc::Position &target_pos_);
        float focus_dolly(const vc::Position &target_pos_, const vc::Size &target_size_);

        ~Camera() noexcept = default;
};
