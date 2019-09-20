
#include "MayaNGL/Viewport/Viewport_Def.hpp"


Viewport::Viewport( mc::View &view_,
                    mc::Projection &projection_,
                    const Camera &camera_ )
                    :
                    view(view_),
                    projection(projection_),
                    camera(camera_),
                    m_aspect_ratio(0.f),
                    m_orthographic_zoom(10.f),
                    m_initial_view(view),
                    m_grid(view,projection,camera),
                    m_projected_text(),
                    m_axis(view)
{;}

void Viewport::initialize()
{
    m_grid.initialize();
    m_projected_text.initialize();
    m_axis.initialize();
    m_initial_view = view;
}

void Viewport::resize(int w_, int h_)
{
    m_aspect_ratio = static_cast<float>(w_)/h_;

    if (camera.get_current_view() != mc::CamView::PERSPECTIVE)
        go_ortho();
    else
        go_persp();

    m_projected_text.resize(w_,h_);
    m_axis.resize(m_aspect_ratio);
}

void Viewport::go_persp()
{
    projection = ngl::perspective(mc::fov,m_aspect_ratio,mc::near_clip,mc::far_clip);
}

void Viewport::go_ortho()
{
    projection = ngl::ortho( -m_aspect_ratio*m_orthographic_zoom,
                              m_aspect_ratio*m_orthographic_zoom,
                             -m_orthographic_zoom,
                              m_orthographic_zoom,
                              mc::near_clip,
                              mc::far_clip );
}

void Viewport::update_draw()
{
    view = m_initial_view * camera.compute_transform();

    m_grid.draw();
    m_projected_text.draw();
    m_axis.draw();
}

template<>
void Viewport::set_view<mc::CamView::PERSPECTIVE>()
{
    m_projected_text.view_title<mc::CamView::PERSPECTIVE>();
    m_grid.view_orientation<mc::CamView::PERSPECTIVE>();
    go_persp();
    m_initial_view = ngl::lookAt(camera.get_eye(),camera.get_target(),camera.get_up());
}

void Viewport::ortho_zoom(const ngl::Vec2 &mouse_drag_)
{
    if (!((m_orthographic_zoom < 0.1f) && (mouse_drag_.m_x > 0.f)))
        m_orthographic_zoom -= mouse_drag_.m_x * 0.05f;
    go_ortho();
}





