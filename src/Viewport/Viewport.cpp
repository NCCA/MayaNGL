
#include "Viewport/Viewport.h"



Viewport::Viewport( vc::View &view_,
                    vc::Projection &projection_ )
                    :
                    view(view_),
                    projection(projection_),
                    m_aspectRatio(0.f),
                    m_orthographic_zoom(10.f),
                    m_initial_view(view),
                    m_mouse(),
                    m_camera(m_mouse),
                    m_grid(view,projection),
                    m_projText(),
                    m_axis(view)
{;}

void Viewport::resize(int w_, int h_)
{
    m_aspectRatio = static_cast<float>(w_)/h_;

    if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
        goOrtho();
    else
        goPersp();

    m_projText.resize(w_,h_);
    m_axis.resize(m_aspectRatio);
}

void Viewport::goPersp()
{
    projection = ngl::perspective(35.0f,m_aspectRatio,0.1f,200.f);
}

void Viewport::goOrtho()
{
    projection = ngl::ortho(-m_aspectRatio*m_orthographic_zoom,m_aspectRatio*m_orthographic_zoom,-m_orthographic_zoom,m_orthographic_zoom,0.1f,200.f);
}

void Viewport::initialize()
{
    m_initial_view = view;

    m_grid.initialize();
    m_projText.initialize();
    m_axis.initialize();
}

void Viewport::update_draw()
{
    view = m_initial_view * m_camera.getTransform();

    m_grid.draw();
    m_projText.draw();
    m_axis.draw();
}

void Viewport::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_projText.title = "persp";
            m_camera.reset();
            m_mouse.reset();
            m_grid.reset();
            goPersp();
            m_initial_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_1:
            m_projText.title = "front";
            m_camera.front();
            m_mouse.reset();
            m_grid.front();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_2:
            m_projText.title = "side";
            m_camera.side();
            m_mouse.reset();
            m_grid.side();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_3:
            m_projText.title = "top";
            m_camera.top();
            m_mouse.reset();
            m_grid.reset();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::out());
            break;

        default:
            break;
    }
}

void Viewport::mousePress(QMouseEvent *event_)
{
    if(event_->modifiers() & Qt::AltModifier)
        m_mouse.setAnchor(event_->x(),event_->y());
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
                {
                    m_orthographic_zoom -= m_mouse.getDrag().m_x * Mouse::slowdown;
                    goOrtho();
                }
                break;

            default:
                break;
        }
    }
}




