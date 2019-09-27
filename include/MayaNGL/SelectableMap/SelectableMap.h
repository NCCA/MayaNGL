#pragma once

#include "VariantPrim_Def.hpp"


struct SelectableMap
{
    template<typename T = class VariantPrim>
    using Selectables = std::unordered_map<std::size_t,T>;
    typedef std::vector<std::size_t> Selections;

    protected:
        Selectables<> m_selectables;
        Selections m_currently_selected;

    public:
        GET_MEMBER(m_selectables,all_selectables)
        GET_MEMBER(m_currently_selected,currently_selected)

        void set_primitive_transform(std::size_t id_, const mc::Transform &transform_);
        void append_multi_primitive_transform(const mc::Transform &mous_transform_);
        bool already_selected(std::size_t id_) const;
        bool id_is_not_found(std::size_t id_) const;

        template<typename PRIM>
        void insert(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_, bool is_moveable_);
};


