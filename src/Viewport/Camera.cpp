
#include "Viewport/Camera.h"



Camera::Direction Camera::updateInverse()
{
    m_inverse = m_position-m_lookAt;
    m_inverse.normalize();
    return m_inverse;
}

Camera::Camera( const Mouse &mouse_ )
                :
                mouse(mouse_),
                m_position(28.f,21.f,28.f),
                m_orig_position(m_position),
                m_lookAt(ngl::Vec3::zero()),
                m_inverse(updateInverse()),
                m_upVector(ngl::Vec3::up()),
                m_currentView(View::PERSPECTIVE)
{;}

void Camera::pan()
{
    updateInverse();

//    auto rotationAxis = sm::absl(m_upVector.cross(m_inverse));
    auto rotationAxis = (m_upVector.cross(m_inverse));
    rotationAxis.normalize();

    ngl::Mat4 R = sm::Y_Matrix(sm::toRads(mouse.getVelocity()*mouse.getDirection().m_x)) *
                  sm::Axis_Matrix(sm::toRads(mouse.getVelocity()*mouse.getDirection().m_y),rotationAxis);

    Rot *= R.inverse();

    m_position -= m_lookAt;
    m_position = m_lookAt + (m_position * R);
}

void Camera::dolly()
{
    updateInverse();

    auto dist = (m_position-m_lookAt).length();

    if(dist<0.5f) // this needs work
    {
        auto dir = -m_inverse;
        m_lookAt += dist*dir;
    }

    m_position += mouse.getVelocity() * mouse.getDirection().m_x * m_inverse * Mouse::slowdown;

//    Rot.translate(m_position.m_x,m_position.m_y,m_position.m_z);
}

void Camera::track()
{
//    updateInverse();

//    auto localTrackAxis = m_upVector.cross(m_inverse);
//    localTrackAxis.normalize();

//    switch(m_currentView)
//    {
//        case View::TOP:
////            m_position.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
////            m_position.m_z += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;

////            m_lookAt.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
////            m_lookAt.m_z += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
//            break;

//        default:
////            m_position.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
////            m_position.m_y += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
////            m_position.m_z -= mouse.velocity * localTrackAxis.m_z * mouse.getDirection().m_x * Mouse::slowdown;

////            m_lookAt.m_x -= mouse.velocity * localTrackAxis.m_x * mouse.getDirection().m_x * Mouse::slowdown;
////            m_lookAt.m_y += mouse.velocity * mouse.getDirection().m_y * Mouse::slowdown;
////            m_lookAt.m_z -= mouse.velocity * localTrackAxis.m_z * mouse.getDirection().m_x * Mouse::slowdown;
//            break;
//    }
}

void Camera::reset(Position &&pos_, View panel_, Direction &&up_)
{
    m_position = pos_;
    m_lookAt = ngl::Vec3::zero();
    m_upVector = up_;
    m_inverse = updateInverse();
    m_currentView = panel_;
    Rot.identity();
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


