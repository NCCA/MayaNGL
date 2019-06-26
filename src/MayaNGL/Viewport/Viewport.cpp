
#include "MayaNGL/Viewport/Viewport_Def.hpp"


Viewport::Viewport( mc::View &view_,
                    mc::Projection &projection_,
                    const Camera &camera_ )
                    :
                    view(view_),
                    projection(projection_),
                    camera(camera_),
                    m_aspectRatio(0.f),
                    m_orthographic_zoom(10.f),
                    m_initial_view(view),
                    m_grid(view,projection,camera),
                    m_projText(),
                    m_axis(view)
{;}

void Viewport::initialize()
{
    m_grid.initialize();
    m_projText.initialize();
    m_axis.initialize();
    m_initial_view = view;
}

void Viewport::resize(int w_, int h_)
{
    m_aspectRatio = static_cast<float>(w_)/h_;

    if (camera.getCurrentView() != mc::CamView::PERSPECTIVE)
        goOrtho();
    else
        goPersp();

    m_projText.resize(w_,h_);
    m_axis.resize(m_aspectRatio);
}

void Viewport::goPersp()
{
    projection = ngl::perspective(mc::fov,m_aspectRatio,mc::near_clip,mc::far_clip);
}

void Viewport::goOrtho()
{
    projection = ngl::ortho( -m_aspectRatio*m_orthographic_zoom,
                              m_aspectRatio*m_orthographic_zoom,
                             -m_orthographic_zoom,
                              m_orthographic_zoom,
                              mc::near_clip,
                              mc::far_clip );
}

void Viewport::update_draw()
{
    view = m_initial_view * camera.computeTransform();

    m_grid.draw();
    m_projText.draw();
    m_axis.draw();
}

template<>
void Viewport::setView<mc::CamView::PERSPECTIVE>()
{
    m_projText.viewTitle<mc::CamView::PERSPECTIVE>();
    m_grid.viewOrientation<mc::CamView::PERSPECTIVE>();
    goPersp();
    m_initial_view = ngl::lookAt(camera.getEye(),camera.getTarget(),camera.getUp());
}

