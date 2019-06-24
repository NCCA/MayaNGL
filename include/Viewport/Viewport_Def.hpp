
#pragma once

#include "Viewport/Viewport.h"


template<typename T>
void Viewport::initialize(const T &eye_, const T &target_, const T &up_)
{
    m_initial_lookAt = {eye_, target_, up_};
    m_camera.reset(m_initial_lookAt);
    this->initialize();
}

template< typename T, std::size_t N, template<typename,std::size_t> class CNT >
void Viewport::initialize(const CNT<T,N> &lookAt_)
{
    this->initialize(lookAt_[0],lookAt_[1],lookAt_[2]);
}

template< typename T, template<typename,typename = std::allocator<T>> class CNT >
void Viewport::initialize(const CNT<T> &lookAt_)
{
    this->initialize(lookAt_[0],lookAt_[1],lookAt_[2]);
}

template<typename PRIM>
void Viewport::make_selectable(std::size_t id_, PRIM &&prim_, const vc::Transform &transform_)
{
    m_select.make_selectable(id_,std::forward<PRIM>(prim_),transform_);
}

template<typename PRIM, typename T, typename>
void Viewport::make_selectable(std::size_t id_, PRIM &&prim_, /*const*/ T &transform_)
{
    make_selectable(id_,std::forward<PRIM>(prim_),transform_.getMatrix());
}

















