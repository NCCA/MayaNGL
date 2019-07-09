#pragma once

#include "MayaNGL/Viewport/Grid/Grid.h"


template<typename C>
Grid::Grid( const mc::View &view_,
            const mc::Projection &projection_,
            const C &camera_ )
            :
            view(view_),
            projection(projection_),
            lookAt(camera_.getLookAt())
{;}

template<mc::CamView>
void Grid::viewOrientation()
{
    m_model.identity();
}

template<>
void Grid::viewOrientation<mc::CamView::FRONT>();

template<>
void Grid::viewOrientation<mc::CamView::SIDE>();

