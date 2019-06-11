
#include "Viewport/Camera.h"


Camera::Camera( const Mouse &mouse_ )
                :
                mouse(mouse_),
                m_position(28.f,21.f,28.f),
                m_lookAt(ngl::Vec3::zero()),
                m_upVector(ngl::Vec3::up()),
                m_inverseDirection(m_position-m_lookAt),
                m_currentView(View::PERSPECTIVE)
{
    m_inverseDirection.normalize();
}

void Camera::pan()
{
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();

    auto rotationAxis = m_upVector.cross(m_inverseDirection);
    rotationAxis.normalize();

    ngl::Mat3 R = sm::Y_Matrix(sm::toRads(mouse.velocity*mouse.getDirection().m_x)) *
                  sm::Axis_Matrix(sm::toRads(mouse.velocity*mouse.getDirection().m_y),rotationAxis);

    m_position -= m_lookAt;
    m_position = m_lookAt + (m_position * R);
}

void Camera::dolly()
{
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();

    auto dist = (m_position-m_lookAt).length();

    if(dist<0.5f) // this needs work
    {
        auto dir = -m_inverseDirection;
        m_lookAt += dist*dir;
    }

    m_position -= mouse.getDirection().m_x * mouse.velocity * m_inverseDirection * Mouse::slowdown;
}

void Camera::track()
{
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();

    auto localTrackAxis = m_upVector.cross(m_inverseDirection);
    localTrackAxis.normalize();

    switch(m_currentView)
    {
        case View::TOP:
            m_position.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
            m_position.m_z += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;

            m_lookAt.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
            m_lookAt.m_z += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
            break;

        default:
            m_position.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
            m_position.m_y += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
            m_position.m_z -= mouse.velocity * localTrackAxis.m_z * mouse.getDirection().m_x * Mouse::slowdown;

            m_lookAt.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
            m_lookAt.m_y += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
            m_lookAt.m_z -= mouse.velocity * localTrackAxis.m_z * mouse.getDirection().m_x * Mouse::slowdown;
            break;
    }
}

void Camera::reset(Position &&pos_, View view_, Direction &&up_)
{
    m_position = pos_;
    m_lookAt = ngl::Vec3::zero();
    m_upVector = up_;
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();
    m_currentView = view_;
}

void Camera::focusOn(const Position &target_)
{

}

void Camera::front()
{
    reset({0.f,0.f,28.f},View::FRONT);
}

void Camera::side()
{
    reset({-28.f,0.f,0.f},View::SIDE);
}

void Camera::top()
{
    reset({0.f,28.f,0.f},View::TOP,ngl::Vec3::in());
}


