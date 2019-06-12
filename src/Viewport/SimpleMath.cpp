
#include "Viewport/SimpleMath.h"


namespace sm
{
    ngl::Vec3 toDegs(const ngl::Vec3 &num_)
    {
        return ngl::Vec3{toDegs(num_.m_x),toDegs(num_.m_y),toDegs(num_.m_z)};
    }

    ngl::Vec3 toRads(const ngl::Vec3 &num_)
    {
        return ngl::Vec3{toRads(num_.m_x),toRads(num_.m_y),toRads(num_.m_z)};
    }

    ngl::Vec3 intersect(const ngl::Vec3 &rayPosition_, const ngl::Vec3 &rayDirection_, const ngl::Vec3 &planePos_)
    {
        auto planePosition = planePos_;
        auto planeNormal = ngl::Vec3::up();

        float t = (planePosition-rayPosition_).dot(planeNormal)/rayDirection_.dot(planeNormal);

        if (t>=0)
        {
            ngl::Vec3 poi = (rayPosition_+(t*rayDirection_)+std::numeric_limits<float>::epsilon());

            //Position top_left = planeCorners[P::Corner::TOP_LEFT];
            //Position top_right = planeCorners[P::Corner::TOP_RIGHT];
            //Position bottom_right = planeCorners[P::Corner::BOTTOM_RIGHT];

            //auto TR_POI = poi-top_right;
            //auto TR_BR = bottom_right-top_right;
            //auto TR_TL = top_left-top_right;

            //if  ( ((Dot(TR_POI,TR_BR) >= 0.0) && (Dot(TR_POI,TR_BR) <= Dot(TR_BR,TR_BR))) &&
            //      ((Dot(TR_POI,TR_TL) >= 0.0) && (Dot(TR_POI,TR_TL) <= Dot(TR_TL,TR_TL))) )
            //{
                return poi;
            //}
            //return ngl::Vec3::zero();
        }
        return ngl::Vec3::zero();
    }

    glm::mat3 X_Matrix(float angle_)
    {
        return glm::mat3{ 1.f, 0.f        , 0.f,
                          0.f, cosf(angle_),-sinf(angle_),
                          0.f, sinf(angle_), cosf(angle_) };
    }

    glm::mat3 Y_Matrix(float angle_)
    {
        return glm::mat3{ cosf(angle_), 0.f, sinf(angle_),
                          0.f        , 1.f, 0.f,
                         -sinf(angle_), 0.f, cosf(angle_) };
    }

    glm::mat3 Z_Matrix(float angle_)
    {
        return glm::mat3{ cosf(angle_),-sinf(angle_), 0.0,
                          sinf(angle_), cosf(angle_), 0.0,
                          0.0        , 0.0        , 1.0 };
    }

    glm::mat3 Axis_Matrix(float angle_, const ngl::Vec3 &axis_)
    {
        return glm::mat3{ cosf(angle_)+powf(axis_.m_x,2)*(1.f-cosf(angle_))                  , (axis_.m_x*axis_.m_y*(1.f-cosf(angle_)))-(axis_.m_z*sinf(angle_)) , (axis_.m_x*axis_.m_z*(1.f-cosf(angle_)))+(axis_.m_y*sinf(angle_)),
                          (axis_.m_y*axis_.m_x*(1.f-cosf(angle_)))+(axis_.m_z*sinf(angle_)) , cosf(angle_)+powf(axis_.m_y,2)*(1.f - cosf(angle_))                , (axis_.m_y*axis_.m_z*(1.f-cosf(angle_)))-(axis_.m_x*sinf(angle_)),
                          (axis_.m_z*axis_.m_x*(1.f-cosf(angle_)))-(axis_.m_y*sinf(angle_)) , (axis_.m_z*axis_.m_y*(1.f-cosf(angle_)))+(axis_.m_x*sinf(angle_))  , cosf(angle_)+powf(axis_.m_z,2)*(1.f - cosf(angle_)) };
    }
}


