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
std::size_t Select<visualize_bv_and_ray>::get_selected_id(float &shortest_distance_)
{
    std::size_t selected_id = 0;

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
    if (this->id_is_not_found(id_))
        this->insert(id_,prim_,transform_,is_moveable_);
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

    auto pos = mc::Position{transform.m_30,transform.m_31,transform.m_32};
    float rad = std::max({transform.m_00,transform.m_11,transform.m_22});
    mc::Sphere bv {pos,rad};

    auto poi = mc::intersect(this->m_ray,bv);
    return poi;
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::pick()
{
    if (this->m_selectables.empty())
        return;

    if (!m_multi_selection)
        this->m_currently_selected.clear();

    auto &&maxf = std::numeric_limits<float>::max();
    float shortest_distance = maxf;
    std::size_t selected_id = get_selected_id(shortest_distance);
    bool something_selected = (shortest_distance < maxf);

    if (something_selected)
        this->m_currently_selected.emplace_back(selected_id);

    m_multi_selection = false;
}
