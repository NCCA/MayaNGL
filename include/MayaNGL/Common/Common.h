#pragma once

#include <glm/glm.hpp>
#include <ngl/Vec2.h>
#include <ngl/Vec3.h>
#include <ngl/Mat3.h>
#include <ngl/Mat4.h>
#include <ngl/VAOFactory.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Text.h>
#include <ngl/NGLStream.h>
#include <math.h>
#include <cxxabi.h>


/*
 * About NGL:
 * (1)  All functions that return a copy should be marked as const.
 *      Currently const & objects cannot call them.
 * (2)  Vec::normalize() should have an overload that returns a
 *      copy. This can be used in constructors.
 * (3)  Make model matrix and/or ngl::Transformation a singleton.
 *      I **think** that maya has only on model
 *      matrix in the graphics side. The user is able to modify the
 *      attributes of each object from the software side.
 * (4)  Implement an Axis-Based rotation matrix (see below).
 * (5)  ngl::Transformation::getMatrix() should not return a copy.
 * (6)  Create a function that can update the font size in ngl::Text.
 * (7)  Create a copy constructor in ngl::Transformation that takes
 *      an ngl::Mat4 as a parameter.
 * (8)  get vertex data from VAOPrimitives.
 * (9)  Convert Matrix to Euler Rotations.
 * (10) Access the static id of each ngl::VAOPrimitive that's created.
 * (11) ngl::Mat4::scale does not work properly.
 * (12) Why isn't ngl::Vec3 * working with ngl::AbstractVAO in the
 *      Base_Selection class.
 * (13) UB when passing an ngl::Transformation as a templated function
 *      argument. This is because the function getMatrix() returns a
 *      copy and not a const&, which means that the local object returned
 *      by getMatrix() gets deleted at the end of scope.
 * (14) Bug with Mat4::rotateX(90.f) and then Mat4::rotateY(0.f). RotateY
 *      seems to affect the transformation even when it's set to 0.
*/


#define GET_MEMBER(n_,N_)                                                               \
    auto get##N_() const noexcept -> const typename std::decay<decltype(n_)>::type &    \
    {                                                                                   \
        return n_;                                                                      \
    }                                                                                   \


namespace mc //maya common
{

    using V2 = ngl::Vec2;
    using V3 = ngl::Vec3;
    using V4 = ngl::Vec4;
    using M4 = ngl::Mat4;

    using Position = V3;
    using Direction = V3;
    using Colour = V4;
    using Translation = M4;
    using Rotation = M4;
    using Model = M4;
    using View = M4;
    using Projection = M4;
    using Transform = M4;

    template<typename T = V3>
    using Size = T;

    template<typename T>
    using Generic = T;



    static const Position default_eye = {28.f,21.f,28.f};
    static const Position failed = {-ngl::EPSILON,-ngl::EPSILON,-ngl::EPSILON};
    static constexpr float fov = 35.f;
    static constexpr float near_clip = 0.1f;
    static constexpr float far_clip = 200.f;
    enum class CamView {PERSPECTIVE,FRONT,SIDE,TOP};



    struct Ray
    {
        Position position;
        Direction direction;
    };

    template<bool infinite = true>
    struct Plane
    {
        Position position;
        Direction normal;
    };

    template<>
    struct Plane<false> : Plane<true>
    {
        Size<V2> size;
    };

    struct Sphere
    {
        Position position;
        float radius;
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
    struct remove_smart_ptr< SmPtr<T> >
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

    Generic<V3> toDegs(const Generic<V3> &num_);
    Generic<V3> toRads(const Generic<V3> &num_);
    Generic<V3> absl(const Generic<V3> &num_);
    Generic<V3> round(const Generic<V3> &num_, unsigned precision_);

    Generic<M4> Y_Matrix(float angle_);
    Generic<M4> Axis_Matrix(float angle_, const Direction &axis_);

    template<bool infinite = true>
    Position intersect(const Ray &ray_, const Plane<infinite> &plane_);
    Position intersect(const Ray &ray_, const Sphere &sphere_);

    template<typename T>
    static std::string demangle_typename()
    {
        int status = 0;
        return abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,&status);
    }

}
