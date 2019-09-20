
#include "MayaNGL/Mouse/Mouse.h"


void Mouse::set_anchor(float x_, float y_)
{
    reset(x_,y_);
}

void Mouse::set_transform(float x_, float y_)
{
    m_previous = m_current;
    m_current = {x_,y_};
    if (m_previous != m_current)
    {
        m_direction = m_current-m_previous;
        m_velocity = m_direction.length() * 0.5f;
        m_direction.normalize();
        m_drag = m_velocity*m_direction;
    }
}

void Mouse::reset(float x_, float y_)
{
    m_previous = {x_,y_};
    m_current = m_previous;
    m_direction = {1.f,1.f};
    m_velocity = 0.f;
}
