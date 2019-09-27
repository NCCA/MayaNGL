
#include "MayaNGL/Selection/BaseSelection.h"


template<>
BaseSelection<false>::BaseSelection( const mc::View &view_,
                                     const mc::Projection &projection_,
                                     const mc::LookAt &cam_lookAt_  )
                                     :
                                     SelectableMap(),
                                     view(view_),
                                     projection(projection_),
                                     cam_lookAt(cam_lookAt_),
                                     m_ray{cam_lookAt.eye,mc::Direction::zero()}
{;}

template<>
void BaseSelection<false>::initialize()
{;}

template<>
void BaseSelection<false>::emit_ray(float mouse_x_, float mouse_y_)
{
    // convert mouse position from Screen Space to Normalized Device Space.
    float norm_mouse_x = (2.f*mouse_x_)/m_screen_width - 1.f;
    float norm_mouse_y = 1.f - (2.f*mouse_y_)/m_screen_height;

    // create vector on Clip Space using -1 on the z-depth.
    mc::V4 clip_coordinates(norm_mouse_x,norm_mouse_y,-1.f,1.f);

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
void BaseSelection<false>::draw() const
{
    if (m_currently_selected.empty())
        return;

    for (const auto &i : m_currently_selected)
    {
        auto &&prim = m_selectables.at(i);
        prim.draw(view,projection);
    }
}


// ------------------------------------------------------------- //


BaseSelection<true>::BaseSelection( const mc::View &view_,
                                    const mc::Projection &projection_,
                                    const mc::LookAt &cam_lookAt_ )
                                    :
                                    BaseSelection<false>(view_,projection_,cam_lookAt_),
                                    m_ray_end(BaseSelection<false>::cam_lookAt.eye),
                                    m_vtxs{{BaseSelection<false>::m_ray.position,m_ray_end}}
{;}

void BaseSelection<true>::initialize()
{
    BaseSelection<false>::initialize();

    m_bv.initialize();
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
}

void BaseSelection<true>::emit_ray(int mouse_x_, int mouse_y_)
{
    BaseSelection<false>::emit_ray(mouse_x_,mouse_y_);

    m_ray_end = this->m_ray.position + this->m_ray.direction * mc::far_clip;
    m_vtxs = {{this->m_ray.position,m_ray_end}};
}

void BaseSelection<true>::draw()
{
    if (m_currently_selected.empty())
        return;

    BaseSelection<false>::draw();

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

    for (const auto &i : m_currently_selected)
    {
        auto &&prim_transform = m_selectables.at(i).get_transform();
        m_bv.draw(prim_transform,view,projection);
    }
}


