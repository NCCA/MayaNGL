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
 * NGL comments:
 * (1)  All functions that return a copy should be marked as const.
 *      Currently const & objects cannot call them.
 * (2)  Vec::normalize() should have an overload that returns a
 *      copy which can be used in constructors.
 * (3)  Allow ngl::Transformation to be referenced.
 * (4)  Implement an Axis-Based rotation matrix (see below).
 * (5)  Getter functions should not return a copy.
 *      ngl::Transformation::getMatrix() should not return a copy.
 * (6)  Create a function that can update the font size in ngl::Text.
 * (7)  Create a copy constructor in ngl::Transformation that takes
 *      an ngl::Mat4 as a parameter.
 * (8)  get vertex data from VAOPrimitives.
 * (9)  Add a convert Matrix to Euler Rotations function.
 * (10) Access the static id of each ngl::VAOPrimitive that's
 *      created.
 * (11) ngl::Mat4::scale does not work properly.
 * (12) Why isn't ngl::Vec3 * working with ngl::AbstractVAO in the
 *      Base_Selection class.
 * (13) UB when passing an ngl::Transformation as a templated
 *      function argument. This is because the function getMatrix()
 *      returns a copy and not a const&, which means that the local
 *      object returned by getMatrix() gets deleted at the end of
 *      scope.
 * (14) Bug with Mat4::rotateX(90.f) and then Mat4::rotateY(0.f).
 *      RotateY seems to affect the transformation even when it's
 *      set to 0.
 * (15) Cannot overload make_selectable_and_movable() for
 *      ngl::Transform because the .getMatrix() returns a copy
 *      which is deleted. I used a static copy as a hack around it.
 * (16) AbstractMesh::calcBoundingSphere() is not accurate. The
 *      function accumulates all vertex positions and finds the
 *      average position. However, this does not work when the
 *      imported obj is heavily meshed on one side (ex: fish).
 * (17) vec3 * mat3 has a different output than vec3 * mat4.
 * (18) UB for Vec3 that uses an anonymus union-struct and array
 *      to access the members.
 * (19) ngl::Obj cannot be referenced. The unique_ptr deleter
 *      releases the memory when referenced. This is due to the
 *      inherited ngl::AbstractVAO which is designed not to be
 *      referenced once a VAO is created. Decouple ngl::Obj with
 *      the VAO creation.
 * (20) Overload Mat4 setter functions (translation,scale) to
 *      accept ngl::Vec3 objects.
*/


#define GET_MEMBER(n_,N_)                                                               \
    auto get_##N_() const noexcept -> const typename std::decay<decltype(n_)>::type &   \
    {                                                                                   \
        return n_;                                                                      \
    }                                                                                   \

#define enable_visualization true
#define disable_visualization false


namespace ngl
{
    class Obj;
}


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


    static const Position default_eye = {28.f,21.f,28.f};
    static const Position failed = {-ngl::EPSILON,-ngl::EPSILON,-ngl::EPSILON};
    static constexpr float fov = 35.f;
    static constexpr float near_clip = 0.1f;
    static constexpr float far_clip = 1000.f;
    enum class CamView {PERSPECTIVE,FRONT,SIDE,TOP};
    constexpr auto grid_shader = "GridShader";
    constexpr auto axis_shader = "AxisShader";


    struct Ray;
    struct Sphere;
    struct AABB;
    template<bool infinite = true>
    struct Plane;


    template <typename T>
    struct remove_smart_ptr;


    struct LookAt;


    void initialize_additional_shaders();


    template<typename T>
    T to_degs(T num_);

    template<typename T>
    T to_rads(T num_);

    template<typename T>
    T round(T num_, unsigned precision_);

    V3 to_degs(const V3 &num_);
    V3 to_rads(const V3 &num_);
    V3 absl(const V3 &num_);
    V3 round(const V3 &num_, unsigned precision_);

    M4 x_matrix(float angle_);
    M4 y_matrix(float angle_);
    M4 axis_matrix(float angle_, const Direction &axis_);

    template<bool infinite = true>
    Position intersect(const Ray &ray_, const Plane<infinite> &plane_);
    Position intersect(const Ray &ray_, const Sphere &sphere_);
    Position intersect(const Ray &ray_, const Ray &ray2_);


    template<typename T>
    static std::string demangle_typename()
    {
        int status = 0;
        return abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,&status);
    }

}






