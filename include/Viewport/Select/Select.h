#pragma once

#include "Viewport/Select/Base_Selection.h"



template<bool visualize_bv_and_ray = false>
struct Select : Base_Selection<visualize_bv_and_ray>
{
    private:
        typedef typename Base_Selection<visualize_bv_and_ray>::SelectablePrim SelectablePrim;

    public:
        explicit Select( const LookAt &cam_lookAt_,
                         const vc::View &view_,
                         /*const*/ vc::Projection &projection_ );

        template<typename S>
        void make_selectable(std::size_t id_, S &&prim_name_, const vc::Transform &prim_transform_);

        bool clickedOnObject(const SelectablePrim &selectable_) const;
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};
