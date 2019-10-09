#pragma once

#include "MayaNGL/Common/Common.h"


namespace mc //maya common
{

    struct Ray
    {
        Position position;
        Direction direction = Direction::in();
    };

    struct Sphere
    {
        Position centre;
        float radius = 1.f;
    };

    struct AABB
    {
        enum {left,right,bottom,top,back,front,num_of_sides};
        typedef std::array<mc::Position,num_of_sides> Dimensions;

        Position centre;
        Dimensions end_points;

        mc::Position min_average(mc::Position &orig_, const mc::Position &vtx_, std::size_t axis_)
        {
            mc::Position min = orig_;

            if (min[axis_] > vtx_[axis_])
                min = vtx_;
            else if (std::abs(min[axis_]-vtx_[axis_]) < std::numeric_limits<float>::epsilon())
                min = (min + vtx_) * 0.5f;

            return min;
        }

        mc::Position max_average(mc::Position &orig_, const mc::Position &vtx_, std::size_t axis_)
        {
            mc::Position max = orig_;

            if (max[axis_] < vtx_[axis_])
                max = vtx_;
            else if (std::abs(max[axis_]-vtx_[axis_]) < std::numeric_limits<float>::epsilon())
                max = (max + vtx_) * 0.5f;

            return max;
        }

        template<typename T>
        void calc_end_points(const T &vtx_list_)
        {
            for (auto itr=std::cbegin(vtx_list_); itr!=std::cend(vtx_list_); ++itr)
            {
                auto &&vtx = *itr;

                end_points[left]   = min_average(end_points[left],vtx,0);
                end_points[right]  = max_average(end_points[right],vtx,0);
                end_points[bottom] = min_average(end_points[bottom],vtx,1);
                end_points[top]    = max_average(end_points[top],vtx,1);
                end_points[back]   = min_average(end_points[back],vtx,2);
                end_points[front]  = max_average(end_points[front],vtx,2);
            }
        }

        void calc_centre()
        {
            centre.m_x = (end_points[left].m_x + end_points[right].m_x) * 0.5f;
            centre.m_y = (end_points[bottom].m_y + end_points[top].m_y) * 0.5f;
            centre.m_z = (end_points[back].m_z + end_points[front].m_z) * 0.5f;
        }
    };

    template<bool infinite>
    struct Plane
    {
        Position centre;
        Direction normal;
    };

    template<>
    struct Plane<false> : Plane<true>
    {
        enum CRNR {TL,TR,BL,BR};
        typedef std::array<Position,4> Points;

        Size<V2> size;
        Rotation orientation;
        Points corners;

        Plane( const Position &position_,
               const Direction &normal_,
               const Size<V2> &size_,
               const Rotation &orientation_ )
               :
               Plane<true>{position_,normal_},
               size(size_),
               orientation(orientation_),
               corners(init_corners())
        {;}

        Points init_corners()
        {
            Points tmp;

            tmp[TL] = this->centre + Position{-size.m_x, size.m_y, 0.f} * orientation;
            tmp[TR] = this->centre + Position{ size.m_x, size.m_y, 0.f} * orientation;
            tmp[BL] = this->centre + Position{-size.m_x,-size.m_y, 0.f} * orientation;
            tmp[BR] = this->centre + Position{ size.m_x,-size.m_y, 0.f} * orientation;

            return tmp;
        }

        ~Plane() noexcept = default;
    };


    template <typename T>
    struct remove_smart_ptr
    {
        typedef T type;
    };

    template <typename T>
    struct remove_smart_ptr<T*>
    {
       typedef T type;
    };

    template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
    struct remove_smart_ptr<SmPtr<T>>
    {
       typedef T type;
    };


    struct LookAt
    {
        Position eye = default_eye;
        Position target = Position::zero();
        Direction up = Direction::up();
        Direction front = glm::normalize((target-eye).toGLM());

        float calc_dist() const;
        Direction calc_direction() const;
    };


    template<typename T>
    T to_degs(T num_)
    {
        return static_cast<T>(num_*(180.f/M_PI));
    }

    template<typename T>
    T to_rads(T num_)
    {
        return static_cast<T>(num_*(M_PI/180.f));
    }

    template<typename T>
    T round(T num_, unsigned precision_)
    {
        return floor((num_ * pow(10.f,precision_)) + 0.5f) / pow(10.f,precision_);
    }

}
