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
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Text.h>
#include <ngl/NGLStream.h>
#include <math.h>


#define GET_MEMBER(n_,N_)                                                               \
    auto get##N_() const noexcept -> const typename std::decay<decltype(n_)>::type &    \
    {                                                                                   \
        return n_;                                                                      \
    }                                                                                   \


namespace sm
{
    template<typename T>
    T toDegs(T num_)
    {
        return static_cast<T>(num_*(180.0/M_PI));
    }

    template<typename T>
    T toRads(T num_)
    {
        return static_cast<T>(num_*(M_PI/180.0));
    }

    template<typename T>
    float inverseCosAngle(const T &v1_,const T &v2_)
    {
        if ((v1_-v2_).length() <= 0.000009)
            return 0.0;

        float c = v1_.dot(v2_)/(v1_.length()*v2_.length());
        return (c>=1.0) ? acos(1.0) : acos(c);
    }

    ngl::Vec3 toDegs(const ngl::Vec3 &num_);
    ngl::Vec3 toRads(const ngl::Vec3 &num_);
    ngl::Vec3 absl(const ngl::Vec3 &num_);
    ngl::Vec3 intersect(const ngl::Vec3 &rayPosition_, const ngl::Vec3 &rayDirection_, const ngl::Vec3 &planePos_);

    glm::mat4 X_Matrix(float angle_);
    glm::mat4 Y_Matrix(float angle_);
    glm::mat4 Z_Matrix(float angle_);
    glm::mat4 Axis_Matrix(float angle_, const ngl::Vec3 &axis_);
}
