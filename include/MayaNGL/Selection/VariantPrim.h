#pragma once

#include "MayaNGL/Common/Common.h"


class VariantPrim
{
    private:
        struct Base;

        template<typename T, template<typename> class Gen>
        struct Base_Generic;

        template<typename T>
        struct Generic;

    private:
        std::unique_ptr<Base> m_prim_ptr = nullptr;
        mc::Transform m_transform;

    private:
        template<typename T>
        using sfinae_type = std::enable_if_t<!std::is_trivially_constructible<T>::value>;

    public:
        template<typename T, typename = sfinae_type<T>>
        VariantPrim( T *primitive_,
                     const mc::Transform &transform_ );

        template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
        VariantPrim( const SmPtr<T> &val_,
                     const mc::Transform &transform_ );

        template<typename T, typename = sfinae_type<T>>
        VariantPrim( const T &val_,
                     const mc::Transform &transform_ );

        VariantPrim( std::string &&val_,
                     const mc::Transform &transform_ );

        GET_MEMBER(m_transform,Transform)

        void draw(const mc::View &view_, const mc::Projection &projection_) const;
};

