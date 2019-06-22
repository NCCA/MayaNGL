
#include "Viewport/Camera/Camera.h"


Camera::Camera( const Mouse &mouse_,
                const LookAt &lookAt_ )
                :
                mouse(mouse_),
                m_lookAt(lookAt_),
                m_inverse(*this),
                m_currentView(View::PERSPECTIVE),
                m_track(),
                m_dolly(),
                m_pan()
{;}

vc::Transform Camera::computeTransform()
{
    return m_dolly * m_pan * m_track;
}

void Camera::pan()
{
    m_inverse.calcCurrent();
    m_inverse.calcShadow();

    vc::Rotation Ry = vc::Y_Matrix(vc::toRads(mouse.getDrag().m_x));
    m_inverse.shadow = m_inverse.shadow*Ry;

    auto rotationAxis = m_lookAt.up.cross(m_inverse.shadow);
    rotationAxis.normalize();

    vc::Rotation Rx = vc::Axis_Matrix(vc::toRads(mouse.getDrag().m_y),rotationAxis);
    vc::Rotation localR = Rx*Ry;

    m_lookAt.eye -= m_lookAt.target;
    m_lookAt.eye = m_lookAt.target + (m_lookAt.eye * localR);

    m_pan *= localR.inverse();
}

void Camera::dolly()
{
    m_inverse.calcCurrent();

    auto dist = m_lookAt.calcDist();
    if (dist < 1.0f)
        m_lookAt.target += 10.f * m_inverse.current;

    auto mouse_move = mouse.getDrag().m_x  * Mouse::slowdown;
    m_lookAt.eye -= mouse_move * m_inverse.current;

    m_dolly.m_30 += mouse_move * m_inverse.original.m_x;
    m_dolly.m_31 += mouse_move * m_inverse.original.m_y;
    m_dolly.m_32 += mouse_move * m_inverse.original.m_z;
}

void Camera::track()
{
    m_inverse.calcCurrent();

    auto horizontal_axis = m_lookAt.up.cross(m_inverse.shadow);
    horizontal_axis.normalize();

    if (vc::absl(horizontal_axis) == vc::absl(m_inverse.current))
        horizontal_axis = m_inverse.shadow;

    auto vertical_axis = horizontal_axis.cross(m_inverse.current);
    vertical_axis.normalize();

    auto mouse_move = mouse.getDrag() * Mouse::slowdown;

    auto Tx = mouse_move.m_x * horizontal_axis.m_x +  mouse_move.m_y * vertical_axis.m_x;
    auto Ty = mouse_move.m_y * vertical_axis.m_y;
    auto Tz = mouse_move.m_x * horizontal_axis.m_z +  mouse_move.m_y * vertical_axis.m_z;

    m_lookAt.eye.m_x -= Tx;
    m_lookAt.eye.m_y -= Ty;
    m_lookAt.eye.m_z -= Tz;

    m_lookAt.target.m_x -= Tx;
    m_lookAt.target.m_y -= Ty;
    m_lookAt.target.m_z -= Tz;

    m_track.m_30 += Tx;
    m_track.m_31 += Ty;
    m_track.m_32 += Tz;
}

void Camera::reset(const LookAt &lookAt_, View panel_)
{
    m_lookAt = lookAt_;
    m_currentView = panel_;
    m_inverse.reset();
    m_track.identity();
    m_dolly.identity();
    m_pan.identity();
}

void Camera::front()
{
    reset(LookAt{{0.f,0.f,28.f}},View::FRONT);
}

void Camera::side()
{
    reset(LookAt{{28.f,0.f,0.f}},View::SIDE);
}

void Camera::top()
{
    reset(LookAt{{0.f,28.f,0.f},vc::Position::zero(),vc::Direction::out()},View::TOP);
}



vc::Position Camera::focus_track(vc::Position &axis_,float dist_)
{
    axis_.normalize();

    auto transform = dist_*axis_;

    m_track.m_30 += transform.m_x;
    m_track.m_31 += transform.m_y;
    m_track.m_32 += transform.m_z;

    return transform;
}

vc::Rotation Camera::focus_pan(const vc::Position &target_pos_)
{
    m_inverse.calcCurrent();
    m_inverse.calcShadow();

    float rotation_dist = (m_lookAt.eye-target_pos_).length();
    auto phi = asin((target_pos_.m_y-m_lookAt.eye.m_y)/rotation_dist);
    auto theta = asin((target_pos_.m_x-m_lookAt.eye.m_x)/(rotation_dist*cos(phi)));

    vc::Rotation Ry = vc::Y_Matrix(theta);
    m_inverse.shadow = m_inverse.shadow*Ry;
    auto rotationAxis = m_lookAt.up.cross(m_inverse.shadow);
    rotationAxis.normalize();
    vc::Rotation Rx = vc::Axis_Matrix(phi*0.5f,rotationAxis);
    vc::Rotation localR = Rx*Ry;

    m_pan *= localR.inverse();

    return localR;
}

float Camera::focus_dolly(const vc::Position &target_pos_, const vc::Size &target_size_)
{
    /* THIS NEEDS MORE WORK */
    float dolly_dist = 2.f * ((m_lookAt.eye-target_pos_).length()-target_size_.length());

    std::cout<< dolly_dist <<std::endl;

    m_dolly.m_30 += dolly_dist * m_inverse.original.m_x;
    m_dolly.m_31 += dolly_dist * m_inverse.original.m_y;
    m_dolly.m_32 += dolly_dist * m_inverse.original.m_z;

    return dolly_dist;
}
















