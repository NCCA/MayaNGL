#pragma once

#include "Viewport/LookAt.h"


struct Select
{
    const LookAt &cam_lookAt;
    const vc::View &view;
    /*const*/ vc::Projection &projection;

    struct Ray
    {
        vc::Position start;
        vc::Direction direction;
        vc::Position end;
    };
    Ray m_ray;

    std::vector<vc::Position> select_dir; // why can't ngl::Vec3* work?
    std::unique_ptr<ngl::AbstractVAO> sdvao;

    explicit Select( const LookAt &cam_lookAt_,
                     const vc::View &view_,
                     /*const*/ vc::Projection &projection_ )
                     :
                     cam_lookAt(cam_lookAt_),
                     view(view_),
                     projection(projection_),
                     m_ray{cam_lookAt.eye,vc::Direction::zero(),cam_lookAt.eye},
                     select_dir{{m_ray.start,m_ray.end}}
    {;}

    void initialize()
    {
        sdvao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
    }

    void draw() const
    {
        ngl::ShaderLib *shader = ngl::ShaderLib::instance();
        shader->use(ngl::nglColourShader);

        auto MV = projection * view;

        shader->setUniform("MVP",MV);
        shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f));

        sdvao->bind();
        sdvao->setData(ngl::AbstractVAO::VertexData(select_dir.size()*sizeof(ngl::Vec3),select_dir[0].m_x));
        sdvao->setNumIndices(select_dir.size());
        sdvao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
        sdvao->draw();
        sdvao->unbind();
    }

    void emitRay(int mouse_x, int mouse_y)
    {
        std::cout<<std::flush;

        /// from here: http://antongerdelan.net/opengl/raycasting.html
        /// Pipeline: Local Space (Vertex) -> Model Matrix (World Space) -> View Matrix (Eye Space) ->
        ///           Projection Matrix (Clip Space) -> Perspective Division (Normalized Device Space) ->
        ///           Viewport Transform (Screen Space).

        int screen_width = 1024;
        int screen_height = 640;

        // get mouse position on Screen Space.
        std::cout<< "mouseX = " << mouse_x <<std::endl;
        std::cout<< "mouseX = " << mouse_y <<std::endl;

        // convert mouse position to Normalized Device Space.
        float normMouseX = (2.f*mouse_x)/screen_width - 1.f;
        float normMouseY = 1.f - (2.f*mouse_y)/screen_height;
        std::cout<< "normMouseX = " << normMouseX <<std::endl;
        std::cout<< "normMouseY = " << normMouseY <<std::endl;

        // create vector on Clip Space using -1 on the z-depth.
        ngl::Vec4 clip_coordinates(normMouseX,normMouseY,-1.f,1.f);
        std::cout<< "clip_coordinates = " << clip_coordinates <<std::endl;

        // convert the clip coordinates to Eye Space.

        ngl::Vec4 eye_coordinates = clip_coordinates * projection.inverse();
        eye_coordinates.m_z = -1.f;
        eye_coordinates.m_w = 0.f;
        std::cout<< "eye_coordinates = " << eye_coordinates <<std::endl;

        // convert the eye coordinates to World Space.
        ngl::Vec4 world_coordinates = eye_coordinates * view;
        m_ray.direction = world_coordinates.toVec3();
        m_ray.direction.normalize();
        std::cout<< "ray_direction = " << m_ray.direction <<std::endl;
        std::cout<< "======================================" <<std::endl;

        m_ray.start = cam_lookAt.eye;
        m_ray.end = m_ray.start + m_ray.direction * vc::far_clip;
        select_dir = {m_ray.start,m_ray.end};
    }
};

