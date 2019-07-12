#pragma once

#include "MayaNGL/Common/Common.h"


class ViewAxis
{
    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &scene_view;

    private:
        mc::Transform m_model;
        mc::View m_view;
        mc::Projection m_projection;
        Vertices m_coordinates;
        VAOPtr m_vao;

    public:
        ViewAxis( const mc::View &view_ );

        void initialize();
        void resize(float aspectRatio_);
        void loadShader() const;
        void draw() const;

        ~ViewAxis() noexcept = default;
};
