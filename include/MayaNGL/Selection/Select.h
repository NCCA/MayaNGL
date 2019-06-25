#pragma once

#include "Viewport/Select/Base_Selection.h"


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
        explicit Select( const LookAt &cam_lookAt_,
                         /*const*/ vc::View &view_,
                         /*const*/ vc::Projection &projection_ );

        template<typename T>
        void make_selectable(std::size_t id_, T &&prim_, const vc::Transform &transform_);

        void resize(int w_, int h_);
        void enableMultiSelection();
        vc::Position clickedOnObject(const SelectablePrim &selectable_) const;
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};



template<typename PRIM>
void make_selectable(std::size_t id_, PRIM &&prim_, const vc::Transform &transform_);

template<typename PRIM, typename T, typename =  std::enable_if_t<
                                                    std::is_same<
                                                        decltype(std::declval<T>().getMatrix()),vc::Transform
                                                                >::value
                                                                >>
void make_selectable(std::size_t id_, PRIM &&prim_, /*const*/ T &transform_);
