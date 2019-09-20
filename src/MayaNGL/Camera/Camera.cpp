
#include "MayaNGL/Camera/Camera.h"


Camera::Camera( const Mouse &mouse_,
                const mc::LookAt &lookAt_ )
                :
                mouse(mouse_),
                m_lookAt(lookAt_),
                m_inverse(*this),
                m_current_view(mc::CamView::PERSPECTIVE),
                m_track(),
                m_dolly(),
                m_pan()
{;}

Camera::LocalHVAxis Camera::calc_local_hv_axis() const
{
    auto horizontal_axis = m_lookAt.up.cross(m_inverse.shadow);
    horizontal_axis.normalize();

    if (mc::absl(horizontal_axis) == mc::absl(m_inverse.current))
        horizontal_axis = m_inverse.shadow;

    auto vertical_axis = horizontal_axis.cross(m_inverse.current);
    vertical_axis.normalize();

    return LocalHVAxis{{horizontal_axis,vertical_axis}};
}

mc::Transform Camera::compute_transform() const
{
    return m_dolly *  m_pan * m_track;
}

void Camera::track()
{
    m_inverse.calc_current();

    auto hv_axis = calc_local_hv_axis();

    float dist_from_cam = m_lookAt.calc_dist()*0.0025f;
    auto mouse_move = mouse.get_drag() * dist_from_cam;

    auto Tx = mouse_move.m_x * hv_axis[0].m_x +  mouse_move.m_y * hv_axis[1].m_x;
    auto Ty = mouse_move.m_y * hv_axis[1].m_y;
    auto Tz = mouse_move.m_x * hv_axis[0].m_z +  mouse_move.m_y * hv_axis[1].m_z;

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
    m_inverse.calc_current();
    m_inverse.calc_shadow();

    mc::Rotation Ry = mc::y_matrix(mc::to_rads(mouse.get_drag().m_x));
    m_inverse.shadow = m_inverse.shadow*Ry;

    auto rotation_axis = m_lookAt.up.cross(m_inverse.shadow);
    rotation_axis.normalize();

    mc::Rotation Rx = mc::axis_matrix(mc::to_rads(mouse.get_drag().m_y),rotation_axis);
    mc::Rotation localR = Rx*Ry;

    m_lookAt.eye -= m_lookAt.target;
    m_lookAt.eye = m_lookAt.target + (m_lookAt.eye * localR);
    m_lookAt.front = m_lookAt.front * localR;

    m_pan *= localR.inverse();
}

void Camera::dolly()
{
    m_inverse.calc_current();

    float dist_from_cam = m_lookAt.calc_dist()*0.005f;
    auto mouse_move = mouse.get_drag().m_x  * dist_from_cam;

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
    m_current_view = panel_;
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

