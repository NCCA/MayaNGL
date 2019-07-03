
#include "MayaNGL/Selection/VariantPrim_Def.hpp"


VariantPrim::VariantPrim( std::string &&val_,
                          const mc::Transform &transform_ )
                          :
                          m_prim_ptr{std::make_unique<Generic<std::string>>(std::move(val_))},
                          m_transform(transform_)
{;}

void VariantPrim::draw(const mc::View &view_, const mc::Projection &projection_) const
{
    m_prim_ptr->draw(m_transform,view_,projection_);
}
