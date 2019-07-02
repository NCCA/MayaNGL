#pragma once

#include "MayaNGL/Common/Common.h"


class VariantPrim
{
    private:
        struct Base
        {
            virtual void draw(const mc::Transform &, const mc::View &, const mc::Projection &) const = 0;
            virtual ~Base() noexcept = default;
        };

        template<typename T>
        struct Generic : Base
        {
            T primitive;

            Generic( T &&primitive_ );

            void dP(std::false_type) const
            {
                std::cout<< "false_type" <<std::endl;
                ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
                prim->draw(primitive);
            }

            void dP(std::true_type) const
            {
                std::cout<< "true_type" <<std::endl;
                primitive.draw();
            }

            /// https://rextester.com/GOF94946

            void draw(const mc::Transform &transform_, const mc::View &view_, const mc::Projection &projection_) const override;
        };

    private:
        std::unique_ptr<Base> m_prim_ptr = nullptr;
        mc::Transform m_transform;

    public:
        template<typename T, typename = std::enable_if_t<!std::is_trivially_constructible<T>::value>>
        VariantPrim( T *val_,
                     const mc::Transform &transform_ );

        template<typename T>
        VariantPrim( T &&val_,
                     const mc::Transform &transform_ );

        GET_MEMBER(m_transform,Transform)

        void draw(const mc::View &view_, const mc::Projection &projection_) const;
};

