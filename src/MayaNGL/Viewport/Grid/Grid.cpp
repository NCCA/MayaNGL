
#include "MayaNGL/Viewport/Grid/Grid_Def.hpp"


void Grid::load_shader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(mc::grid_shader);

    auto MVP = projection * view * m_model;

    shader->setUniform("MVP",MVP);
    shader->setUniform("cam_eye",lookAt.eye);
}

template<>
void Grid::view_orientation<mc::CamView::FRONT>()
{
    m_model.identity();
    m_model.rotateX(90.f);
}

template<>
void Grid::view_orientation<mc::CamView::SIDE>()
{
    m_model.identity();
    m_model.rotateZ(90.f);
}

void Grid::initialize()
{
    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();
    grid->createLineGrid("Grid",24,24,24);
}

void Grid::draw() const
{
    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();

    load_shader();
    grid->draw("Grid");
}
