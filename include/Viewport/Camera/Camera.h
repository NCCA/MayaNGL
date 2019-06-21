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
            vc::Position target(target_transform.m_30,target_transform.m_31,target_transform.m_32);



//            m_inverse.reset(); //this is important

//            auto dist = m_lookAt.calcDist()*0.75f;
//            m_lookAt.eye = m_lookAt.target + dist*m_inverse.current;
//            reset(m_lookAt);


            auto translate_axis = target-m_lookAt.target;
            if (translate_axis == vc::Direction::zero())
                return;
            translate_axis.normalize();

            float translate_dist = (target-m_lookAt.target).length();

            m_track.m_30 = -translate_dist*translate_axis.m_x;
            m_track.m_31 = -translate_dist*translate_axis.m_y;
            m_track.m_32 = -translate_dist*translate_axis.m_z;



            auto rotation_dist = (m_lookAt.eye-target).length();
            auto phi = asin((target.m_y-m_lookAt.eye.m_y)/rotation_dist);
            auto theta = asin((target.m_x-m_lookAt.eye.m_x)/(rotation_dist*cos(phi)));

            vc::Rotation Ry = vc::Y_Matrix(theta);
            m_inverse.shadow = m_inverse.shadow*Ry;
            auto rotationAxis = m_lookAt.up.cross(m_inverse.shadow);
            rotationAxis.normalize();
            vc::Rotation Rx = vc::Axis_Matrix(phi*0.5f,rotationAxis);
            vc::Rotation localR = Rx*Ry;
            m_pan = localR.inverse();


            float dolly_dist = rotation_dist * 0.6f;
            m_dolly.m_30 = dolly_dist * m_inverse.original.m_x;
            m_dolly.m_31 = dolly_dist * m_inverse.original.m_y;
            m_dolly.m_32 = dolly_dist * m_inverse.original.m_z;
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
