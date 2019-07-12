#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"


struct Gizmo
{
    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;

    private:
        mc::Transform m_model;
        Vertices m_coordinates;
        VAOPtr m_vao;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ )
               :
               view(view_),
               projection(projection_),
               camera(camera_),
               m_model(),
               m_coordinates{{
                                 mc::Direction::zero(),
                                 mc::Direction::right(),
                                 mc::Direction::zero(),
                                 mc::Direction::up(),
                                 mc::Direction::zero(),
                                 mc::Direction::in()
                            }},
               m_vao()
        {;}

        void initialize()
        {
            m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
            m_vao->bind();
            m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(mc::Position),m_coordinates[0].m_x));
            m_vao->setNumIndices(m_coordinates.size());
            m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
            m_vao->unbind();

//            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
//            prim->createCone("arrow_head",0.2f,0.7f,10,1);
//            prim->createDisk("central",0.5f,4);
        }

        void loadShader()
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(mc::AxisShader);

            auto MVP = projection * view * m_model;

            shader->setUniform("MVP",MVP);
        }

        void draw()
        {
            glClear(GL_DEPTH_BUFFER_BIT);

            auto average_dist = ((view.m_30+view.m_31+view.m_32)/3)*(-0.25f);

            m_model.identity();
                m_model.scale(average_dist,average_dist,average_dist);
                loadShader();
                m_vao->bind();
                m_vao->draw();
                m_vao->unbind();

//            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
//            m_model.identity();
//                m_model.translate(3.f,0.f,0.f);
//                m_model.rotateY(90.f);
//                loadLineColourShader(mc::Colour(1.f,0.f,0.f,1.f));
//                prim->draw("arrow_head");

//            m_model.identity();
//                m_model.translate(0.f,3.f,0.f);
//                m_model.rotateX(-90.f);
//                loadLineColourShader(mc::Colour(0.f,1.f,0.f,1.f));
//                prim->draw("arrow_head");

//            m_model.identity();
//                m_model.translate(0.f,0.f,3.f);
//                loadLineColourShader(mc::Colour(0.f,0.f,1.f,1.f));
//                prim->draw("arrow_head");


//            m_model.identity();
//                auto &&direction = camera.getInvDirection();
//                auto &&shadow = camera.getInvShadow();

//                mc::Rotation Rz;
//                Rz.rotateZ(45.f);

//                auto theta = atan2(direction.m_x,direction.m_z);
//                mc::Rotation Ry = mc::Y_Matrix(-theta);

//                auto rotationAxis = camera.getUp().cross(shadow);
//                rotationAxis.normalize();
//                auto phi = atan2(rotationAxis.dot(direction.cross(shadow)) , direction.dot(shadow));
//                mc::Rotation Rx = mc::Axis_Matrix(phi,rotationAxis);

//                mc::Rotation R = Rx * Ry * Rz;
//                mc::Size<float> S = 1.f;

//                m_model = S * R;
//                m_model.translate(0.f,0.f,0.f);

//                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//                loadLineColourShader(mc::Colour(1.f,1.f,0.f,1.f));
//                prim->draw("central");
//                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        ~Gizmo() noexcept = default;
};



