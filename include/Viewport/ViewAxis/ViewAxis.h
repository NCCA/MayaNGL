#pragma once

#include "Viewport/Common.h"


struct ViewAxis
{
    private:
        typedef std::array<vc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const vc::View &scene_view;

    private:
        vc::Transform m_model;
        vc::View m_view;
        vc::Projection m_projection;
        Vertices m_coordinates;
        VAOPtr m_vao;

    public:
        ViewAxis( const vc::View &view_ );

        void initialize();
        void resize(float aspectRatio_);
        void loadLineColourShader() const;
        void draw() const;

        ~ViewAxis() noexcept = default;
};