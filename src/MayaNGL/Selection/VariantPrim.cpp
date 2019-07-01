
#include "MayaNGL/Selection/VariantPrim_Def.hpp"


void VariantPrim::draw(const mc::View &view_, const mc::Projection &projection_) const
{
    m_prim_ptr->draw(m_transform,view_,projection_);
}
