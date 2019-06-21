#pragma once

#include "Viewport/LookAt/LookAt.h"


template<bool visualize_bv_and_ray>
class Base_Selection
{
    protected:
        struct SelectablePrim
        {
            std::string name;
            vc::Transform transform;
        };

    private:
        typedef std::unordered_map<std::size_t,SelectablePrim> Selectables;
        typedef std::vector<std::size_t> Selections;

    protected:
        const LookAt &cam_lookAt;
        /*const*/ vc::View &view;
        /*const*/ vc::Projection &projection;

    protected:
        int m_screen_width;
        int m_screen_height;
        vc::Ray m_ray;
        Selectables m_selectables;
        Selections m_currently_selected;

    public:
        explicit Base_Selection( const LookAt &cam_lookAt_,
                                 /*const*/ vc::View &view_,
                                 /*const*/ vc::Projection &projection_ );

        GET_MEMBER(m_selectables,AllSelectables)
        GET_MEMBER(m_currently_selected,CurrentlySelected)

        void initialize();
        void emitRay(int mouse_x, int mouse_y);
        void draw() const;

        ~Base_Selection() noexcept = default;
};


// ------------------------------------------------------------- //


template<>
class Base_Selection<true> : public Base_Selection<false>
{
    private:
        typedef std::array<vc::Position,2> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        vc::Position m_ray_end;
        Vertices m_vtxs;
        VAOPtr m_vao;

    public:
        explicit Base_Selection( const LookAt &cam_lookAt_,
                                 /*const*/ vc::View &view_,
                                 /*const*/ vc::Projection &projection_ );

        void initialize();
        void emitRay(int mouse_x, int mouse_y);
        void draw() const;

        ~Base_Selection() noexcept = default;
};

