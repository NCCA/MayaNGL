
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>


NGLScene::NGLScene() : m_model(),
                       m_view(),
                       m_projection(),
                       m_viewport(m_view,m_projection)
{
    setTitle( "Maya-like Viewport using NGL" );
}

void NGLScene::resizeGL(int w_, int h_)
{
    m_projection = ngl::perspective(35.0f, static_cast<float>(w_)/h_, 0.1f, 200.0f );
    m_viewport.resize(w_,h_);
}

void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();

    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );

//    m_view = ngl::lookAt(m_viewport.getCamera().getEye(),
//                         m_viewport.getCamera().getTarget(),
//                         m_viewport.getCamera().getUp());

    m_view = ngl::lookAt({0.f,0.f,28.f},
                         {0.f,0.f,0.f},
                         {0.f,1.f,0.f});

    m_viewport.initialize(ngl::Vec3{0.f,0.f,28.f},ngl::Vec3{0.f,0.f,0.f},ngl::Vec3{0.f,1.f,0.f});

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->use(ngl::nglDiffuseShader);
    shader->setUniform("lightPos",ngl::Vec3(0.0, 3.0f, 6.0f));
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
}

void NGLScene::loadDiffuseShader()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglDiffuseShader);

    ngl::Mat4 MV = m_view * m_model;
    ngl::Mat4 MVP = m_projection * MV;
    ngl::Mat3 normalMatrix = MV;
    normalMatrix.inverse().transpose();

    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
    shader->setUniform("Colour",0.7f,0.7f,0.7f,1.0f);
}

void NGLScene::paintGL()
{
    glViewport( 0, 0, width(), height());
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_viewport.update_draw();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    m_model.identity();
    {
        m_model.translate(0.f,0.f,0.f);
        m_model.rotateY(45.f);
        m_model.scale(4.f,4.f,4.f);
        loadDiffuseShader();
        prim->draw( "teapot" );
        m_viewport.make_selectable(1,"teapot",m_model);
    }

    m_model.identity();
    {
        m_model.translate(8.f,0.f,0.f);
        m_model.rotateY(45.f);
        m_model.rotateX(45.f);
        m_model.scale(2.f,2.f,2.f);
        loadDiffuseShader();
        prim->draw( "football" );
        m_viewport.make_selectable(2,"football",m_model);
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
    update();
}

void NGLScene::mouseMoveEvent(QMouseEvent *event_)
{
    m_viewport.mouseMove(event_);
    update();
}









