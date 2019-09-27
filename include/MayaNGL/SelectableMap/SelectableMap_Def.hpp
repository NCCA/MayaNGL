#pragma once

#include "SelectableMap.h"


template<typename PRIM>
void SelectableMap::insert(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_)
{
    m_selectables.emplace(id_, VariantPrim{std::forward<PRIM>(prim_),transform_,is_moveable_});
}
