#pragma once

#include "Base_Selection.h"
#include "MayaNGL/Gizmo/Gizmo.h"


template<bool visualize_bv_and_ray = false>
class Select final : public Base_Selection<visualize_bv_and_ray>
{
    private:
        Gizmo &m_gizmo;
        bool m_multi_selection;

    private:
        bool alreadySelected(std::size_t id_) const;
        std::size_t getSelectedId(float &shortest_distance_);

    public:
        explicit Select( const mc::View &view_,
                         const mc::Projection &projection_,
                         const Camera &camera_,
                         Gizmo &gizmo_ );

        template<typename PRIM>
        void make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_);

        void resize(int w_, int h_);
        void enableMultiSelection();
        mc::Position clickedOnObject(const VariantPrim &selectable_);
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};
