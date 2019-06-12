#pragma once

#include "Viewport/SimpleMath.h"


struct ViewAxis
{
    private:
        typedef ngl::Transformation Transform;
        typedef ngl::Mat4 ViewMat;
        typedef ngl::Mat4 ProjectionMat;
        typedef std::array<ngl::Vec3,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> AvaoPtr;

    private:
        const ViewMat &scene_view;

    private:
        Transform m_model;
        ViewMat m_view;
        ProjectionMat m_projection;
        Vertices m_coordinates;
        AvaoPtr m_vao;

    public:
        ViewAxis( const ViewMat &scene_view_ );

        void initialize();
        void resize(float aspectRatio_);
        void loadLineColourShader();
        void draw();

        ~ViewAxis() noexcept = default;
};
