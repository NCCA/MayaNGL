
#include "Viewport/Camera.h"


Camera::Camera( const Mouse &mouse_ )
                :
                mouse(mouse_),
                m_position(10.f,21.f,28.f),
                m_lookAt(10.f,0.f,0.f),//ngl::Vec3::zero()),
                m_inverse(*this),
                m_currentView(View::PERSPECTIVE)
{;}

void Camera::pan()
{
    m_inverse.update();

    vc::Rotation Ry = vc::Y_Matrix(vc::toRads(mouse.getDrag().m_x));
    m_inverse.shadow = m_inverse.shadow*Ry;

    auto rotationAxis = ngl::Vec3::up().cross(m_inverse.shadow);
    rotationAxis.normalize();

    vc::Rotation Rx = vc::Axis_Matrix(vc::toRads(mouse.getDrag().m_y),rotationAxis);
    vc::Rotation localR = Rx*Ry;

    m_position -= m_lookAt;
    m_position = m_lookAt + (m_position * localR);

    // this works, but needs refactoring...
    vc::Translation tmp;
    tmp.m_30 = -10.f;
    tmp.m_31 = 0.f;
    tmp.m_32 = 0.f;

    m_transform.m_30 = 10.f;
    m_transform.m_31 = 0.f;
    m_transform.m_32 = 0.f;
    m_transform *= localR.inverse() * tmp;
}

void Camera::dolly()
{
    m_inverse.update();

    auto mouse_move = mouse.getDrag().m_x * Mouse::slowdown;
    m_position -= mouse_move * m_inverse.current;

    auto translate = mouse_move * m_inverse.original;

    m_transform.m_30 += translate.m_x;
    m_transform.m_31 += translate.m_y;
    m_transform.m_32 += translate.m_z;
}

void Camera::track()
{
    m_inverse.update();

    auto horizontal_axis = ngl::Vec3::up().cross(m_inverse.shadow);
    horizontal_axis.normalize();

    auto vertical_axis = horizontal_axis.cross(m_inverse.current);
    vertical_axis.normalize();

    auto mouse_move = mouse.getDrag() * Mouse::slowdown;

    auto Tx = mouse_move.m_x * horizontal_axis.m_x +  mouse_move.m_y * vertical_axis.m_x;
    auto Ty = mouse_move.m_y * vertical_axis.m_y;
    auto Tz = mouse_move.m_x * horizontal_axis.m_z +  mouse_move.m_y * vertical_axis.m_z;

    m_position.m_x -= Tx;
    m_position.m_y += Ty;
    m_position.m_z -= Tz;

    m_lookAt.m_x -= Tx;
    m_lookAt.m_y += Ty;
    m_lookAt.m_z -= Tz;

    vc::Translation tmp;
    tmp.m_30 = Tx;
    tmp.m_31 = Ty;
    tmp.m_32 = Tz;

    m_transform *= tmp;
}

void Camera::reset(vc::Position &&pos_, View panel_)
{
    m_position = pos_;
    m_lookAt = vc::Position::zero();
    m_inverse.reset();
    m_currentView = panel_;
    m_transform.identity();
}

void Camera::focusOn(const vc::Position &target_)
{

}

void Camera::front()
{
    reset({0.f,0.f,28.f},View::FRONT);
}

void Camera::side()
{
    reset({28.f,0.f,0.f},View::SIDE);
}

void Camera::top()
{
    reset({0.f,28.f,0.f},View::TOP);
}


