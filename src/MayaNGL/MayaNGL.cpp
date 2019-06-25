
#include "MayaNGL/MayaNGL.h"


MayaNGL::MayaNGL( mc::View &view_,
                  mc::Projection &projection_ )
                  :
                  view(view_),
                  projection(projection_),
                  m_mouse()
{;}

void MayaNGL::initialize()
{

}

void MayaNGL::resize(int w_, int h_)
{

}

void MayaNGL::update_draw()
{

}

void MayaNGL::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_mouse.reset();
            break;

        case Qt::Key_Space:
            m_mouse.reset();
            break;

        case Qt::Key_1:
            m_mouse.reset();
            break;

        case Qt::Key_2:
            m_mouse.reset();
            break;

        case Qt::Key_3:
            m_mouse.reset();
            break;

        default:
            break;
    }
}

void MayaNGL::mousePress(QMouseEvent *event_)
{
    switch(event_->buttons())
    {
        case Qt::LeftButton:
            if(event_->modifiers() & Qt::AltModifier)
            {
                m_mouse.setAnchor(event_->x(),event_->y());
                break;
            }
            break;

        default:
            break;
    }
}

void MayaNGL::mouseMove(QMouseEvent *event_)
{
    m_mouse.setTransform(event_->x(),event_->y());

    if(event_->modifiers() & Qt::AltModifier)
    {
        switch(event_->buttons())
        {
            case Qt::LeftButton :
                break;

            case Qt::MiddleButton:
                break;

            case Qt::RightButton:
                break;

            default:
                break;
        }
    }
}
