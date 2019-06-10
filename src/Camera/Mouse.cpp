
#include "Camera/Mouse.h"


void Mouse::setAnchor(float x_, float y_)
{
    m_startingPosition = {x_,y_};
    m_previousPosition = m_startingPosition;
    m_currentPosition = m_startingPosition;
    m_direction = {1.f,1.f};
    velocity = 0.f;
}

void Mouse::setTransform(float x_, float y_)
{
    m_previousPosition = m_currentPosition;
    m_currentPosition = {x_,y_};
    if (m_previousPosition != m_currentPosition)
    {
        m_direction = m_currentPosition-m_previousPosition; //reversed
        m_direction.normalize();
        velocity = (m_previousPosition-m_currentPosition).length() * 0.5f;
    }
}

void Mouse::reset()
{
    m_startingPosition = {0.f,0.f};
    m_previousPosition = m_startingPosition;
    m_currentPosition = m_startingPosition;
    m_direction = {1.f,1.f};
    velocity = 0.f;
}
