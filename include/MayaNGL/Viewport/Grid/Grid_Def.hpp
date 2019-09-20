#pragma once

#include "MayaNGL/Viewport/Grid/Grid.h"


template<typename C>
Grid::Grid( const mc::View &view_,
            const mc::Projection &projection_,
            const C &camera_ )
            :
            view(view_),
            projection(projection_),
            lookAt(camera_.get_lookAt())
{;}

template<mc::CamView>
void Grid::view_orientation()
{
    m_model.identity();
}

template<>
void Grid::view_orientation<mc::CamView::FRONT>();

template<>
void Grid::view_orientation<mc::CamView::SIDE>();

