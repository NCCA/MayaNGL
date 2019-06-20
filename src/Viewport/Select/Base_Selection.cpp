
#include "Viewport/Select/Base_Selection.h"


template<>
Base_Selection<false>::Base_Selection( const LookAt &cam_lookAt_,
                                       const vc::View &view_,
                                       /*const*/ vc::Projection &projection_ )
                                       :
                                       cam_lookAt(cam_lookAt_),
                                       view(view_),
                                       projection(projection_),
                                       m_ray{cam_lookAt.eye,vc::Direction::zero()},
                                       m_selectables(),
                                       m_current_selections()
{;}

template<>
void Base_Selection<false>::initialize()
{;}

template<>
void Base_Selection<false>::emitRay(int mouse_x, int mouse_y)
{
/// from here: http://antongerdelan.net/opengl/raycasting.html
/// Pipeline: Local Space (Vertex) -> Model Matrix (World Space) -> View Matrix (Eye Space) ->
///           Projection Matrix (Clip Space) -> Perspective Division (Normalized Device Space) ->
///           Viewport Transform (Screen Space).

    /* THIS NEEDS SORTING OUT */
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

    m_ray.position = cam_lookAt.eye;
    m_ray.direction = world_coordinates.toVec3();
    m_ray.direction.normalize();
}

template<>
void Base_Selection<false>::draw() const
{
    if (m_current_selections.empty())
        return;

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    for (const auto &i : m_current_selections)
    {
        auto &&prim_name = m_selectables.at(i).name;
        auto &&prim_transform = m_selectables.at(i).transform;

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        auto MVP = projection * view * prim_transform;
        shader->setUniform("MVP",MVP);
        shader->setUniform("Colour",ngl::Vec4(0.263f,1.f,0.639f,1.f));
        prim->draw(prim_name);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}


// ------------------------------------------------------------- //


Base_Selection<true>::Base_Selection( const LookAt &cam_lookAt_,
                                      const vc::View &view_,
                                      /*const*/ vc::Projection &projection_ )
                                      :
                                      Base_Selection<false>(cam_lookAt_,view_,projection_),
                                      m_ray_end(Base_Selection<false>::cam_lookAt.eye),
                                      m_vtxs{{Base_Selection<false>::m_ray.position,m_ray_end}}
{;}

void Base_Selection<true>::initialize()
{
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createSphere("BV",1.f,10);
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
}

void Base_Selection<true>::emitRay(int mouse_x, int mouse_y)
{
    Base_Selection<false>::emitRay(mouse_x,mouse_y);
    m_ray_end = this->m_ray.position + this->m_ray.direction * vc::far_clip;
    m_vtxs = {{this->m_ray.position,m_ray_end}};
}

void Base_Selection<true>::draw() const
{
    Base_Selection<false>::draw();

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);

    auto MV = projection * view;

    shader->setUniform("MVP",MV);
    shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f,1.f));

    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_vtxs.size()*sizeof(vc::Position),m_vtxs[0].m_x));
    m_vao->setNumIndices(m_vtxs.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->draw();
    m_vao->unbind();

    if (!m_current_selections.empty())
    {
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        for (const auto &i : m_current_selections)
        {
            auto &&prim_transform = m_selectables.at(i).transform;

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

            auto MVP = projection * view * prim_transform;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,0.263f,0.639f,1.f));
            prim->draw("BV");

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }
}


