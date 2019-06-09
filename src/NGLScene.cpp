
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>



NGLScene::NGLScene()
{
    setTitle( "Maya-like Viewport with NGL" );
}

void NGLScene::resizeGL( int _w, int _h )
{
    m_projection = ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.1f, 200.0f );

    width  = static_cast<int>( _w * devicePixelRatio() );
    height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();

    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );

    ngl::ShaderLib* shader = ngl::ShaderLib::instance();

    shader->use(ngl::nglDiffuseShader);

    ngl::Vec3 from( 12.0f, 6.0f, 12.0f );
    ngl::Vec3 to( 0.0f, 0.0f, 0.0f );
    ngl::Vec3 up( 0.0f, 1.0f, 0.0f );

    m_view=ngl::lookAt(from,to,up);

    shader->setUniform("Colour",1.f,0.4f,0.4f,1.0f);
    shader->setUniform("lightPos",ngl::Vec3(0.0, 4.0f, 4.0f));
    shader->setUniform("lightDiffuse",1.0f,1.0f,1.0f,1.0f);
}


void NGLScene::loadMatricesToShader()
{
    ngl::ShaderLib* shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglDiffuseShader);

    ngl::Mat4 MVP = m_projection*m_view*m_transform.getMatrix();
    ngl::Mat4 normalMatrix = m_view;
    normalMatrix.inverse().transpose();

    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
}


void NGLScene::paintGL()
{
    glViewport( 0, 0, width, height );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    ngl::ShaderLib* shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglDiffuseShader);


    ngl::VAOPrimitives* prim = ngl::VAOPrimitives::instance();

    m_transform.reset();
    {
        m_transform.setPosition(0.f,0.f,0.f);
        loadMatricesToShader();
        prim->draw( "teapot" );
    }

}


void NGLScene::keyPressEvent( QKeyEvent* _event )
{
    switch ( _event->key() )
    {
        case Qt::Key_Escape:
            QGuiApplication::exit( EXIT_SUCCESS );
            break;

        #ifndef USINGIOS_
        case Qt::Key_W:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            break;

        case Qt::Key_S:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            break;
        #endif

        default: break;
    }
    update();
}
