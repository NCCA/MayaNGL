#pragma once

#include "BVs.h"


template<>
class BoundingVolume<mc::Sphere, false>
{
    private:
        template<typename T>
        using DoubleOf = std::array<T,2>;
        typedef std::vector<mc::Position> VertexList;

    private:
        mc::Sphere bv;

    private:
        mc::Position point_on_sphere(const mc::Position &centre_, float radius_, const mc::V2 &coordinates_);
        int getSign(float val);
        mc::V2 calc_spherical_coordinates(const mc::Position &centre_, const mc::Position &point_);
        mc::Position calc_centre_of_sphere(const mc::Position &aabb_centre_, const DoubleOf<mc::Position> &points_, float radius_);
        void calc_tight_bound_volume(const VertexList &vtx_list_);

    public:
        GET_MEMBER(bv,volume)

        template<typename PRIM>
        void compute_volume(const PRIM &prim_);

        template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
        void compute_volume(const SmPtr<T> &prim_);
};


//===================================================================//


template<>
class BoundingVolume<mc::Sphere, true>
{
    private:
        mc::Sphere m_bv;
        mc::Transform m_transform;

    public:
        GET_MEMBER(m_bv,updated_volume)

        void initialize();

        template<typename PRIM>
        void update(const PRIM &prim_);

        template<typename PRIM>
        void draw(const PRIM &prim_, const mc::View &view_, const mc::Projection &projection_);
};




