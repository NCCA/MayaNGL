
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>


NGLScene::NGLScene() : m_camera()
{
    setTitle( "Maya-like Viewport using NGL" );
}

void NGLScene::resizeGL(int w_, int h_)
{
    m_projection = ngl::perspective( 45.0f, static_cast<float>(w_)/h_, 0.1f, 200.0f );
}

void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();

    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );

    m_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),m_camera.getUpVector());

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->createShaderProgram( "LineColour" );
    shader->attachShader( "LineColour_Vertex", ngl::ShaderType::VERTEX );
    shader->attachShader( "LineColour_Geometry", ngl::ShaderType::GEOMETRY );
    shader->attachShader( "LineColour_Fragment", ngl::ShaderType::FRAGMENT );
    shader->loadShaderSource( "LineColour_Vertex", "shaders/LineColour_Vertex.glsl" );
    shader->loadShaderSource( "LineColour_Geometry", "shaders/LineColour_Geometry.glsl" );
    shader->loadShaderSource( "LineColour_Fragment", "shaders/LineColour_Fragment.glsl" );
    shader->compileShader( "LineColour_Vertex" );
    shader->compileShader( "LineColour_Geometry" );
    shader->compileShader( "LineColour_Fragment" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Vertex" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Geometry" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Fragment" );
    shader->linkProgramObject( "LineColour" );

    shader->use(ngl::nglDiffuseShader);
    shader->setUniform("lightPos",ngl::Vec3(0.0, 3.0f, 6.0f));
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createLineGrid("grid",24,24,24);

    m_projection_title = std::make_unique<ngl::Text>(QFont("Helvetica",10,QFont::Bold));
    m_projection_title->setScreenSize(this->size().width(),this->size().height());
    m_projection_title->setColour(1.f,1.f,1.f);
}

void NGLScene::loadDiffuseShader()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglDiffuseShader);

    ngl::Mat4 MV = m_view * m_model.getMatrix();
    ngl::Mat4 MVP = m_projection * MV;
    ngl::Mat3 normalMatrix = MV;
    normalMatrix.inverse().transpose();

    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    shader->setUniform("Colour",0.7f,0.7f,0.7f,1.0f);
}

void NGLScene::loadLineColourShader()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("LineColour");

    ngl::Mat4 MVP = m_projection * m_view * m_model.getMatrix();

    float dist = m_view.m_32*0.025;
    shader->setUniform("MVP",MVP);
    shader->setUniform("Dist",dist);
}

void NGLScene::paintGL()
{
    glViewport( 0, 0, this->size().width(), this->size().height());
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),m_camera.getUpVector());

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    m_model.reset();
    {
        m_model.setPosition(0.f,0.f,0.f);
        loadLineColourShader();
        prim->draw( "grid" );
    }

    m_projection_title->renderText(this->size().width()*0.5-3,this->size().height()-50,"persp");

    m_model.reset();
    {
        m_model.setPosition(0.f,0.f,0.f);
        m_model.setScale(2.f,2.f,2.f);
        loadDiffuseShader();
        prim->draw( "teapot" );
    }
}

void NGLScene::keyPressEvent(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_Escape:
            QGuiApplication::exit(EXIT_SUCCESS);
            break;

        default:
            break;
    }

    m_camera.keyPress(event_);
    update();
}

void NGLScene::mousePressEvent(QMouseEvent *event_)
{
    m_camera.mousePress(event_);
}

void NGLScene::mouseMoveEvent(QMouseEvent *event_)
{
    m_camera.mouseMove(event_);
    update();
}









