#pragma once

#include "Select.h"


template<bool visualize_bv_and_ray>
    template<typename CAM>
Select<visualize_bv_and_ray>::Select( const mc::View &view_,
                                      const mc::Projection &projection_,
                                      const CAM &camera_ )
                                      :
                                      BaseSelection<visualize_bv_and_ray>(view_,projection_,camera_.get_lookAt()),
                                      m_multi_selection(false)
{;}

template<bool visualize_bv_and_ray>
int Select<visualize_bv_and_ray>::get_selected_id(float &shortest_distance_)
{
    int selected_id = -1;

    for (const auto &i : this->m_selectables)
    {
        auto &&id = i.first;
        auto &&prim = i.second;

        auto poi = clicked_on_object(prim);
        if (!this->already_selected(id) && (poi != mc::failed))
        {
            auto dist = (poi-this->cam_lookAt.eye).lengthSquared();
            if (shortest_distance_ > dist)
            {
                shortest_distance_ = dist;
                selected_id = id;
            }
        }
    }

    return selected_id;
}

template<bool visualize_bv_and_ray>
    template<typename PRIM>
void Select<visualize_bv_and_ray>::make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_)
{
    this->insert(id_,std::forward<PRIM>(prim_),transform_,is_moveable_);
    this->m_bv.compute_volume(std::forward<PRIM>(prim_));
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::resize(int w_, int h_)
{
    this->m_screen_width = w_;
    this->m_screen_height = h_;
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::enable_multi_selection()
{
    m_multi_selection = true;
}

template<bool visualize_bv_and_ray>
mc::Position Select<visualize_bv_and_ray>::clicked_on_object(const VariantPrim &selectable_)
{
    auto &&transform = selectable_.get_transform();
    this->m_bv.update(transform);
    auto poi = mc::intersect(this->m_ray,this->m_bv.get_volume());
    return poi;
}

template<bool visualize_bv_and_ray>
int Select<visualize_bv_and_ray>::pick()
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
