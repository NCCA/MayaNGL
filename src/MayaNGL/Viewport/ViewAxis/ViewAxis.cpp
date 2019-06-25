
#include "MayaNGL/Viewport/ViewAxis/ViewAxis.h"


ViewAxis::ViewAxis( const vc::View &view_ )
                    :
                    scene_view(view_),
                    m_model(),
                    m_view(ngl::lookAt(vc::Position::in(),vc::Position::zero(),vc::Direction::up())),
                    m_projection(),
                    m_coordinates{{
                                      vc::Direction::zero(),
                                      vc::Direction::right(),
                                      vc::Direction::zero(),
                                      vc::Direction::up(),
                                      vc::Direction::zero(),
                                      vc::Direction::in()
                                 }},
                    m_vao()
{;}

void ViewAxis::initialize()
{
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(vc::Position),m_coordinates[0].m_x));
    m_vao->setNumIndices(m_coordinates.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->unbind();
}

void ViewAxis::resize(float aspectRatio_)
{
    float zoom = 20.f;
    m_projection = ngl::ortho(-aspectRatio_*zoom,aspectRatio_*zoom,-zoom,zoom,vc::near_clip,vc::far_clip);

    auto bl = vc::Position(1.5f-(aspectRatio_*zoom),1.5f-zoom,0.f);
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
    shader->setUniform("enableViewAxisColours",true);
}

void ViewAxis::draw() const
{
    loadLineColourShader();
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
}
