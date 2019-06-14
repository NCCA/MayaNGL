
#include "Viewport/Viewport.h"
#include <ngl/NGLStream.h>



Viewport::Viewport( const int &screenWidth_,
                    const int &screenHeight_,
                    ngl::Mat4 &view_,
                    ngl::Mat4 &projection_ )
                    :
                    screenWidth(screenWidth_),
                    screenHeight(screenHeight_),
                    view(view_),
                    orig_view(view),
                    projection(projection_),
                    m_aspectRatio(static_cast<float>(screenWidth)/screenHeight),
                    m_ortho_zoom(10.f),
                    m_mouse(),
                    m_camera(m_mouse),
                    m_projText(screenWidth,screenHeight),
                    m_model(),
                    m_axis(view)
{
    m_model.reset();
    m_model.setPosition(ngl::Vec3::zero());
}

void Viewport::loadLineColourShader()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("LineColour");

    auto MVP = projection * view * m_model.getMatrix();

    shader->setUniform("MVP",MVP);
    shader->setUniform("Dist",abs(view.m_32));
    shader->setUniform("enableViewAxisColours",false);
}

void Viewport::resize()
{
    m_aspectRatio = static_cast<float>(screenWidth)/screenHeight;
    m_projText.resize();
    m_axis.resize(m_aspectRatio);
}

Viewport::ProjectionMat Viewport::goPersp()
{
    return ngl::perspective(35.0f,static_cast<float>(screenWidth)/screenHeight,0.1f,200.f);
}

Viewport::ProjectionMat Viewport::goOrtho()
{
    return ngl::ortho(-m_aspectRatio*m_ortho_zoom,m_aspectRatio*m_ortho_zoom,-m_ortho_zoom,m_ortho_zoom,0.1f,200.f);
}

void Viewport::initialize()
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();

    shader->createShaderProgram( "LineColour" );
    shader->attachShader( "LineColour_Vertex", ngl::ShaderType::VERTEX );
    shader->attachShader( "LineColour_Geometry", ngl::ShaderType::GEOMETRY );
    shader->attachShader( "LineColour_Fragment", ngl::ShaderType::FRAGMENT );
    shader->loadShaderSource( "LineColour_Vertex", "shaders/Viewport/LineColour_Vertex.glsl" );
    shader->loadShaderSource( "LineColour_Geometry", "shaders/Viewport/LineColour_Geometry.glsl" );
    shader->loadShaderSource( "LineColour_Fragment", "shaders/Viewport/LineColour_Fragment.glsl" );
    shader->compileShader( "LineColour_Vertex" );
    shader->compileShader( "LineColour_Geometry" );
    shader->compileShader( "LineColour_Fragment" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Vertex" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Geometry" );
    shader->attachShaderToProgram( "LineColour", "LineColour_Fragment" );
    shader->linkProgramObject( "LineColour" );

    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();
    grid->createLineGrid("Grid",24,24,24);

    m_projText.initialize();
    m_axis.initialize();

    orig_view = view;
}

void Viewport::update_draw()
{
    view = orig_view * m_camera.Rot * m_camera.Tra;

    ngl::VAOPrimitives *grid = ngl::VAOPrimitives::instance();

    loadLineColourShader();
    grid->draw("Grid");

    m_projText.draw();
    m_axis.draw();
}

void Viewport::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_projText.title = "persp";
            m_camera.reset();
            m_mouse.reset();
            m_model.setRotation(ngl::Vec3::zero());
            projection = goPersp();
            orig_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_1:
            m_projText.title = "front";
            m_camera.front();
            m_mouse.reset();
            m_model.setRotation(90.f,0.f,0.f);
            m_ortho_zoom = 10.f;
            projection = goOrtho();
            orig_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_2:
            m_projText.title = "side";
            m_camera.side();
            m_mouse.reset();
            m_model.setRotation(0.f,0.f,90.f);
            m_ortho_zoom = 10.f;
            projection = goOrtho();
            orig_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        case Qt::Key_3:
            m_projText.title = "top";
            m_camera.top();
            m_mouse.reset();
            m_model.setRotation(ngl::Vec3::zero());
            m_ortho_zoom = 10.f;
            projection = goOrtho();
            orig_view = ngl::lookAt(m_camera.getPosition(),m_camera.getLookAt(),ngl::Vec3::up());
            break;

        default:
            break;
    }
}

void Viewport::mousePress(QMouseEvent *event_)
{
    if(event_->modifiers() & Qt::AltModifier)
        m_mouse.setAnchor(event_->x(),event_->y());
}

void Viewport::mouseMove(QMouseEvent *event_)
{
    m_mouse.setTransform(event_->x(),event_->y());

    if(event_->modifiers() & Qt::AltModifier)
    {
        switch(event_->buttons())
        {
            case Qt::LeftButton :
                if (m_camera.getCurrentView() == Camera::View::PERSPECTIVE)
                    m_camera.pan();
                break;

            case Qt::MiddleButton:
                m_camera.track();
                break;

            case Qt::RightButton:
                m_camera.dolly();
//                if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
//                {
//                    m_ortho_zoom -= m_mouse.getDirection().m_x * m_mouse.velocity * Mouse::slowdown;
//                    projection = goOrtho();
//                }
                break;

            default:
                break;
        }
    }
}




