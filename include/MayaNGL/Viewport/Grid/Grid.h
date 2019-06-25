#pragma once

#include "MayaNGL/Viewport/LookAt/LookAt.h"


class Grid
{
    private:
        const vc::View &view;
        const vc::Projection &projection;
        const LookAt &camera;

    private:
        vc::Transform m_model;

    private:
        void loadLineColourShader() const;

    public:
        template<typename CAM>
        explicit Grid( const vc::View &view_,
                       const vc::Projection &projection_,
                       const CAM &camera_ );

        void initialize();
        void reset();
        void front();
        void side();
        void draw() const;

        ~Grid() noexcept = default;
};

