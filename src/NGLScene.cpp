
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>


NGLScene::NGLScene() : screen_width(0),
                       screen_height(0),
                       m_model(),
                       m_view(),
                       m_projection(),
                       m_viewport(screen_width,screen_height,m_view,m_projection)
{
    setTitle( "Maya-like Viewport using NGL" );
}

void NGLScene::resizeGL(int w_, int h_)
{
    screen_width = w_;
    screen_height = h_;
    m_projection = ngl::perspective( 45.0f, static_cast<float>(screen_width)/screen_height, 0.1f, 200.0f );
    m_viewport.resize();
}

void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();

    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );

    m_view = ngl::lookAt(m_viewport.getCamera().getPosition(),
                         m_viewport.getCamera().getLookAt(),
                         m_viewport.getCamera().getUpVector());

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->use(ngl::nglDiffuseShader);
    shader->setUniform("lightPos",ngl::Vec3(0.0, 3.0f, 6.0f));
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);

    screen_width = this->size().width();
    screen_height = this->size().height();

    m_viewport.initialize();
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

void NGLScene::paintGL()
{
    glViewport( 0, 0, screen_width, screen_height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_viewport.update_draw();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

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

    m_viewport.keyPress(event_);
    update();
}

void NGLScene::mousePressEvent(QMouseEvent *event_)
{
    m_viewport.mousePress(event_);
}

void NGLScene::mouseMoveEvent(QMouseEvent *event_)
{
    m_viewport.mouseMove(event_);
    update();
}









