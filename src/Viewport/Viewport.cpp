
#include "Viewport/Viewport.h"



Viewport::Viewport( vc::View &view_,
                    vc::Projection &projection_ )
                    :
                    view(view_),
                    projection(projection_),
                    m_aspectRatio(0.f),
                    m_orthographic_zoom(10.f),
                    m_initial_lookAt(),
                    m_initial_view(view),
                    m_mouse(),
                    m_camera(m_mouse,m_initial_lookAt),
                    m_grid(view,projection,m_camera),
                    m_projText(),
                    m_axis(view),
                    m_select(m_camera.getLookAt(),view,projection)
{;}

void Viewport::initialize()
{
    m_grid.initialize();
    m_projText.initialize();
    m_axis.initialize();
    m_select.initialize();

    m_initial_view = view;
}

void Viewport::resize(int w_, int h_)
{
    m_aspectRatio = static_cast<float>(w_)/h_;

    if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
        goOrtho();
    else
        goPersp();

    m_projText.resize(w_,h_);
    m_axis.resize(m_aspectRatio);
    m_select.resize(w_,h_);
}

void Viewport::goPersp()
{
    projection = ngl::perspective(vc::fov,m_aspectRatio,vc::near_clip,vc::far_clip);
}

void Viewport::goOrtho()
{
    projection = ngl::ortho( -m_aspectRatio*m_orthographic_zoom,
                              m_aspectRatio*m_orthographic_zoom,
                             -m_orthographic_zoom,
                              m_orthographic_zoom,
                              vc::near_clip,
                              vc::far_clip );
}

void Viewport::update_draw()
{
    view = m_initial_view * m_camera.computeTransform();

    m_grid.draw();
    m_projText.draw();
    m_axis.draw();
    m_select.draw();
}

void Viewport::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_camera.focus(m_select);
            m_mouse.reset();
            m_grid.reset();
//            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_Space:
            m_projText.title = "persp";
            m_camera.reset(m_initial_lookAt);
            m_mouse.reset();
            m_grid.reset();
            goPersp();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_1:
            m_projText.title = "front";
            m_camera.front();
            m_mouse.reset();
            m_grid.front();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_2:
            m_projText.title = "side";
            m_camera.side();
            m_mouse.reset();
            m_grid.side();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_3:
            m_projText.title = "top";
            m_camera.top();
            m_mouse.reset();
            m_grid.reset();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        default:
            break;
    }
}

void Viewport::mousePress(QMouseEvent *event_)
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

void Viewport::mouseMove(QMouseEvent *event_)
{
    m_mouse.setTransform(event_->x(),event_->y());

    if(event_->modifiers() & Qt::AltModifier)
    {
        switch(event_->buttons())
        {
            case Qt::LeftButton :
                if (m_camera.getCurrentView() == Camera::View::PERSPECTIVE)
                    m_camera.pan();
                break;

            case Qt::MiddleButton:
                m_camera.track();
                break;

            case Qt::RightButton:
                m_camera.dolly();
                if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
                { //--------------- this needs work ---------------
                    std::cout<< m_orthographic_zoom <<std::endl;
                    if ((m_orthographic_zoom < 0.1f) && (m_mouse.getDrag().m_x > 0.f))
                        std::cout<< "that's it!" <<std::endl;
                    else
                        m_orthographic_zoom -= m_mouse.getDrag().m_x * Mouse::slowdown;
                    goOrtho();
                } //-----------------------------------------------
                break;

            default:
                break;
        }
    }
}




