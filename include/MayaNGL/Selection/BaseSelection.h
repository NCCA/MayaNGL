 #pragma once

#include "MayaNGL/SelectableMap/SelectableMap.h"


template<bool visualize_bv_and_ray>
class BaseSelection : public SelectableMap
{
    protected:
        const mc::View &view;
        const mc::Projection &projection;
        const mc::LookAt &cam_lookAt;

    protected:
        mc::Ray m_ray;
        int m_screen_width = 0;
        int m_screen_height = 0;

    public:
        explicit BaseSelection( const mc::View &view_,
                                const mc::Projection &projection_,
                                const mc::LookAt &cam_lookAt_ );

        GET_MEMBER(m_ray,ray)

        void initialize();
        void emit_ray(float mouse_x_, float mouse_y_);
        void draw() const;

        ~BaseSelection() noexcept = default;
};


// ------------------------------------------------------------- //


template<>
class BaseSelection<true> : public BaseSelection<false>
{
    private:
        typedef std::array<mc::Position,2> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        mc::Position m_ray_end;
        Vertices m_vtxs;
        VAOPtr m_vao;

    public:
        explicit BaseSelection( const mc::View &view_,
                                const mc::Projection &projection_,
                                const mc::LookAt &cam_lookAt_ );

        void initialize();
        void emit_ray(int mouse_x_, int mouse_y_);
        void draw() const;

        ~BaseSelection() noexcept = default;
};

