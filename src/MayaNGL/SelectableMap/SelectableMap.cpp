
#include "MayaNGL/SelectableMap/SelectableMap.h"


void SelectableMap::set_primitive_transform(std::size_t id_, const mc::Transform &transform_)
{
    m_selectables.at(id_).set_transform(transform_);
}

void SelectableMap::append_multi_primitive_transform(const mc::Transform &mous_transform_)
{
    for (auto itr=m_currently_selected.begin(); itr!=m_currently_selected.end()-1; ++itr)
        if (m_selectables.at(*itr).get_is_movable())
            m_selectables.at(*itr).append_transform(mous_transform_);
}

bool SelectableMap::already_selected(std::size_t id_) const
{
    auto elem = std::find(m_currently_selected.cbegin(),m_currently_selected.cend(),id_);
    return (elem != m_currently_selected.cend());
}

bool SelectableMap::id_is_not_found(std::size_t id_) const
{
    return (m_selectables.find(id_) == m_selectables.cend());
}


