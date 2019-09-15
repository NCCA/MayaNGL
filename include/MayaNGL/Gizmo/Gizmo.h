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
        mutable mc::Transform m_model;
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
        void draw() const;

        mc::Position clickedOnHandle(const mc::Ray &mouse_)
        {
//            auto &&transform = selectable_.getTransform();

//            auto pos = mc::Position{transform.m_30,transform.m_31,transform.m_32};
//            float rad = std::max({transform.m_00,transform.m_11,transform.m_22});
//            mc::Sphere bv {pos,rad};

//            auto poi = mc::intersect(this->m_ray,bv);
//            return poi;
            return mc::Position::zero();
        }

        ~Gizmo() noexcept = default;
};



