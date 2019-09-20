
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <QGuiApplication>


NGLScene::NGLScene() : m_airplane_mesh("models_textures/airplane.obj"),
                       m_fish_mesh(),
                       m_view(),
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

    m_view = ngl::lookAt(m_maya.get_camera().get_eye(),
                         m_maya.get_camera().get_target(),
                         m_maya.get_camera().get_up());

    m_maya.initialize();

    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->createShaderProgram("DiffTexShader");
    shader->attachShader("DiffTexVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("DiffTexFragment",ngl::ShaderType::FRAGMENT);
    shader->loadShaderSource("DiffTexVertex","shaders/DiffTexVertex.glsl");
    shader->loadShaderSource("DiffTexFragment","shaders/DiffTexFragment.glsl");
    shader->compileShader("DiffTexVertex");
    shader->compileShader("DiffTexFragment");
    shader->attachShaderToProgram("DiffTexShader","DiffTexVertex");
    shader->attachShaderToProgram("DiffTexShader","DiffTexFragment");
    shader->linkProgramObject("DiffTexShader");

    auto lightPos = ngl::Vec3(0.f, 3.f, 6.f);
    auto lightDiffuse = ngl::Vec4(1.f, 1.f, 1.f, 1.f);

    shader->use("DiffTexShader");
    shader->setUniform("lightPos",lightPos);
    shader->setUniform("lightDiffuse",lightDiffuse);

    shader->use(ngl::nglDiffuseShader);
    shader->setUniform("lightPos",lightPos);
    shader->setUniform("lightDiffuse",lightDiffuse);

    m_airplane_mesh.createVAO();
    m_fish_mesh = std::make_unique<ngl::Obj>("models_textures/fish.obj","models_textures/fish.jpg");
    m_fish_mesh->createVAO();
}

template<>
void NGLScene::loadShader<false>(const ngl::Mat4 &mat_)
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

template<>
void NGLScene::loadShader<true>(const ngl::Mat4 &mat_)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("DiffTexShader");

    ngl::Mat4 MV = m_view * mat_;
    ngl::Mat4 MVP = m_projection * MV;
    ngl::Mat3 normalMatrix = MV;
    normalMatrix.inverse().transpose();

    shader->setUniform("MVP",MVP);
    shader->setUniform("normalMatrix",normalMatrix);
}

void NGLScene::paintGL()
{
    glViewport( 0, 0, width(), height());
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_maya.draw();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    // This is a moveable object, so there's no point in reseting the model matrix.
    {
        m_model.scale(1.f,1.f,1.f);
        loadShader<false>(m_model);
        prim->draw( "teapot" );
        m_maya.make_selectable_and_moveable(1,"teapot",m_model);
    }

//    {
//        m_model.scale(2.f,2.f,2.f);
//        loadShader<true>(m_model);
//        m_fish_mesh->draw();
//        m_maya.make_selectable_and_moveable(2,m_fish_mesh,m_model);
//    }

//    m_transform.reset();
//    {
//        m_transform.setPosition(8.f,0.f,0.f);
//        loadShader<false>(m_transform.getMatrix());
//        prim->draw( "football" );
//        m_maya.make_selectable(3,"football",m_transform);
//    }

//    m_transform.reset();
//    {
//        m_transform.setPosition(0.f,0.f,8.f);
//        m_transform.setRotation(0.f,30.f,0.f);
//        m_transform.setScale(2.f,2.f,2.f);
//        loadShader<false>(m_transform.getMatrix());
//        m_airplane_mesh.draw();
//        m_maya.make_selectable(4,m_airplane_mesh,m_transform);
//    }

    m_maya.draw_gizmo();
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

    m_maya.key_press(event_);
    update();
}

void NGLScene::mousePressEvent(QMouseEvent *event_)
{
    m_maya.mouse_press(event_);
    update();
}

void NGLScene::mouseMoveEvent(QMouseEvent *event_)
{
    m_maya.mouse_move(event_);
    update();
}

void NGLScene::mouseReleaseEvent(QMouseEvent *event_)
{
    m_maya.mouse_release(event_);
    update();
}










