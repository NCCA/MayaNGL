#pragma once

#include <glm/glm.hpp>
#include <ngl/Vec3.h>


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

    float toRads(float num_);

    ngl::Vec3 intersect(const ngl::Vec3 &rayPosition_, const ngl::Vec3 &rayDirection_, const ngl::Vec3 &planePos_);

    glm::mat3 X_Matrix(float angle_);
    glm::mat3 Y_Matrix(float angle_);
    glm::mat3 Z_Matrix(float angle_);
    glm::mat3 Axis_Matrix(float angle_, const ngl::Vec3 &axis_);
}
