
#include "MayaNGL/Viewport/Grid/Grid_Def.hpp"


void Grid::loadLineColourShader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("LineColour");

    auto MVP = projection * view * m_model;

    shader->setUniform("MVP",MVP);
    shader->setUniform("cam_eye",lookAt.eye);
    shader->setUniform("enableViewAxisColours",false);
}

template<>
void Grid::viewOrientation<mc::CamView::FRONT>()
{
    m_model.identity();
    m_model.rotateX(90.f);
}

template<>
void Grid::viewOrientation<mc::CamView::SIDE>()
{
    m_model.identity();
    m_model.rotateZ(90.f);
}

void Grid::initialize()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->createShaderProgram( "LineColour" );
    shader->attachShader( "LineColour_Vertex", ngl::ShaderType::VERTEX );
    shader->attachShader( "LineColour_Geometry", ngl::ShaderType::GEOMETRY );
    shader->attachShader( "LineColour_Fragment", ngl::ShaderType::FRAGMENT );
    shader->loadShaderSource( "LineColour_Vertex", "shaders/MayaNGL/LineColour_Vertex.glsl" );
    shader->loadShaderSource( "LineColour_Geometry", "shaders/MayaNGL/LineColour_Geometry.glsl" );
    shader->loadShaderSource( "LineColour_Fragment", "shaders/MayaNGL/LineColour_Fragment.glsl" );
    shader->compileShader( "LineColour_Vertex" );
    shader->compileShader( "LineColour_Geometry" );
    shader->compileShader( "LineColour_Fragment" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Vertex" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Geometry" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Fragment" );
    shader->linkProgramObject( "LineColour" );

    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();
    grid->createLineGrid("Grid",24,24,24);
}

void Grid::draw() const
{
    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();

    loadLineColourShader();
    grid->draw("Grid");
}
