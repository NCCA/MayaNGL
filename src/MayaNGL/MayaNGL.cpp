
#include "MayaNGL/MayaNGL.h"


MayaNGL::MayaNGL( mc::View &view_,
                  mc::Projection &projection_ )
                  :
                  view(view_),
                  projection(projection_),
                  m_initial_lookAt(),
                  m_mouse(),
                  m_camera(m_mouse,m_initial_lookAt),
                  m_viewport(view,projection,m_camera),
                  m_select(view,projection,m_camera)
{;}

void MayaNGL::initialize()
{
    m_viewport.initialize();
    m_select.initialize();
    m_viewport.setView<mc::CamView::PERSPECTIVE>();
}

void MayaNGL::resize(int w_, int h_)
{
    m_viewport.resize(w_,h_);
    m_select.resize(w_,h_);
}

void MayaNGL::draw()
{
    m_viewport.update_draw();
    m_select.draw();
}

void MayaNGL::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_mouse.reset();
            m_camera.focus(m_select);
            break;

        case Qt::Key_Space:
            m_mouse.reset();
            m_camera.reset(m_initial_lookAt);
            m_viewport.setView<mc::CamView::PERSPECTIVE>();
            break;

        case Qt::Key_1:
            m_mouse.reset();
            m_camera.front();
            m_viewport.setView<mc::CamView::FRONT>();
            break;

        case Qt::Key_2:
            m_mouse.reset();
            m_camera.side();
            m_viewport.setView<mc::CamView::SIDE>();
            break;

        case Qt::Key_3:
            m_mouse.reset();
            m_camera.top();
            m_viewport.setView<mc::CamView::TOP>();
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
            if(event_->modifiers() & Qt::ShiftModifier)
                m_select.enableMultiSelection();
            m_select.pick(event_->x(),event_->y());
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
            case Qt::LeftButton:
                if (m_camera.getCurrentView() == mc::CamView::PERSPECTIVE)
                    m_camera.pan();
                break;

            case Qt::MiddleButton:
                m_camera.track();
                break;

            case Qt::RightButton:
                m_camera.dolly();
//                if (m_camera.getCurrentView() != mc::CamView::PERSPECTIVE)
//                { //--------------- this needs work ---------------
//                    std::cout<< m_orthographic_zoom <<std::endl;
//                    if ((m_orthographic_zoom < 0.1f) && (m_mouse.getDrag().m_x > 0.f))
//                        std::cout<< "that's it!" <<std::endl;
//                    else
//                        m_orthographic_zoom -= m_mouse.getDrag().m_x * Mouse::slowdown;
//                    goOrtho();
//                } //-----------------------------------------------
                break;

            default:
                break;
        }
    }
}



