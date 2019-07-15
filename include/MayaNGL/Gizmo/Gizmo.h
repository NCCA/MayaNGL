#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"


class Gizmo
{
    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;

    private:
        mutable mc::Transform m_model;
        Vertices m_coordinates;
        VAOPtr m_vao;

    private:
        void loadShader() const;
        void loadShader(mc::Colour &&colour_) const;

    public:
        bool display;
        mc::Position position;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        void initialize();
        void draw() const;

        ~Gizmo() noexcept = default;
};



