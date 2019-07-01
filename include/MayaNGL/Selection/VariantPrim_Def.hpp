#pragma once

#include "VariantPrim.h"


template<typename T>
VariantPrim::Generic<T>::Generic( T &&primitive_ )
                                  :
                                  primitive(std::forward<T>(primitive_))
{;}

template<typename T>
void VariantPrim::Generic<T>::draw(const mc::Transform &transform_, const mc::View &view_, const mc::Projection &projection_) const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    auto MVP = projection_ * view_ * transform_;
    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour",ngl::Vec4(0.263f,1.f,0.639f,1.f));

    std::cout<< mc::demangle_typename<T>() <<std::endl;

    typedef typename std::remove_reference<std::remove_const<T>::type>::type Type;

    dP(std::is_same<Type,ngl::Obj>());

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

template<>
void VariantPrim::Generic<ngl::Obj>::draw(const mc::Transform &transform_, const mc::View &view_, const mc::Projection &projection_) const;

template<typename T, typename>
VariantPrim::VariantPrim( T *const val_,
                          const mc::Transform &transform_ )
                          :
                          m_prim_ptr{std::make_unique<Generic<const T *>>(val_)},
                          m_transform(transform_)
{;}

template<typename T>
VariantPrim::VariantPrim( T &&val_,
                          const mc::Transform &transform_ )
                          :
                          m_prim_ptr{std::make_unique<Generic<T>>(std::forward<T>(val_))},
                          m_transform(transform_)
{;}

