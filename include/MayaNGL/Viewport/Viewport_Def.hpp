#pragma once

#include "Viewport.h"


template<mc::CamView V>
void Viewport::setView()
{
    m_projText.viewTitle<V>();
    m_grid.viewOrientation<V>();
    m_orthographic_zoom = 10.f;
    goOrtho();
    m_initial_view = ngl::lookAt(camera.getEye(),camera.getTarget(),camera.getUp());
}

template<>
void Viewport::setView<mc::CamView::PERSPECTIVE>();
