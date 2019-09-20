#pragma once

#include "BaseSelection.h"


template<bool visualize_bv_and_ray = false>
class Select final : public BaseSelection<visualize_bv_and_ray>
{
    private:
        bool m_multi_selection;

    private:
        std::size_t get_selected_id(float &shortest_distance_);

    public:
        template<typename CAM>
        explicit Select( const mc::View &view_,
                         const mc::Projection &projection_,
                         const CAM &camera_ );

        template<typename PRIM>
        void make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_ = false);

        void resize(int w_, int h_);
        void enable_multi_selection();
        mc::Position clicked_on_object(const VariantPrim &selectable_);
        void pick();

        ~Select() noexcept = default;
};
