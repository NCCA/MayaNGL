
#include "Viewport/Mouse.h"


void Mouse::setAnchor(float x_, float y_)
{
    reset(x_,y_);
}

void Mouse::setTransform(float x_, float y_)
{
    m_previous = m_current;
    m_current = {x_,y_};
    if (m_previous != m_current)
    {
        m_direction = m_current-m_previous;
        m_velocity = m_direction.length() * 0.5f;
        m_direction.normalize();
    }
}

void Mouse::reset(float x_, float y_)
{
    m_previous = {x_,y_};
    m_current = m_previous;
    m_direction = {1.f,1.f};
    m_velocity = 0.f;
}
