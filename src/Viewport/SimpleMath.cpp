
#include "Viewport/SimpleMath.h"


namespace sm
{
    float toDegs(float num_)
    {
        return num_*(180.f/static_cast<float>(M_PI));
    }

    float toRads(float num_)
    {
        return num_*(static_cast<float>(M_PI)/180.f);
    }

    glm::mat3 X_Matrix(float angle_)
    {
        return glm::mat3{ 1.f, 0.f        , 0.f,
                          0.f, cos(angle_),-sin(angle_),
                          0.f, sin(angle_), cos(angle_) };
    }

    glm::mat3 Y_Matrix(float angle_)
    {
        return glm::mat3{ cos(angle_), 0.f, sin(angle_),
                          0.f        , 1.f, 0.f,
                         -sin(angle_), 0.f, cos(angle_) };
    }

    glm::mat3 Z_Matrix(float angle_)
    {
        return glm::mat3{ cos(angle_),-sin(angle_), 0.0,
                          sin(angle_), cos(angle_), 0.0,
                          0.0        , 0.0        , 1.0 };
    }

    glm::mat3 Axis_Matrix(float angle_, const ngl::Vec3 &axis_)
    {
        return glm::mat3{ cos(angle_)+pow(axis_.m_x,2)*(1.f-cos(angle_))                  , (axis_.m_x*axis_.m_y*(1.f-cos(angle_)))-(axis_.m_z*sin(angle_)) , (axis_.m_x*axis_.m_z*(1.f-cos(angle_)))+(axis_.m_y*sin(angle_)),
                          (axis_.m_y*axis_.m_x*(1.f-cos(angle_)))+(axis_.m_z*sin(angle_)) , cos(angle_)+pow(axis_.m_y,2)*(1.f - cos(angle_))                , (axis_.m_y*axis_.m_z*(1.f-cos(angle_)))-(axis_.m_x*sin(angle_)),
                          (axis_.m_z*axis_.m_x*(1.f-cos(angle_)))-(axis_.m_y*sin(angle_)) , (axis_.m_z*axis_.m_y*(1.f-cos(angle_)))+(axis_.m_x*sin(angle_))   , cos(angle_)+pow(axis_.m_z,2)*(1.f - cos(angle_)) };
    }
}


