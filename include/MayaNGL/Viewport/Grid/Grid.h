#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


class Grid
{
    private:
        const mc::View &view;
        const mc::Projection &projection;
        const mc::LookAt &lookAt;

    private:
        mc::Transform m_model;

    private:
        void loadShader() const;

    public:
        template<typename C>
        explicit Grid( const mc::View &view_,
                       const mc::Projection &projection_,
                       const C &camera_ );

        template<mc::CamView>
        void viewOrientation();

        void initialize();
        void draw() const;

        ~Grid() noexcept = default;
};

