
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
        m_direction = m_current-m_previous; //reversed
        m_direction.normalize();
        velocity = (m_previous-m_current).length() * 0.5f;
    }
}

void Mouse::reset(float x_, float y_)
{
    m_start = {x_,y_};
    m_previous = m_start;
    m_current = m_start;
    m_direction = {1.f,1.f};
    velocity = 0.f;
}
