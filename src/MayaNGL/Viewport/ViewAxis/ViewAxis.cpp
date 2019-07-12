
#include "MayaNGL/Viewport/ViewAxis/ViewAxis.h"


ViewAxis::ViewAxis( const mc::View &view_ )
                    :
                    scene_view(view_),
                    m_model(),
                    m_view(ngl::lookAt(mc::Position::in(),mc::Position::zero(),mc::Direction::up())),
                    m_projection(),
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

void ViewAxis::initialize()
{
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(mc::Position),m_coordinates[0].m_x));
    m_vao->setNumIndices(m_coordinates.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->unbind();
}

void ViewAxis::resize(float aspectRatio_)
{
    float zoom = 20.f;
    m_projection = ngl::ortho(-aspectRatio_*zoom,aspectRatio_*zoom,-zoom,zoom,mc::near_clip,mc::far_clip);

    auto pos = mc::Position(1.5f-(aspectRatio_*zoom),1.5f-zoom,0.f);
    m_model.translate(pos.m_x,pos.m_y,pos.m_z);
}

void ViewAxis::loadShader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(mc::AxisShader);

    auto rot = scene_view;
    rot.translate(0.f,0.f,0.f);
    auto MVP = m_projection * m_view * m_model * rot;

    shader->setUniform("MVP",MVP);
}

void ViewAxis::draw() const
{
    loadShader();
    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();
}
