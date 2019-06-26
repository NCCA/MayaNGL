#pragma once

#include "Base_Selection.h"


template<bool visualize_bv_and_ray = false>
struct Select final : Base_Selection<visualize_bv_and_ray>
{
    private:
        typedef typename Base_Selection<visualize_bv_and_ray>::SelectablePrim SelectablePrim;

    private:
        bool m_multi_selection;

    private:
        bool alreadySelected(std::size_t id_) const;
        std::size_t getSelectedId(float &shortest_distance_) const;

    public:
        template<typename C>
        explicit Select( /*const*/ mc::View &view_,
                         /*const*/ mc::Projection &projection_,
                         const C &camera_ );

        template<typename T>
        void make_selectable(std::size_t id_, T &&prim_, const mc::Transform &transform_);

        void resize(int w_, int h_);
        void enableMultiSelection();
        mc::Position clickedOnObject(const SelectablePrim &selectable_) const;
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};
