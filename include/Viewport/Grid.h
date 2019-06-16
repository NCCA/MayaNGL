#pragma once

#include "Viewport/Common.h"


class Grid
{
    private:
        const vc::View &view;
        const vc::Projection &projection;

    private:
        vc::Transform m_model;

    private:
        void loadLineColourShader() const;

    public:
        explicit Grid( const vc::View &view_,
                       const vc::Projection &projection_ );

        void initialize();
        void reset();
        void front();
        void side();
        void draw() const;

        ~Grid() noexcept = default;
};

