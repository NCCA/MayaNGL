#pragma once

#include "Viewport/Select/Select.h"


template<bool visualize_bv_and_ray>
Select<visualize_bv_and_ray>::Select( const LookAt &cam_lookAt_,
                                      /*const*/ vc::View &view_,
                                      /*const*/ vc::Projection &projection_ )
                                      :
                                      Base_Selection<visualize_bv_and_ray>(cam_lookAt_,view_,projection_),
                                      m_multi_selection(false)
{;}

template<bool visualize_bv_and_ray>
bool Select<visualize_bv_and_ray>::alreadySelected(std::size_t id_) const
{
    auto elem = std::find(this->m_currently_selected.cbegin(),this->m_currently_selected.cend(),id_);
    return (elem != this->m_currently_selected.cend());
}

template<bool visualize_bv_and_ray>
std::size_t Select<visualize_bv_and_ray>::getSelectedId(float &shortest_distance_) const
{
    std::size_t selected_id = 0;

    for (auto &&i : this->m_selectables)
    {
        auto &&id = i.first;
        auto &&prim = i.second;

        auto poi = this->clickedOnObject(prim);
        if (!alreadySelected(id) && (poi != vc::failed))
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
    template<typename T>
void Select<visualize_bv_and_ray>::make_selectable(std::size_t id_, T &&prim_, const vc::Transform &transform_)
{
//    this->m_selectables.emplace(id_,SelectablePrim{std::move(prim_),transform_});
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::resize(int w_, int h_)
{
    this->m_screen_width = w_;
    this->m_screen_height = h_;
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::enableMultiSelection()
{
    m_multi_selection = true;
}

template<bool visualize_bv_and_ray>
vc::Position Select<visualize_bv_and_ray>::clickedOnObject(const SelectablePrim &selectable_) const
{
    auto &&transform = selectable_.transform;

    auto pos = vc::Position{transform.m_30,transform.m_31,transform.m_32};
    float rad = std::max({transform.m_00,transform.m_11,transform.m_22});
    vc::Sphere bv {pos,rad};

    auto poi = vc::intersect(this->m_ray,bv);
    return poi;
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::pick(int mouse_x, int mouse_y)
{
    if (this->m_selectables.empty())
        return;

    if (!m_multi_selection)
        this->m_currently_selected.clear();

    this->emitRay(mouse_x,mouse_y);

    auto &&maxf = std::numeric_limits<float>::max();
    float shortest_distance = maxf;
    std::size_t selected_id = getSelectedId(shortest_distance);

    if (!alreadySelected(selected_id) && (shortest_distance != maxf))
        this->m_currently_selected.emplace_back(selected_id);

    m_multi_selection = false;
}





































