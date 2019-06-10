
#include "Camera/Camera.h"


Camera::Camera() : m_position(28.f,21.f,28.f),
                   m_lookAt(ngl::Vec3::zero()),
                   m_upVector(0.f,1.f,0.f),
                   m_inverseDirection(m_position-m_lookAt),
                   m_currentView(View::PERSPECTIVE),
                   m_mouse()
{
    m_inverseDirection.normalize();
}

void Camera::pan()
{
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();

    auto rotationAxis = m_upVector.cross(m_inverseDirection);
    rotationAxis.normalize();

    auto maxTopAngle = fabs(m_inverseDirection.dot(m_upVector));
    if (maxTopAngle > 0.98f)
        m_mouse.velocity *= (1.f-maxTopAngle);

    ngl::Mat3 R = sm::Y_Matrix(sm::toRads(m_mouse.velocity*m_mouse.getDirection().m_x)) *
                  sm::Axis_Matrix(sm::toRads(m_mouse.velocity*m_mouse.getDirection().m_y),rotationAxis);

    m_position -= m_lookAt;
    m_position = m_lookAt + (m_position * R);
}

void Camera::dolly()
{
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();

    auto dist = (m_position-m_lookAt).length();

    if(dist<0.1f)
    {
        auto dir = -m_inverseDirection;
        m_lookAt += dist*dir;
    }

    m_position -= m_mouse.getDirection().m_x * m_mouse.velocity * m_inverseDirection * Mouse::slowdown;
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
            m_position.m_x -= m_mouse.velocity * localTrackAxis.m_x * m_mouse.getDirection().m_x * Mouse::slowdown;
            m_position.m_z += m_mouse.velocity * m_mouse.getDirection().m_y * Mouse::slowdown;

            m_lookAt.m_x -= m_mouse.velocity * localTrackAxis.m_x * m_mouse.getDirection().m_x * Mouse::slowdown;
            m_lookAt.m_z += m_mouse.velocity * m_mouse.getDirection().m_y * Mouse::slowdown;
            break;

        default:
            m_position.m_x -= m_mouse.velocity * localTrackAxis.m_x * m_mouse.getDirection().m_x * Mouse::slowdown;
            m_position.m_y += m_mouse.velocity * m_mouse.getDirection().m_y * Mouse::slowdown;
            m_position.m_z -= m_mouse.velocity * localTrackAxis.m_z * m_mouse.getDirection().m_x * Mouse::slowdown;

            m_lookAt.m_x -= m_mouse.velocity * localTrackAxis.m_x * m_mouse.getDirection().m_x * Mouse::slowdown;
            m_lookAt.m_y += m_mouse.velocity * m_mouse.getDirection().m_y * Mouse::slowdown;
            m_lookAt.m_z -= m_mouse.velocity * localTrackAxis.m_z * m_mouse.getDirection().m_x * Mouse::slowdown;
            break;
    }
}

void Camera::refocus()
{
    m_position = {28.f,21.f,28.f};
    m_lookAt = ngl::Vec3::zero();
    m_upVector = {0.f,1.f,0.f};
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();
    m_currentView = View::PERSPECTIVE;
    m_mouse.reset();
}

void Camera::focusOn(const Position &target_)
{

}

void Camera::front()
{
    m_position = {0.f,0.f,1.f};
    m_lookAt = ngl::Vec3::zero();
    m_upVector = {0.f,1.f,0.f};
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();
    m_currentView = View::FRONT;
    m_mouse.reset();
}

void Camera::side()
{
    m_position = {-28.f,0.f,0.f};
    m_lookAt = ngl::Vec3::zero();
    m_upVector = {0.f,1.f,0.f};
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();
    m_currentView = View::SIDE;
    m_mouse.reset();
}

void Camera::top()
{
    m_position = {0.f,60.f,0.f};
    m_lookAt = ngl::Vec3::zero();
    m_upVector = {0.f,0.f,1.f};
    m_inverseDirection = m_position-m_lookAt;
    m_inverseDirection.normalize();
    m_currentView = View::TOP;
    m_mouse.reset();
}

void Camera::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            // use perspective
            refocus();
            break;

        case Qt::Key_1:
            // use orthographic
            front();
            break;

        case Qt::Key_2:
            // use orthographic
            side();
            break;

        case Qt::Key_3:
            // use orthographic
            top();
            break;

        default:
            break;
    }
}

void Camera::mousePress(QMouseEvent *event_)
{
    if(event_->modifiers() & Qt::AltModifier)
        m_mouse.setAnchor(event_->x(),event_->y());
}

void Camera::mouseMove(QMouseEvent *event_)
{
    m_mouse.setTransform(event_->x(),event_->y());

    if(event_->modifiers() & Qt::AltModifier)
    {
        switch(event_->buttons())
        {
            case Qt::LeftButton :
                if (m_currentView == View::PERSPECTIVE)
                    pan();
                break;

            case Qt::MiddleButton:
                track();
                break;

            case Qt::RightButton:
                dolly();
                break;

            default:
                break;
        }
    }
}


