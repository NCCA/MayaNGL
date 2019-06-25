
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>


NGLScene::NGLScene() : m_view(),
                       m_projection(),
                       m_maya(m_view,m_projection)
{
    setTitle( "Maya-like Viewport using NGL" );
}

void NGLScene::resizeGL(int w_, int h_)
{
    m_projection = ngl::perspective(35.0f, static_cast<float>(w_)/h_, 0.1f, 200.0f );
    m_maya.resize(w_,h_);
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

    m_view = ngl::lookAt(ngl::Vec3(28,21,28),
                         ngl::Vec3::zero(),
                         ngl::Vec3::up());

    m_maya.initialize();

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->use(ngl::nglDiffuseShader);
    shader->setUniform("lightPos",ngl::Vec3(0.0, 3.0f, 6.0f));
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
}

void NGLScene::loadDiffuseShader(const ngl::Mat4 &mat_)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglDiffuseShader);

    ngl::Mat4 MV = m_view * mat_;
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

    m_maya.update_draw();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    m_model.identity();
    {
        m_model.translate(0.f,0.f,0.f);
        m_model.rotateY(45.f);
        m_model.scale(4.f,4.f,4.f);
        loadDiffuseShader(m_model);
        prim->draw( "teapot" );
//        m_viewport.make_selectable(1,"teapot",m_model);
    }

    m_transform.reset();
    {
        m_transform.setPosition(8.f,0.f,0.f);
        m_transform.setRotation(0.f,45.f,0.f);
        m_transform.setScale(2.f,2.f,2.f);
        loadDiffuseShader(m_transform.getMatrix());
        prim->draw( "football" );
//        m_viewport.make_selectable(2,"football",m_transform);
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

    m_maya.keyPress(event_);
    update();
}

void NGLScene::mousePressEvent(QMouseEvent *event_)
{
    m_maya.mousePress(event_);
    update();
}

void NGLScene::mouseMoveEvent(QMouseEvent *event_)
{
    m_maya.mouseMove(event_);
    update();
}









