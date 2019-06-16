
#include "Viewport/ViewAxis.h"


ViewAxis::ViewAxis( const vc::View &view_ )
                    :
                    scene_view(view_),
                    m_model(),
                    m_view(ngl::lookAt(ngl::Vec3::in(),ngl::Vec3::zero(),ngl::Vec3::up())),
                    m_projection(),
                    m_coordinates{{
                                      ngl::Vec3::zero(),
                                      ngl::Vec3::right(),
                                      ngl::Vec3::zero(),
                                      ngl::Vec3::up(),
                                      ngl::Vec3::zero(),
                                      ngl::Vec3::in()
                                 }},
                    m_vao()
{;}

void ViewAxis::initialize()
{
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(ngl::Vec3),m_coordinates[0].m_x));
    m_vao->setNumIndices(m_coordinates.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->unbind();
}

void ViewAxis::resize(float aspectRatio_)
{
    float zoom = 20.f;
    m_projection = ngl::ortho(-aspectRatio_*zoom,aspectRatio_*zoom,-zoom,zoom,0.1f,200.f);

    auto bl = ngl::Vec3(1.5f-(aspectRatio_*zoom),1.5f-zoom,0.f);
    m_model.translate(bl.m_x,bl.m_y,bl.m_z);
}

void ViewAxis::loadLineColourShader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("LineColour");

    auto rot = scene_view;
    rot.translate(0.f,0.f,0.f);
    auto MVP = m_projection * m_view * m_model * rot;

    shader->setUniform("MVP",MVP);
    shader->setUniform("Dist",1.f);
    shader->setUniform("enableViewAxisColours",true);
}

void ViewAxis::draw() const
{
    loadLineColourShader();
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
}
