
#include "MayaNGL/SelectableMap/VariantPrim_Def.hpp"


VariantPrim::VariantPrim( std::string &&val_,
                          const mc::Transform &transform_,
                          bool is_moveable_ )
                          :
                          m_is_moveable(is_moveable_),
                          m_prim_ptr{std::make_unique<Generic<std::string>>(std::move(val_))},
                          m_transform(transform_)
{;}

void VariantPrim::set_transform(const mc::Transform &transform_)
{
    // this is only needed because m_transform is a copy.
    m_transform.m_30 = transform_.m_30;
    m_transform.m_31 = transform_.m_31;
    m_transform.m_32 = transform_.m_32;
}

void VariantPrim::append_transform(const mc::Transform &transform_)
{
    // this is only needed because m_transform is a copy.
    m_transform.m_30 += transform_.m_30;
    m_transform.m_31 += transform_.m_31;
    m_transform.m_32 += transform_.m_32;
}

void VariantPrim::draw(const mc::View &view_, const mc::Projection &projection_) const
{
    m_prim_ptr->draw(m_transform,view_,projection_);
}
