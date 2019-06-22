#pragma once

#include "Viewport/LookAt/LookAt.h"
#include "Viewport/Camera/Inverse_Def.hpp"


template <typename C = class Camera>
class Focus
{
    friend class Camera;

    C *crtp = static_cast<C*>(this);
//    C &camera;

//    Focus(C &camera_) : camera(camera_)
//    {;}

    template<typename SEL>
    void focus(const SEL &select_)
    {
        auto &&selectables = select_.getAllSelectables();
        auto &&currently_selected = select_.getCurrentlySelected();

        if (currently_selected.empty())
            return;

        auto && lookAt = crtp->m_lookAt;
        auto && inverse = crtp->m_inverse;

        auto target_transform = selectables.at(currently_selected[0]).transform;
        vc::Position target_position(target_transform.m_30,target_transform.m_31,target_transform.m_32);
        vc::Size target_size(target_transform.m_00,target_transform.m_11,target_transform.m_22);

        auto axis = lookAt.target-target_position;
        float dist = axis.length();

        auto translate = this->track(dist,axis);
        auto rotate = this->pan(dist,target_position);
        auto zoom = this->dolly(target_position,target_size);

        lookAt.eye -= translate + target_position;
        lookAt.eye = target_position + (lookAt.eye * rotate);
        lookAt.target = target_position;
        inverse.calcCurrent();
        lookAt.eye -= zoom * inverse.current;
    }


    vc::Position track(float dist_, vc::Position &axis_)
    {
        if (dist_ < ngl::EPSILON)
            return vc::Position::zero();

        auto && track = crtp->m_track;

        axis_.normalize();
        auto transform = dist_*axis_;

        track.m_30 += transform.m_x;
        track.m_31 += transform.m_y;
        track.m_32 += transform.m_z;

        return transform;
    }

    vc::Rotation pan(float dist_, const vc::Position &target_pos_)
    {
        if (dist_ < ngl::EPSILON)
            return vc::Rotation();

        auto && lookAt = crtp->m_lookAt;
        auto && inverse = crtp->m_inverse;
        auto && pan = crtp->m_pan;

        inverse.calcCurrent();
        inverse.calcShadow();

        float rotation_dist = (lookAt.eye-target_pos_).length();
        auto phi = asin((target_pos_.m_y-lookAt.eye.m_y)/rotation_dist);
        auto theta = asin((target_pos_.m_x-lookAt.eye.m_x)/(rotation_dist*cos(phi)));

        vc::Rotation Ry = vc::Y_Matrix(theta);
        inverse.shadow = inverse.shadow*Ry;
        auto rotationAxis = lookAt.up.cross(inverse.shadow);
        rotationAxis.normalize();
        vc::Rotation Rx = vc::Axis_Matrix(phi*0.5f,rotationAxis);
        vc::Rotation localR = Rx*Ry;

        pan *= localR.inverse();

        return localR;
    }

    float dolly(const vc::Position &target_pos_, const vc::Size &target_size_)
    {
        auto && lookAt = crtp->m_lookAt;
        auto && inverse = crtp->m_inverse;
        auto && dolly = crtp->m_dolly;

        auto t_dir = (lookAt.eye-target_pos_);
        t_dir.normalize();

        auto edge = target_pos_ + 6.f*t_dir*target_size_;
        auto e_dir = (lookAt.eye-edge);

        float dolly_dist = e_dir.length();
        e_dir.normalize();

        if (t_dir.dot(e_dir) < 0.f)
            dolly_dist = 0.f;

        dolly.m_30 += dolly_dist * inverse.original.m_x;
        dolly.m_31 += dolly_dist * inverse.original.m_y;
        dolly.m_32 += dolly_dist * inverse.original.m_z;

        return dolly_dist;
    }


};

