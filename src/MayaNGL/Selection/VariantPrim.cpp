
#include "MayaNGL/Selection/VariantPrim_Def.hpp"


VariantPrim::VariantPrim( std::string &&val_,
                          const mc::Transform &transform_ )
                          :
                          impl{std::make_unique<Generic<>>(std::move(val_),transform_)}
{;}

void VariantPrim::draw(const mc::View &view_, const mc::Projection &projection_) const
{
    impl->draw(view_,projection_);
}
