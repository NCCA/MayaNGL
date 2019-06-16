
#include "Viewport/Common.h"


namespace vc
{

    ngl::Vec3 toDegs(const ngl::Vec3 &num_)
    {
        return ngl::Vec3{toDegs(num_.m_x),toDegs(num_.m_y),toDegs(num_.m_z)};
    }

    ngl::Vec3 toRads(const ngl::Vec3 &num_)
    {
        return ngl::Vec3{toRads(num_.m_x),toRads(num_.m_y),toRads(num_.m_z)};
    }

    ngl::Vec3 absl(const ngl::Vec3 &num_)
    {
        return ngl::Vec3{fabs(num_.m_x),fabs(num_.m_y),fabs(num_.m_z)};
    }

    Position intersect(const Position &ray_position_, const Direction &ray_direction_, const Position &plane_position_, const Direction &plane_normal_)
    {
        float t = (plane_position_-ray_position_).dot(plane_normal_)/ray_direction_.dot(plane_normal_);

        if (t>=0)
            return ray_position_+(t*ray_direction_) + ngl::EPSILON;

        return Position::zero();
    }

    glm::mat4 Y_Matrix(float angle_)
    {
        return glm::mat4{ cosf(angle_), 0.f, sinf(angle_), 0.f,
                          0.f         , 1.f, 0.f         , 0.f,
                         -sinf(angle_), 0.f, cosf(angle_), 0.f,
                          0.f         , 0.f, 0.f         , 1.f };
    }

    glm::mat4 Axis_Matrix(float angle_, const Direction &axis_)
    {
        return glm::mat4{ cosf(angle_)+powf(axis_.m_x,2)*(1.f-cosf(angle_))                 , (axis_.m_x*axis_.m_y*(1.f-cosf(angle_)))-(axis_.m_z*sinf(angle_)) , (axis_.m_x*axis_.m_z*(1.f-cosf(angle_)))+(axis_.m_y*sinf(angle_)), 0.f,
                          (axis_.m_y*axis_.m_x*(1.f-cosf(angle_)))+(axis_.m_z*sinf(angle_)) , cosf(angle_)+powf(axis_.m_y,2)*(1.f - cosf(angle_))               , (axis_.m_y*axis_.m_z*(1.f-cosf(angle_)))-(axis_.m_x*sinf(angle_)), 0.f,
                          (axis_.m_z*axis_.m_x*(1.f-cosf(angle_)))-(axis_.m_y*sinf(angle_)) , (axis_.m_z*axis_.m_y*(1.f-cosf(angle_)))+(axis_.m_x*sinf(angle_)) , cosf(angle_)+powf(axis_.m_z,2)*(1.f - cosf(angle_))              , 0.f,
                          0.f                                                               , 0.f                                                               , 0.f                                                              , 1.f };
    }

}


