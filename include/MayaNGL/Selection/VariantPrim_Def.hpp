#pragma once

#include "VariantPrim.h"


template<typename T>
VariantPrim::Generic<T>::Generic( T &&primitive_,
                                  const mc::Transform &transform_ )
                                  :
                                  primitive(std::forward<T>(primitive_)),
                                  transform(transform_)
{;}

template<typename T>
void VariantPrim::Generic<T>::draw(const mc::View &view_, const mc::Projection &projection_) const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    auto MVP = projection_ * view_ * transform;
    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour",ngl::Vec4(0.263f,1.f,0.639f,1.f));
    prim->draw(primitive);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

template<typename T, typename>
VariantPrim::VariantPrim( T *const val_,
                          const mc::Transform &transform_ )
                          :
                          impl{std::make_unique<Generic<const T *>>(val_,transform_)}
{;}

template<typename T>
VariantPrim::VariantPrim( T &val_,
                          const mc::Transform &transform_ )
                          :
                          impl{std::make_unique<Generic<const T &>>(val_,transform_)}
{;}

template<typename T>
VariantPrim::VariantPrim( T &&val_,
                          const mc::Transform &transform_ )
                          :
                          impl{std::make_unique<Generic<T>>(std::forward<T>(val_),transform_)}
{;}

