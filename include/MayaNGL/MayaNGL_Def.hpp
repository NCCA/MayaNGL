#pragma once

#include "MayaNGL.h"


template<typename T, std::size_t N, template<typename,std::size_t> class CNT >
void MayaNGL::initialize(const CNT<T,N> &lookAt_)
{
    this->initialize(lookAt_[0],lookAt_[1],lookAt_[2]);
}

template< typename T, template<typename,typename = std::allocator<T>> class CNT >
void MayaNGL::initialize(const CNT<T> &lookAt_)
{
    this->initialize(lookAt_[0],lookAt_[1],lookAt_[2]);
}

template<typename T>
void MayaNGL::initialize(const T &eye_, const T &target_, const T &up_)
{
    m_initial_lookAt = {eye_, target_, up_};
    m_camera.reset(m_initial_lookAt);
    this->initialize();
}

template<typename PRIM, typename T, typename>
void MayaNGL::make_selectable(std::size_t id_, PRIM &&prim_, T &transform_)
{
    this->make_selectable(id_,std::forward<PRIM>(prim_),transform_.getMatrix());
}

template<typename PRIM>
void MayaNGL::make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_)
{
    if (m_select.id_is_not_found(id_))
        m_select.make_selectable(id_,std::forward<PRIM>(prim_),transform_);
}

template<typename PRIM, typename T, typename>
void MayaNGL::make_selectable_and_movable(std::size_t id_, PRIM &&prim_, T &transform_)
{
    if (m_select.id_is_not_found(id_))
    {
        static auto t = transform_.getMatrix(); // this is a hack to keep a copy of the getMatrix().
        this->make_selectable_and_movable(id_,std::forward<PRIM>(prim_),t);
    }
    if (m_select.already_selected(id_))
        std::cerr<< "MayaNGL::ERROR >> Cannot use ngl::Transformation for moving!" <<std::endl;
}

template<typename PRIM>
void MayaNGL::make_selectable_and_movable(std::size_t id_, PRIM &&prim_, mc::Transform &transform_)
{
    if (m_select.id_is_not_found(id_))
    {
        m_gizmo.make_movable(id_,transform_);
        m_select.make_selectable(id_,std::forward<PRIM>(prim_),transform_,true);
    }
}

