#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"


class Gizmo
{
    private:
        enum class Handle {NONE,X,Y,Z,MID};

    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;

    private:
        mc::Transform *m_object_model;
        mc::Position m_position;
        mc::Size<float> m_average_dist;
        mc::Size<float> m_uniform_scale;
        mc::Rotation m_orientation;
        mc::Transform m_model;
        Vertices m_coordinates;
        VAOPtr m_vao;
        mc::V2 mouse_move;
        Camera::LocalHVAxis m_hv_axis;
        bool m_display = false;
        Handle m_handle_selected = Handle::NONE;

    private:
        template<Handle H>
        bool clickedOn(const mc::Ray &mouse_ray_);
        template<Handle H>
        void dragOn();

        float calc_length(float p_);
        void loadShader() const;
        void loadShader(mc::Colour &&colour_) const;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        GET_MEMBER(m_object_model,ObjectTransform)

        void initialize();
        void setPosition(float x_, float y_, float z_);
        void make_moveable(mc::Transform &transform_);
        void show();
        void hide();
        bool is_enabled();
        bool is_selected();
        void deselect();
        void findSelectedHandle(const mc::Ray &mouse_ray_);
        void drag_on_axis(const mc::V2 &mouse_drag_);
        void draw();

        ~Gizmo() noexcept;
};


