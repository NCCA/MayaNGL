#pragma once

#include "Viewport.h"


template<mc::CamView V>
void Viewport::set_view()
{
    m_projected_text.view_title<V>();
    m_grid.view_orientation<V>();
    m_orthographic_zoom = 10.f;
    go_ortho();
    m_initial_view = ngl::lookAt(camera.get_eye(),camera.get_target(),camera.get_up());
}

template<>
void Viewport::set_view<mc::CamView::PERSPECTIVE>();
