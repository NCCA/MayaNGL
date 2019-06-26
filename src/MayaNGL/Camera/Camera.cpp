
#include "MayaNGL/Camera/Camera.h"


Camera::Camera( const Mouse &mouse_,
                const mc::LookAt &lookAt_ )
                :
                mouse(mouse_),
                m_lookAt(lookAt_),
                m_inverse(*this),
                m_currentView(mc::CamView::PERSPECTIVE),
                m_track(),
                m_dolly(),
                m_pan()
{;}

mc::Transform Camera::computeTransform() const
{
    return m_dolly *  m_pan * m_track;
}

void Camera::track()
{
    m_inverse.calcCurrent();

    auto horizontal_axis = m_lookAt.up.cross(m_inverse.shadow);
    horizontal_axis.normalize();

    if (mc::absl(horizontal_axis) == mc::absl(m_inverse.current))
        horizontal_axis = m_inverse.shadow;

    auto vertical_axis = horizontal_axis.cross(m_inverse.current);
    vertical_axis.normalize();

    float dist_from_cam = m_lookAt.calcDist()*0.0025f;
    auto mouse_move = mouse.getDrag() * dist_from_cam;

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

void Camera::pan()
{
    m_inverse.calcCurrent();
    m_inverse.calcShadow();

    mc::Rotation Ry = mc::Y_Matrix(mc::toRads(mouse.getDrag().m_x));
    m_inverse.shadow = m_inverse.shadow*Ry;

    auto rotationAxis = m_lookAt.up.cross(m_inverse.shadow);
    rotationAxis.normalize();

    mc::Rotation Rx = mc::Axis_Matrix(mc::toRads(mouse.getDrag().m_y),rotationAxis);
    mc::Rotation localR = Rx*Ry;

    m_lookAt.eye -= m_lookAt.target;
    m_lookAt.eye = m_lookAt.target + (m_lookAt.eye * localR);
    m_lookAt.front = m_lookAt.front * localR;

    m_pan *= localR.inverse();
}

void Camera::dolly()
{
    m_inverse.calcCurrent();

    float dist_from_cam = m_lookAt.calcDist()*0.005f;
    auto mouse_move = mouse.getDrag().m_x  * dist_from_cam;

    if (m_lookAt.front.dot(m_inverse.current) > 0.f)
        m_lookAt.eye += mouse_move * m_inverse.current;
    else
        m_lookAt.eye -= mouse_move * m_inverse.current;

    m_dolly.m_30 += mouse_move * m_inverse.original.m_x;
    m_dolly.m_31 += mouse_move * m_inverse.original.m_y;
    m_dolly.m_32 += mouse_move * m_inverse.original.m_z;
}

void Camera::reset(const mc::LookAt &lookAt_, mc::CamView panel_)
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
    reset(mc::LookAt{{0.f,0.f,28.f}},mc::CamView::FRONT);
}

void Camera::side()
{
    reset(mc::LookAt{{28.f,0.f,0.f}},mc::CamView::SIDE);
}

void Camera::top()
{
    reset(mc::LookAt{{0.f,28.f,0.f},mc::Position::zero(),mc::Direction::out()},mc::CamView::TOP);
}

