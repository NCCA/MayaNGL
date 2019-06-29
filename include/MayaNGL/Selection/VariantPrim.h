#pragma once

#include "MayaNGL/Common/Common.h"


class VariantPrim
{
    private:
        struct Base
        {
            virtual void draw(const mc::View &, const mc::Projection &) const = 0;
            virtual ~Base() noexcept = default;
        };

        template<typename T = std::string>
        struct Generic : Base
        {
            const T primitive;
            const mc::Transform &transform;

            Generic( T &&primitive_,
                     const mc::Transform &transform_ );

            void draw(const mc::View &view_, const mc::Projection &projection_) const override;
        };

    private:
        std::unique_ptr<Base> impl = nullptr;

    public:
        template<typename T, typename = std::enable_if_t<!std::is_trivially_constructible<T>::value>>
        VariantPrim( T *const val_,
                     const mc::Transform &transform_ );

        template<typename T>
        VariantPrim( T &val_,
                     const mc::Transform &transform_ );

        template<typename T>
        VariantPrim( T &&val_,
                     const mc::Transform &transform_ );

        VariantPrim( std::string &&val_,
                     const mc::Transform &transform_ );

        void draw(const mc::View &view_, const mc::Projection &projection_) const;
};

