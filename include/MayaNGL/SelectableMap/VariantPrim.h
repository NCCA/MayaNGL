#pragma once

#include "MayaNGL/Common/Common_Def.hpp"
#include "MayaNGL/BoundingVolumes/BoundingVolumes.h"


class VariantPrim
{
    private:
        struct Base;

        template<typename T, template<typename> class Gen>
        struct BaseGeneric;

        template<typename T>
        struct Generic;

    private:
        bool m_is_moveable;
        std::unique_ptr<Base> m_prim_ptr = nullptr;
        // This needs to be a copy because Transformation::getMatrix() returns a copy.
        mc::Transform m_transform;

    public:
        BoundingVolume<mc::Sphere,false> bv;

    private:
        template<typename T>
        using sfinae_type = std::enable_if_t<!std::is_trivially_constructible<T>::value>;

    public:
        VariantPrim() = default;

        template<typename T, typename = sfinae_type<T>>
        VariantPrim( T *primitive_,
                     const mc::Transform &transform_,
                     bool is_moveable_ = false);

        template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
        VariantPrim( const SmPtr<T> &primitive_,
                     const mc::Transform &transform_,
                     bool is_moveable_ = false );

        template<typename T, typename = sfinae_type<T>>
        VariantPrim( const T &primitive_,
                     const mc::Transform &transform_,
                     bool is_moveable_ = false );

        VariantPrim( std::string &&primitive_,
                     const mc::Transform &transform_,
                     bool is_moveable_ = false );

        GET_MEMBER(m_is_moveable,is_movable)
        GET_MEMBER(m_transform,transform)

        void set_transform(const mc::Transform &transform_);
        void append_transform(const mc::Transform &transform_);
        void draw(const mc::View &view_, const mc::Projection &projection_) const;
};

