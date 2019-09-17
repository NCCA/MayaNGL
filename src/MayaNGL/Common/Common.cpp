
#include "MayaNGL/Common/Common.h"


namespace mc
{

    V3 toDegs(const V3 &num_)
    {
        return V3{toDegs(num_.m_x),toDegs(num_.m_y),toDegs(num_.m_z)};
    }

    V3 toRads(const V3 &num_)
    {
        return V3{toRads(num_.m_x),toRads(num_.m_y),toRads(num_.m_z)};
    }

    V3 absl(const V3 &num_)
    {
        return V3{fabs(num_.m_x),fabs(num_.m_y),fabs(num_.m_z)};
    }

    V3 round(const V3 &num_, unsigned precision_)
    {
        return V3{round(num_.m_x,precision_),round(num_.m_y,precision_),round(num_.m_z,precision_)};
    }

    M4 X_Matrix(float angle_)
    {
        return glm::mat4{ 1.f, 0.f        , 0.f        , 0.f,
                          0.f, cos(angle_),-sin(angle_), 0.f,
                          0.f, sin(angle_), cos(angle_), 0.f,
                          0.f, 0.f        , 0.f        , 1.f };
    }

    M4 Y_Matrix(float angle_)
    {
        return glm::mat4{ cosf(angle_), 0.f, sinf(angle_), 0.f,
                          0.f         , 1.f, 0.f         , 0.f,
                         -sinf(angle_), 0.f, cosf(angle_), 0.f,
                          0.f         , 0.f, 0.f         , 1.f };
    }

    M4 Axis_Matrix(float angle_, const Direction &axis_)
    {
        return glm::mat4{ cosf(angle_)+powf(axis_.m_x,2)*(1.f-cosf(angle_))                 , (axis_.m_x*axis_.m_y*(1.f-cosf(angle_)))-(axis_.m_z*sinf(angle_)) , (axis_.m_x*axis_.m_z*(1.f-cosf(angle_)))+(axis_.m_y*sinf(angle_)), 0.f,
                          (axis_.m_y*axis_.m_x*(1.f-cosf(angle_)))+(axis_.m_z*sinf(angle_)) , cosf(angle_)+powf(axis_.m_y,2)*(1.f - cosf(angle_))               , (axis_.m_y*axis_.m_z*(1.f-cosf(angle_)))-(axis_.m_x*sinf(angle_)), 0.f,
                          (axis_.m_z*axis_.m_x*(1.f-cosf(angle_)))-(axis_.m_y*sinf(angle_)) , (axis_.m_z*axis_.m_y*(1.f-cosf(angle_)))+(axis_.m_x*sinf(angle_)) , cosf(angle_)+powf(axis_.m_z,2)*(1.f - cosf(angle_))              , 0.f,
                          0.f                                                               , 0.f                                                               , 0.f                                                              , 1.f };
    }


    float LookAt::calcDist() const
    {
        return (target-eye).length();
    }

    Direction LookAt::calcDirection() const
    {
        auto dir = target-eye;
        dir.normalize();
        return dir;
    }

    void initializeAdditionalShaders()
    {
        ngl::ShaderLib *shader = ngl::ShaderLib::instance();

        shader->createShaderProgram( GridShader );
        shader->attachShader( "GridShader_Vertex", ngl::ShaderType::VERTEX );
        shader->attachShader( "GridShader_Geometry", ngl::ShaderType::GEOMETRY );
        shader->attachShader( "GridShader_Fragment", ngl::ShaderType::FRAGMENT );
        shader->loadShaderSource( "GridShader_Vertex", "shaders/MayaNGL/GridShader_Vertex.glsl" );
        shader->loadShaderSource( "GridShader_Geometry", "shaders/MayaNGL/GridShader_Geometry.glsl" );
        shader->loadShaderSource( "GridShader_Fragment", "shaders/MayaNGL/GridShader_Fragment.glsl" );
        shader->compileShader( "GridShader_Vertex" );
        shader->compileShader( "GridShader_Geometry" );
        shader->compileShader( "GridShader_Fragment" );
        shader->attachShaderToProgram( GridShader, "GridShader_Vertex" );
        shader->attachShaderToProgram( GridShader, "GridShader_Geometry" );
        shader->attachShaderToProgram( GridShader, "GridShader_Fragment" );
        shader->linkProgramObject( GridShader );

        shader->createShaderProgram( AxisShader );
        shader->attachShader( "AxisShader_Vertex", ngl::ShaderType::VERTEX );
        shader->attachShader( "AxisShader_Geometry", ngl::ShaderType::GEOMETRY );
        shader->attachShader( "AxisShader_Fragment", ngl::ShaderType::FRAGMENT );
        shader->loadShaderSource( "AxisShader_Vertex", "shaders/MayaNGL/AxisShader_Vertex.glsl" );
        shader->loadShaderSource( "AxisShader_Geometry", "shaders/MayaNGL/AxisShader_Geometry.glsl" );
        shader->loadShaderSource( "AxisShader_Fragment", "shaders/MayaNGL/AxisShader_Fragment.glsl" );
        shader->compileShader( "AxisShader_Vertex" );
        shader->compileShader( "AxisShader_Geometry" );
        shader->compileShader( "AxisShader_Fragment" );
        shader->attachShaderToProgram( AxisShader, "AxisShader_Vertex" );
        shader->attachShaderToProgram( AxisShader, "AxisShader_Geometry" );
        shader->attachShaderToProgram( AxisShader, "AxisShader_Fragment" );
        shader->linkProgramObject( AxisShader );
    }



    template<>
    Position intersect(const Ray &ray_, const Plane<true> &plane_)
    {
        auto &&r_pos = ray_.position;
        auto &&r_dir = ray_.direction;
        auto &&p_pos = plane_.position;
        auto &&p_nrm = plane_.normal;

        float t = (p_pos-r_pos).dot(p_nrm)/r_dir.dot(p_nrm);
        if (t>=0)
            return r_pos+(t*r_dir);
        return failed;
    }

    template<>
    Position intersect(const Ray &ray_, const Plane<false> &plane_)
    {
        auto &&p_pos = plane_.position;
        auto &&p_size = plane_.size;

        auto poi = intersect<true>(ray_, plane_);
        if (poi != failed)
        {
            Position top_left = {p_pos.m_x-p_size.m_x,p_pos.m_y+p_size.m_y,p_pos.m_z};
            Position top_right = {p_pos.m_x+p_size.m_x,p_pos.m_y+p_size.m_y,p_pos.m_z};
            Position bottom_right = {p_pos.m_x+p_size.m_x,p_pos.m_y-p_size.m_y,p_pos.m_z};

            auto TR_POI = poi-top_right;
            auto TR_BR = bottom_right-top_right;
            auto TR_TL = top_left-top_right;

            if  ( ( (TR_POI.dot(TR_BR) >= 0.f) && (TR_POI.dot(TR_BR) <= TR_BR.dot(TR_BR)) ) &&
                  ( (TR_POI.dot(TR_TL) >= 0.f) && (TR_POI.dot(TR_TL) <= TR_TL.dot(TR_TL)) ) )
            {
                return poi;
            }
            return failed;
        }
        return failed;
    }

    Position intersect(const Ray &ray_, const Sphere &sphere_)
    {
        auto &&r_pos = ray_.position;
        auto &&r_dir = ray_.direction;
        auto &&s_pos = sphere_.position;
        auto &&s_rad = sphere_.radius;

        auto a = r_pos - s_pos;
        float b = a.dot(r_dir);
        float c = a.dot(a) - powf(s_rad,2);

        float discr = powf(b,2) - c;
        if (discr > 0.f)
        {
            auto t = -b - sqrt(discr);  // front side
            if (t < 0.f)  // check if inside the sphere
                t = -b + sqrt(discr);  // back side
            return r_pos+(t*r_dir);
        }
        return failed;
    }

    Position intersect(const Ray &ray_, const Ray &ray2_)
    {
        auto &&r1_pos = ray_.position;
        auto &&r1_dir = ray_.direction;
        auto &&r2_pos = ray2_.position;
        auto &&r2_dir = ray2_.direction;

        auto t = ((r1_pos-r2_pos).cross(r1_dir)).length() / (r2_dir.cross(r1_dir)).length();
        auto s = ((r2_pos-r1_pos).cross(r2_dir)).length() / (r1_dir.cross(r2_dir)).length();

        if (t < far_clip && s < far_clip)
        {
            Position reql = r2_pos+(t*r2_dir);
            Position leql = r1_pos+(s*r1_dir);
            float dist = (reql-leql).length();

            if (dist<=0.25f)
                return reql;
        }
        return failed;
    }

}


