
#include "Viewport/Camera.h"


Camera::Camera( const Mouse &mouse_,
                const LookAt &lookAt_ )
                :
                mouse(mouse_),
                m_lookAt(lookAt_),
                m_inverse(*this),
                m_currentView(View::PERSPECTIVE)
{;}

vc::Transform Camera::computeTransform()
{
    return m_dolly * m_pan * m_track;
}

void Camera::pan()
{
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
    m_lookAt.eye.m_y += Ty;
    m_lookAt.eye.m_z -= Tz;

    m_lookAt.target.m_x -= Tx;
    m_lookAt.target.m_y += Ty;
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

void Camera::focusOn(const vc::Position &target_)
{

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
