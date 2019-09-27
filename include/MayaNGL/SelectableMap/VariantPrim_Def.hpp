#pragma once

#include "VariantPrim.h"


namespace ngl
{
    class Obj;
}


struct VariantPrim::Base
{
    virtual void draw(const mc::Transform &, const mc::View &, const mc::Projection &) const = 0;
    virtual ~Base() noexcept = default;
};

template<typename T, template<typename> class Gen>
struct VariantPrim::BaseGeneric : Base
{
    Gen<T> *crtp = static_cast<Gen<T>*>(this);

    void draw(const mc::Transform &transform_, const mc::View &view_, const mc::Projection &projection_) const override
    {
        ngl::ShaderLib *shader = ngl::ShaderLib::instance();
        shader->use(ngl::nglColourShader);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        auto MVP = projection_ * view_ * transform_;
        shader->setUniform("MVP",MVP);
        shader->setUniform("Colour",ngl::Vec4(0.263f,1.f,0.639f,1.f));

        typedef typename std::remove_reference<T>::type nRType;
        typedef typename std::remove_const<nRType>::type nCType;
        typedef typename mc::remove_smart_ptr<nCType>::type PrimType;

        crtp->dP(std::is_same<PrimType,ngl::Obj>());

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
};

template<typename T>
struct VariantPrim::Generic : BaseGeneric<T,Generic>
{
    T primitive;

    Generic( T &&primitive_ )
             :
             primitive(std::forward<T>(primitive_))
    {;}

    void dP(std::false_type) const
    {
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        prim->draw(primitive);
    }

    void dP(std::true_type) const
    {
        primitive.draw();
    }
};

template<typename T>
struct VariantPrim::Generic<T*> : BaseGeneric<T*,Generic>
{
    const T *const primitive;

    Generic( T *primitive_ )
             :
             primitive(primitive_)
    {;}

    void dP(std::false_type) const
    {
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        prim->draw(primitive);
    }

    void dP(std::true_type) const
    {
        primitive->draw();
    }
};


// ------------------------------------------------------------- //


template<typename T, typename>
VariantPrim::VariantPrim( T *primitive_,
                          const mc::Transform &transform_,
                          bool is_moveable_ )
                          :
                          m_is_moveable(is_moveable_),
                          m_prim_ptr{std::make_unique<Generic<T*>>(primitive_)},
                          m_transform(transform_)
{;}

template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
VariantPrim::VariantPrim( const SmPtr<T> &val_,
                          const mc::Transform &transform_,
                          bool is_moveable_ )
                          :
                          m_is_moveable(is_moveable_),
                          m_prim_ptr{std::make_unique<Generic<T*>>(val_.get())},
                          m_transform(transform_)
{;}

template<typename T, typename>
VariantPrim::VariantPrim( const T &val_,
                          const mc::Transform &transform_,
                          bool is_moveable_ )
                          :
                          m_is_moveable(is_moveable_),
                          m_prim_ptr{std::make_unique<Generic<const T &>>(val_)},
                          m_transform(transform_)
{;}



