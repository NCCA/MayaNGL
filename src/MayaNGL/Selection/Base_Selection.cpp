
#include "MayaNGL/Selection/Base_Selection.h"


template<>
Base_Selection<false>::Base_Selection( const mc::View &view_,
                                       const mc::Projection &projection_,
                                       const mc::LookAt &cam_lookAt_ )
                                       :
                                       view(view_),
                                       projection(projection_),
                                       cam_lookAt(cam_lookAt_),
                                       m_screen_width(0),
                                       m_screen_height(0),
                                       m_ray{cam_lookAt.eye,mc::Direction::zero()},
                                       m_selectables(),
                                       m_currently_selected()
{;}

template<>
void Base_Selection<false>::initialize()
{;}

template<>
void Base_Selection<false>::emitRay(int mouse_x, int mouse_y)
{
    // convert mouse position from Screen Space to Normalized Device Space.
    float normMouseX = (2.f*mouse_x)/m_screen_width - 1.f;
    float normMouseY = 1.f - (2.f*mouse_y)/m_screen_height;

    // create vector on Clip Space using -1 on the z-depth.
    mc::V4 clip_coordinates(normMouseX,normMouseY,-1.f,1.f);

    // convert the clip coordinates to Eye Space.
    mc::V4 eye_coordinates = clip_coordinates * const_cast<mc::Projection &>(projection).inverse();
    eye_coordinates.m_z = -1.f;
    eye_coordinates.m_w = 0.f;

    // convert the eye coordinates to World Space.
    mc::V4 world_coordinates = eye_coordinates * view;
    m_ray.position = cam_lookAt.eye;
    m_ray.direction = world_coordinates.toVec3();
    m_ray.direction.normalize();
}

template<>
void Base_Selection<false>::draw() const
{
    if (m_currently_selected.empty())
        return;

    for (const auto &i : m_currently_selected)
    {
        auto &&vprim = m_selectables.at(i);
        vprim.draw(view,projection);
    }
}


// ------------------------------------------------------------- //


Base_Selection<true>::Base_Selection( const mc::View &view_,
                                      const mc::Projection &projection_,
                                      const mc::LookAt &cam_lookAt_ )
                                      :
                                      Base_Selection<false>(view_,projection_,cam_lookAt_),
                                      m_ray_end(Base_Selection<false>::cam_lookAt.eye),
                                      m_vtxs{{Base_Selection<false>::m_ray.position,m_ray_end}}
{;}

void Base_Selection<true>::initialize()
{
    Base_Selection<false>::initialize();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createSphere("BV",1.f,12);
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
}

void Base_Selection<true>::emitRay(int mouse_x, int mouse_y)
{
    Base_Selection<false>::emitRay(mouse_x,mouse_y);

    m_ray_end = this->m_ray.position + this->m_ray.direction * mc::far_clip;
    m_vtxs = {{this->m_ray.position,m_ray_end}};
}

void Base_Selection<true>::draw() const
{
    if (m_currently_selected.empty())
        return;

    Base_Selection<false>::draw();

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);

    auto MV = projection * view;

    shader->setUniform("MVP",MV);
    shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f,1.f));

    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_vtxs.size()*sizeof(mc::Position),m_vtxs[0].m_x));
    m_vao->setNumIndices(m_vtxs.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->draw();
    m_vao->unbind();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    for (const auto &i : m_currently_selected)
    {
        auto &&vprim_transform = m_selectables.at(i).getTransform();
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        auto MVP = projection * view * vprim_transform;
        shader->setUniform("MVP",MVP);
        shader->setUniform("Colour",ngl::Vec4(1.f,0.263f,0.639f,1.f));
        prim->draw("BV");

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}


