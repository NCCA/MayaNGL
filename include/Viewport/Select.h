#pragma once

#include "Viewport/LookAt.h"
#include <set>
#include <ngl/Transformation.h>


/*
 * THIS IS SUCH A BAD CODE!!! AS SOON AS IT WORKS, I'M MAJOR REFACTORING...
*/

struct Select
{
    const LookAt &cam_lookAt;
    /*const*/ vc::View &view;
    /*const*/ vc::Projection &projection;

    struct Ray
    {
        vc::Position position;
        vc::Direction direction;
    };
    Ray m_ray;
    vc::Position ray_end;

    std::vector<vc::Position> select_dir; // why can't ngl::Vec3* work?
    std::unique_ptr<ngl::AbstractVAO> sdvao;

    // this needs to be a set/map for sure, but I need a smart way of handling
    // cases where the user wants the same primitive with the same transforms.
    std::unordered_map<std::size_t,std::tuple<ngl::Mat4,std::string,bool> > selectables;



    explicit Select( const LookAt &cam_lookAt_,
                     /*const*/ vc::View &view_,
                     /*const*/ vc::Projection &projection_ )
                     :
                     cam_lookAt(cam_lookAt_),
                     view(view_),
                     projection(projection_),
                     m_ray{cam_lookAt.eye,vc::Direction::zero()},
                     ray_end(cam_lookAt.eye),
                     select_dir{{m_ray.position,ray_end}},
                     selectables()
    {;}

    void make_selectable(std::size_t id_, const vc::Model &model_, std::string &&prim_name_)
    {
        selectables.emplace(id_,std::make_tuple(model_,prim_name_,false));
    }

    void initialize()
    {
        sdvao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        prim->createSphere("BV",1.f,12);
    }

    void draw() const
    {
        ngl::ShaderLib *shader = ngl::ShaderLib::instance();
        shader->use(ngl::nglColourShader);

        auto MV = projection * view;

        shader->setUniform("MVP",MV);
        shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f,1.f));

        sdvao->bind();
        sdvao->setData(ngl::AbstractVAO::VertexData(select_dir.size()*sizeof(ngl::Vec3),select_dir[0].m_x));
        sdvao->setNumIndices(select_dir.size());
        sdvao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
        sdvao->draw();
        sdvao->unbind();


        if (selectables.size() > 0)
        {
            for (const auto &i : selectables)
            {
                auto &&model_mat = std::get<0>(i.second);
                auto &&prim_name = std::get<1>(i.second);
                auto &&selected = std::get<2>(i.second);

                if (selected)
                {
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

                    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

                    auto MVP = projection * view * model_mat;
                    shader->setUniform("MVP",MVP);
                    shader->setUniform("Colour",ngl::Vec4(0.f,1.f,0.f,1.f));
//                    prim->draw("BV");
                    prim->draw(prim_name);

                    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                }
            }
        }

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

        // convert mouse position from Screen Space to Normalized Device Space.
        float normMouseX = (2.f*mouse_x)/screen_width - 1.f;
        float normMouseY = 1.f - (2.f*mouse_y)/screen_height;

        // create vector on Clip Space using -1 on the z-depth.
        ngl::Vec4 clip_coordinates(normMouseX,normMouseY,-1.f,1.f);

        // convert the clip coordinates to Eye Space.
        ngl::Vec4 eye_coordinates = clip_coordinates * projection.inverse();
        eye_coordinates.m_z = -1.f;
        eye_coordinates.m_w = 0.f;

        // convert the eye coordinates to World Space.
        ngl::Vec4 world_coordinates = eye_coordinates * view;
        m_ray.direction = world_coordinates.toVec3();
        m_ray.direction.normalize();

        m_ray.position = cam_lookAt.eye;
        ray_end = m_ray.position + m_ray.direction * vc::far_clip;
        select_dir = {m_ray.position,ray_end};
    }

    bool clickedOnObject(const std::tuple<ngl::Mat4,std::string,bool> &selectable_) const
    {
        auto &&model_mat = std::get<0>(selectable_);

        vc::Position bv_pos = vc::Position{model_mat.m_30,model_mat.m_31,model_mat.m_32};
        float bv_radius = model_mat.m_00; //get max component from scale

        auto poi = vc::intersect(m_ray.position,m_ray.direction,bv_pos,bv_radius);
        if (poi != vc::failed)
            return true;
        return false;
    }

    void pick(int mouse_x, int mouse_y)
    {
        if (selectables.size() > 0)
        {
            emitRay(mouse_x,mouse_y);

            for (auto &&i : selectables)
            {
                auto &&selected = std::get<2>(i.second);
                if (clickedOnObject(i.second))
                {
                    // pick the correct object. that's hard because ngl::VAOPrimitives does
                    // not have a static object counter.
                    // write a shader that sits on top of another shader.

                    selected = true;
                    break;
                }
                selected = false;
            }
        }
    }

};

