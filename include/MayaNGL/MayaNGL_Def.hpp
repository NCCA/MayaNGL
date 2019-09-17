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
    this->make_selectable(id_,std::forward<PRIM>(prim_), transform_.getMatrix());
}

template<typename PRIM>
void MayaNGL::make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_)
{
    m_select.make_selectable(id_,std::forward<PRIM>(prim_),transform_);
}

template<typename PRIM, typename T, typename>
void MayaNGL::make_selectable_and_moveable(std::size_t id_, PRIM &&prim_, T &transform_)
{
    this->make_selectable_and_moveable(id_,std::forward<PRIM>(prim_), transform_.getMatrix());
}

template<typename PRIM>
void MayaNGL::make_selectable_and_moveable(std::size_t id_, PRIM &&prim_, mc::Transform &transform_)
{
    m_select.make_selectable(id_,std::forward<PRIM>(prim_),transform_);
    m_gizmo.make_moveable(transform_);

//    if (m_select.getCurrentlySelected().empty())
//        m_gizmo.hide();
//    else
//        m_gizmo.show();
}

