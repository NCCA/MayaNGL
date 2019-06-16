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

    ngl::Vec3 toDegs(const ngl::Vec3 &num_);
    ngl::Vec3 toRads(const ngl::Vec3 &num_);
    ngl::Vec3 absl(const ngl::Vec3 &num_);

    Position intersect(const Position &ray_position_, const Direction &ray_direction_, const Position &plane_position_, const Direction &plane_normal_);

    glm::mat4 X_Matrix(float angle_);
    glm::mat4 Y_Matrix(float angle_);
    glm::mat4 Z_Matrix(float angle_);
    glm::mat4 Axis_Matrix(float angle_, const Direction &axis_);

}
