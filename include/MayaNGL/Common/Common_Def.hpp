#pragma once

#include "MayaNGL/Common/Common.h"


namespace mc //maya common
{

    struct Ray
    {
        Position position;
        Direction direction;
    };

    struct Sphere
    {
        Position position;
        float radius;
    };

    template<bool infinite>
    struct Plane
    {
        Position position;
        Direction normal;
    };

    template<>
    struct Plane<false> : Plane<true>
    {
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
               corners(initCorners())
        {;}

        Points initCorners()
        {
            Points tmp = {{
                            {this->position.m_x-size.m_x,this->position.m_y+size.m_y,this->position.m_z}, // TL
                            {this->position.m_x+size.m_x,this->position.m_y+size.m_y,this->position.m_z}, // TR
                            {this->position.m_x-size.m_x,this->position.m_y-size.m_y,this->position.m_z}, // BL
                            {this->position.m_x+size.m_x,this->position.m_y-size.m_y,this->position.m_z}  // BR
                         }};

            for (auto &&i : tmp)
                i = i * orientation;

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

        float calcDist() const;
        Direction calcDirection() const;
    };


    template<typename T>
    T toDegs(T num_)
    {
        return static_cast<T>(num_*(180.f/M_PI));
    }

    template<typename T>
    T toRads(T num_)
    {
        return static_cast<T>(num_*(M_PI/180.f));
    }

    template<typename T>
    T round(T num_, unsigned precision_)
    {
        return floor((num_ * pow(10.f,precision_)) + 0.5f) / pow(10.f,precision_);
    }

}
