#pragma once

#include "VariantPrim_Def.hpp"


struct SelectableMap
{
    template<typename T = class VariantPrim>
    using Selectables = std::unordered_map<std::size_t,T>;
    typedef std::vector<std::size_t> Selections;

    Selectables<> m_selectables;
    Selections m_currently_selected;

    GET_MEMBER(m_selectables,all_selectables)
    GET_MEMBER(m_currently_selected,currently_selected)

    void set_primitive_transform(std::size_t id_, const mc::Transform &transform_)
    {
        m_selectables.at(id_).set_transform(transform_);
    }

    void append_multi_primitive_transform(const mc::Transform &mous_transform_)
    {
        for (auto itr=m_currently_selected.begin(); itr!=m_currently_selected.end()-1; ++itr)
            if (m_selectables.at(*itr).get_is_movable())
                m_selectables.at(*itr).append_transform(mous_transform_);
    }

    bool already_selected(std::size_t id_) const
    {
        auto elem = std::find(m_currently_selected.cbegin(),m_currently_selected.cend(),id_);
        return (elem != m_currently_selected.cend());
    }

    bool id_is_not_found(std::size_t id_) const
    {
        return (m_selectables.find(id_) == m_selectables.cend());
    }

    template<typename PRIM>
    void insert(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_)
    {
        m_selectables.emplace(id_, VariantPrim{std::forward<PRIM>(prim_),transform_,is_moveable_});
    }
};

