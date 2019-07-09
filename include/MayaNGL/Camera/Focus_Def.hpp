#pragma once

#include "Focus.h"


template<typename CAM>
Focus<CAM>::Focus( CAM &camera_ )
                   :
                   camera(camera_)
{;}

template<typename CAM>
    template<typename SEL>
void Focus<CAM>::onSelected(const SEL &select_)
{
    auto &&selectables = select_.getAllSelectables();
    auto &&currently_selected = select_.getCurrentlySelected();

    if (currently_selected.empty())
        return;

    auto && lookAt = camera.m_lookAt;
    auto && inverse = camera.m_inverse;

    auto target_transform = selectables.at(currently_selected[0]).getTransform();
    mc::Position target_position(target_transform.m_30,target_transform.m_31,target_transform.m_32);
    mc::Size<> target_size(target_transform.m_00,target_transform.m_11,target_transform.m_22);

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
    lookAt.front = -inverse.current;
}

template<typename CAM>
mc::Position Focus<CAM>::track(float dist_, mc::Position &axis_)
{
    if (dist_ < ngl::EPSILON)
        return mc::Position::zero();

    auto &&track = camera.m_track;

    axis_.normalize();
    auto transform = dist_*axis_;

    track.m_30 += transform.m_x;
    track.m_31 += transform.m_y;
    track.m_32 += transform.m_z;

    return transform;
}

template<typename CAM>
mc::Rotation Focus<CAM>::pan(float dist_, const mc::Position &target_pos_)
{
    if (dist_ < ngl::EPSILON)
        return mc::Rotation();

    auto &&lookAt = camera.m_lookAt;
    auto &&inverse = camera.m_inverse;
    auto &&pan = camera.m_pan;

    inverse.calcCurrent();
    inverse.calcShadow();

    float rotation_dist = (lookAt.eye-target_pos_).length();
    auto phi = asin((target_pos_.m_y-lookAt.eye.m_y)/rotation_dist) * 0.25f;
    auto theta = asin((target_pos_.m_x-lookAt.eye.m_x)/(rotation_dist*cos(phi))) * 0.5f;

    mc::Rotation Ry = mc::Y_Matrix(theta);
    inverse.shadow = inverse.shadow*Ry;
    auto rotationAxis = lookAt.up.cross(inverse.shadow);
    rotationAxis.normalize();
    mc::Rotation Rx = mc::Axis_Matrix(phi,rotationAxis);
    mc::Rotation localR = Rx*Ry;

    pan *= localR.inverse();
    return localR;
}

template<typename CAM>
float Focus<CAM>::dolly(const mc::Position &target_pos_, const mc::Size<> &target_size_)
{
    auto &&lookAt = camera.m_lookAt;
    auto &&inverse = camera.m_inverse;
    auto &&dolly = camera.m_dolly;

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
