#pragma once

#include "Select.h"


template<bool visualization>
    template<typename CAM>
Select<visualization>::Select( const mc::View &view_,
                               const mc::Projection &projection_,
                               const CAM &camera_ )
                               :
                               BaseSelection<visualization>(view_,projection_,camera_.get_lookAt()),
                               m_multi_selection(false)
{;}

template<bool visualization>
int Select<visualization>::get_selected_id(float &shortest_distance_)
{
    int selected_id = -1;

    for (const auto &i : this->m_selectables)
    {
        auto &&id = i.first;
        auto &&prim = i.second;

        auto poi = clicked_on_object(prim);
        if (!this->already_selected(id) && (poi != mc::failed))
        {
            auto &&camera_eye = this->cam_lookAt.eye;

            auto dist = (poi-camera_eye).lengthSquared();
            if (shortest_distance_ > dist)
            {
                shortest_distance_ = dist;
                selected_id = id;
            }
        }
    }

    return selected_id;
}

template<bool visualization>
    template<typename PRIM>
void Select<visualization>::make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_)
{
    this->insert(id_,std::forward<PRIM>(prim_),transform_,is_moveable_);
}

template<bool visualization>
void Select<visualization>::resize(int w_, int h_)
{
    this->m_screen_width = w_;
    this->m_screen_height = h_;
}

template<bool visualization>
void Select<visualization>::enable_multi_selection()
{
    m_multi_selection = true;
}

template<bool visualization>
mc::Position Select<visualization>::clicked_on_object(const VariantPrim &selectable_)
{
    this->m_viz_bv.update(selectable_);
    auto poi = mc::intersect(this->m_ray,this->m_viz_bv.get_updated_volume());
    return poi;
}

template<bool visualization>
int Select<visualization>::pick()
{
    if (!m_multi_selection)
        this->m_currently_selected.clear();

    auto &&maxf = std::numeric_limits<float>::max();
    float shortest_distance = maxf;
    int selected_id = get_selected_id(shortest_distance);
    bool something_selected = (shortest_distance < maxf);

    if (something_selected)
        this->m_currently_selected.emplace_back(selected_id);

    m_multi_selection = false;

    return selected_id;
}
