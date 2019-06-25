#pragma once

#include "MayaNGL/Viewport/Grid/Grid.h"


template<typename CAM>
Grid::Grid( const vc::View &view_,
            const vc::Projection &projection_,
            const CAM &camera_ )
            :
            view(view_),
            projection(projection_),
            camera(camera_.getLookAt())
{;}
