#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"
#include "Grid/Grid_Def.hpp"
#include "ProjectionText/ProjectionText.h"
#include "ViewAxis/ViewAxis.h"


class Viewport
{
    private:
        mc::View &view;
        mc::Projection &projection;
        const Camera &camera;

    private:
        float m_aspectRatio;
        float m_orthographic_zoom;
        mc::View m_initial_view;

        Grid m_grid;
        ProjectionText m_projText;
        ViewAxis m_axis;

    private:
        void goPersp();
        void goOrtho();

    public:
        explicit Viewport( mc::View &view_,
                           mc::Projection &projection_,
                           const Camera &camera_ );

        template<mc::CamView V>
        void setView();

        void initialize();
        void resize(int w_, int h_);
        void update_draw();
        void orthoZoom(const mc::V2 &mouseDrag_);

        ~Viewport() noexcept = default;
};

