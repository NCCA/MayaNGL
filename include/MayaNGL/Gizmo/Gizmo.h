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
        mc::Transform *object_model;

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
        bool handle_selected;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        void initialize();
        void setPosition(float x_, float y_, float z_);
        void draw();

        void show()
        {
            display = true;
        }

        void hide()
        {
            display = false;
        }

        void make_moveable(mc::Transform &transform_)
        {
            object_model = &transform_;
            setPosition(object_model->m_30,object_model->m_31,object_model->m_32);
        }

        bool clickedOnHandle(const mc::Ray &mouse_)
        {
            float handle_length = position.m_y+average_dist+(uniform_scale*0.7f);
            mc::Ray up_handle{position,ngl::Vec3::up()};
            auto poi = mc::intersect(mouse_,up_handle);
            if ((poi != mc::failed) && (poi.m_y < handle_length))
                return true;
            return false;
        }

        void dragX(float x_)
        {
            position.m_x += x_;
        }
        void dragY(float y_)
        {
            position.m_y -= y_*0.08f;
            object_model->m_31 = position.m_y;
        }
        void dragZ(float z_)
        {
            position.m_z += z_;
        }

        ~Gizmo() noexcept = default;
};



