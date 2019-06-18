#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
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


/*
 * Recomendations for NGL:
 * (1) All functions that return a copy should be marked as const.
 *     Currently const & objects cannot call them.
 * (2) Vec::normalize() should have an overload that returns a
 *     copy. This can be used in constructors.
 * (3) Make model matrix and/or ngl::Transformation a singleton.
 *     I **think** that maya has only on model
 *     matrix in the graphics side. The user is able to modify the
 *     attributes of each object from the software side.
 * (4) Implement an Axis-Based rotation matrix (see below).
 * (5) ngl::Transformation::getMatrix() should not return a copy.
 * (6) Create a function that can update the font size in ngl::Text.
*/


#define GET_MEMBER(n_,N_)                                                               \
    auto get##N_() const noexcept -> const typename std::decay<decltype(n_)>::type &    \
    {                                                                                   \
        return n_;                                                                      \
    }                                                                                   \


namespace vc //viewport common
{

    using Position = ngl::Vec3;
    using Direction = ngl::Vec3;
    using Translation = ngl::Mat4;
    using Rotation = ngl::Mat4;
    using View = ngl::Mat4;
    using Projection = ngl::Mat4;
    using Transform = ngl::Mat4;

    static constexpr float fov = 35.f;
    static constexpr float near_clip = 0.1f;
    static constexpr float far_clip = 200.f;

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

    ngl::Vec3 toDegs(const ngl::Vec3 &num_);
    ngl::Vec3 toRads(const ngl::Vec3 &num_);
    ngl::Vec3 absl(const ngl::Vec3 &num_);
    ngl::Vec3 round(const ngl::Vec3 &num_, unsigned precision_);

    Position intersect(const Position &ray_position_, const Direction &ray_direction_, const Position &plane_position_, const Direction &plane_normal_);

    glm::mat4 Y_Matrix(float angle_);
    glm::mat4 Axis_Matrix(float angle_, const Direction &axis_);

}
