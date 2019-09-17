 #pragma once

#include "VariantPrim_Def.hpp"


template<bool visualize_bv_and_ray>
class Base_Selection
{
    private:
        typedef std::unordered_map<std::size_t,VariantPrim> Selectables;
        typedef std::vector<std::size_t> Selections;

    protected:
        const mc::View &view;
        const mc::Projection &projection;
        const mc::LookAt &cam_lookAt;

    protected:
        int m_screen_width;
        int m_screen_height;
        mc::Ray m_ray;
        Selectables m_selectables;
        Selections m_currently_selected;

    public:
        explicit Base_Selection( const mc::View &view_,
                                 const mc::Projection &projection_,
                                 const mc::LookAt &cam_lookAt_ );

        GET_MEMBER(m_selectables,AllSelectables)
        GET_MEMBER(m_currently_selected,CurrentlySelected)
        GET_MEMBER(m_ray,Ray)

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
        typedef std::array<mc::Position,2> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        mc::Position m_ray_end;
        Vertices m_vtxs;
        VAOPtr m_vao;

    public:
        explicit Base_Selection( const mc::View &view_,
                                 const mc::Projection &projection_,
                                 const mc::LookAt &cam_lookAt_ );

        void initialize();
        void emitRay(int mouse_x, int mouse_y);
        void draw() const;

        ~Base_Selection() noexcept = default;
};

