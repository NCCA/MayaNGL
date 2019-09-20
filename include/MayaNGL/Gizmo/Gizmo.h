#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"
#include "MayaNGL/SelectableMap/SelectableMap.h"


class Gizmo
{
    private:
        enum class Handle {NONE,X,Y,Z,MID};

    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;
        typedef SelectableMap::Selectables<mc::Transform*> Transforms;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;

    private:
        Transforms m_object_models;
        mc::Transform *m_currently_selected_model;
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
        bool clicked_on(const mc::Ray &mouse_ray_);
        template<Handle H>
        void dragged_on();

        float calc_length(float p_);
        void load_shader() const;
        void load_shader(mc::Colour &&colour_) const;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        GET_MEMBER(m_currently_selected_model,currently_selected_model)

        void initialize();
        void set_on_selected_id(int id_);
        void make_movable(std::size_t id_, mc::Transform &transform_);
        void show();
        void hide();
        bool is_enabled();
        bool is_selected();
        void deselect();
        void find_selected_handle(const mc::Ray &mouse_ray_);
        void dragged_on_axis(const mc::V2 &mouse_drag_);
        void draw();

        ~Gizmo() noexcept = default;
};


