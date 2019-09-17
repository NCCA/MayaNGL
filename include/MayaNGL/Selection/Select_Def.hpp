#pragma once

#include "Select.h"


template<bool visualize_bv_and_ray>
Select<visualize_bv_and_ray>::Select( const mc::View &view_,
                                      const mc::Projection &projection_,
                                      const Camera &camera_/*,
                                      Gizmo &gizmo_ */)
                                      :
                                      Base_Selection<visualize_bv_and_ray>(view_,projection_,camera_.getLookAt()),
//                                      m_gizmo(gizmo_),
                                      m_multi_selection(false)
{;}

template<bool visualize_bv_and_ray>
bool Select<visualize_bv_and_ray>::alreadySelected(std::size_t id_) const
{
    auto elem = std::find(this->m_currently_selected.cbegin(),this->m_currently_selected.cend(),id_);
    return (elem != this->m_currently_selected.cend());
}

template<bool visualize_bv_and_ray>
std::size_t Select<visualize_bv_and_ray>::getSelectedId(float &shortest_distance_)
{
    std::size_t selected_id = 0;

    for (const auto &i : this->m_selectables)
    {
        auto &&id = i.first;
        auto &&prim = i.second;

        auto poi = clickedOnObject(prim);
        if (!alreadySelected(id) && (poi != mc::failed))
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
void Select<visualize_bv_and_ray>::make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_)
{
    if (this->m_selectables.find(id_) == this->m_selectables.cend())
        this->m_selectables.emplace(id_, VariantPrim{std::forward<PRIM>(prim_),transform_});
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
mc::Position Select<visualize_bv_and_ray>::clickedOnObject(const VariantPrim &selectable_)
{
    auto &&transform = selectable_.getTransform();

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
    std::size_t selected_id = getSelectedId(shortest_distance);
    bool something_selected = (shortest_distance < maxf);

    if (something_selected)
        this->m_currently_selected.emplace_back(selected_id);

//    if (this->m_currently_selected.empty())
//        m_gizmo.display = false;
//    else
//    {
//        if (something_selected)
//        {
//            auto objpos = this->m_selectables.at(selected_id).getTransform();
//            m_gizmo.setPosition(objpos.m_30,objpos.m_31,objpos.m_32);
//        }
//        m_gizmo.display = true;
//    }

    m_multi_selection = false;
}






