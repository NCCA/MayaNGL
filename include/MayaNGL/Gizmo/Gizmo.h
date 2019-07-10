#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"


struct Gizmo
{
    private:
        typedef std::array<mc::Position,2> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;

    private:
        mc::Transform m_model;
        VAOPtr m_vao;
        mc::View m_view;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ )
               :
               view(view_),
               projection(projection_),
               camera(camera_),
               m_model(),
               m_vao()
        {;}

        void initialize()
        {
            m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);

            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
            prim->createCone("arrow_head",0.2f,0.7f,10,1);
            prim->createDisk("central",0.5f,4);
        }

        void loadLineColourShader(mc::Colour &&colour_)
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(ngl::nglColourShader);

            auto MVP = projection * view * m_model;

            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",std::move(colour_));
        }

        void drawLine(mc::Direction &&dir_)
        {
            Vertices vtxs = {{ mc::Direction::zero(), std::move(dir_) }};
            m_vao->bind();
            m_vao->setData(ngl::AbstractVAO::VertexData(vtxs.size()*sizeof(mc::Position),vtxs[0].m_x));
            m_vao->setNumIndices(vtxs.size());
            m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
            m_vao->draw();
            m_vao->unbind();
        }


        void draw()
        {
            glClear(GL_DEPTH_BUFFER_BIT);

            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

            m_model.identity();
                loadLineColourShader(mc::Colour(1.f,0.f,0.f,1.f));
                drawLine(mc::Direction::right()*3.f);
                loadLineColourShader(mc::Colour(0.f,1.f,0.f,1.f));
                drawLine(mc::Direction::up()*3.f);
                loadLineColourShader(mc::Colour(0.f,0.f,1.f,1.f));
                drawLine(mc::Direction::in()*3.f);

            m_model.identity();
                m_model.translate(3.f,0.f,0.f);
                m_model.rotateY(90.f);
                loadLineColourShader(mc::Colour(1.f,0.f,0.f,1.f));
                prim->draw("arrow_head");

            m_model.identity();
                m_model.translate(0.f,3.f,0.f);
                m_model.rotateX(-90.f);
                loadLineColourShader(mc::Colour(0.f,1.f,0.f,1.f));
                prim->draw("arrow_head");

            m_model.identity();
                m_model.translate(0.f,0.f,3.f);
                loadLineColourShader(mc::Colour(0.f,0.f,1.f,1.f));
                prim->draw("arrow_head");


            m_model.identity();
                auto &&direction = camera.getInvDirection();
                auto &&shadow = camera.getInvShadow();

                mc::Rotation Rz;
                Rz.rotateZ(45.f);

                auto theta = atan2(direction.m_x,direction.m_z);
                mc::Rotation Ry = mc::Y_Matrix(-theta);

                auto rotationAxis = camera.getUp().cross(shadow);
                rotationAxis.normalize();
                auto phi = atan2(rotationAxis.dot(direction.cross(shadow)) , direction.dot(shadow));
                mc::Rotation Rx = mc::Axis_Matrix(phi,rotationAxis);

                m_model = Rx * Ry * Rz;

                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                loadLineColourShader(mc::Colour(1.f,1.f,0.f,1.f));
                prim->draw("central");
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


        }

        ~Gizmo() noexcept = default;
};



