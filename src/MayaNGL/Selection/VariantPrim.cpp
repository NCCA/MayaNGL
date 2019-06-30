
#include "MayaNGL/Selection/VariantPrim_Def.hpp"


VariantPrim::VariantPrim( std::string &&val_,
                          const mc::Transform &transform_ )
                          :
                          transform(transform_),
                          m_prim_ptr{std::make_unique<Generic<>>(std::move(val_))}
{;}

void VariantPrim::draw(const mc::Transform &transform_, const mc::View &view_, const mc::Projection &projection_) const
{
    m_prim_ptr->draw(view_,projection_);
}
