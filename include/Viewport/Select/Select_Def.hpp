#pragma once

#include "Viewport/Select/Select.h"


template<bool visualize_bv_and_ray>
Select<visualize_bv_and_ray>::Select( const LookAt &cam_lookAt_,
                                      const vc::View &view_,
                                      /*const*/ vc::Projection &projection_ )
                                      :
                                      Base_Selection<visualize_bv_and_ray>(cam_lookAt_,view_,projection_)
{;}

template<bool visualize_bv_and_ray>
    template<typename S>
void Select<visualize_bv_and_ray>::make_selectable(std::size_t id_, S &&prim_name_, const vc::Transform &prim_transform_)
{
    this->m_selectables.emplace(id_,SelectablePrim{std::forward<S>(prim_name_),prim_transform_});
}

template<bool visualize_bv_and_ray>
bool Select<visualize_bv_and_ray>::clickedOnObject(const SelectablePrim &selectable_) const
{
    auto &&transform = selectable_.transform;

    auto pos = vc::Position{transform.m_30,transform.m_31,transform.m_32};
    float rad = std::max({transform.m_00,transform.m_11,transform.m_22});
    vc::Sphere bv {pos,rad};

    auto poi = vc::intersect(this->m_ray,bv);
    if (poi != vc::failed)
        return true;
    return false;
}

template<bool visualize_bv_and_ray>
void Select<visualize_bv_and_ray>::pick(int mouse_x, int mouse_y)
{
    if (this->m_selectables.empty())
        return;

    /* THIS NEEDS SORTING OUT */

    this->emitRay(mouse_x,mouse_y);

    bool nothing_selected = true;
    for (auto &&i : this->m_selectables)
    {
        auto &&prim = i.second;
        if (this->clickedOnObject(prim))
        {
            auto &&id = i.first;
            this->m_current_selections.emplace_back(id);
            nothing_selected = false;
            break;
        }
    }

    if (nothing_selected)
        this->m_current_selections.clear();


    std::cout<<this->m_current_selections.size()<<std::endl;
    std::cout<<this->m_selectables.size()<<std::endl;

//    auto elem = std::find_if(this->m_selectables.cbegin(), this->m_selectables.cend(), [&id_](const SelectablePrim &sp)
//                                                                                       {
//                                                                                            return (id_ == sp.id);
//                                                                                       });
//    if (elem == this->m_selectables.cend())
//        this->m_selectables.emplace_back(id_,std::forward<S>(prim_name_),prim_transform_);
}
