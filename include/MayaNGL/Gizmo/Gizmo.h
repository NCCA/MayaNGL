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
        mc::Position position;
        mc::Size<float> average_dist;
        mc::Size<float> uniform_scale;
        mc::Transform m_model;
        Vertices m_coordinates;
        VAOPtr m_vao;

    private:
        void loadShader() const;
        void loadShader(mc::Colour &&colour_) const;

    public:
        bool display;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        void initialize();
        void setPosition(float x_, float y_, float z_);
        void draw();

        bool clickedOnHandle(const mc::Ray &mouse_)
        {
            mc::Ray up_handle{position,ngl::Vec3::up(),average_dist+(uniform_scale*0.7f)};
            auto poi = mc::intersect(mouse_,up_handle);
            if (poi != mc::failed)
                return true;
            return false;
        }

        ~Gizmo() noexcept = default;
};



