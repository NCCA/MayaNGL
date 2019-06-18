
#include "Viewport/Viewport.h"



Viewport::Viewport( vc::View &view_,
                    vc::Projection &projection_ )
                    :
                    view(view_),
                    projection(projection_),
                    m_aspectRatio(0.f),
                    m_orthographic_zoom(10.f),
                    m_initial_lookAt(),
                    m_initial_view(view),
                    m_mouse(),
                    m_camera(m_mouse,m_initial_lookAt),
                    m_grid(view,projection,m_camera),
                    m_projText(),
                    m_axis(view)
{;}

void Viewport::initialize()
{
    m_grid.initialize();
    m_projText.initialize();
    m_axis.initialize();

    m_initial_view = view;



    select_dir[0] = select_dir[1] = m_camera.getEye();
    sdvao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
}

void Viewport::resize(int w_, int h_)
{
    m_aspectRatio = static_cast<float>(w_)/h_;

    if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
        goOrtho();
    else
        goPersp();

    m_projText.resize(w_,h_);
    m_axis.resize(m_aspectRatio);
}

void Viewport::goPersp()
{
    projection = ngl::perspective(vc::fov,m_aspectRatio,vc::near_clip,vc::far_clip);
}

void Viewport::goOrtho()
{
    projection = ngl::ortho( -m_aspectRatio*m_orthographic_zoom,
                              m_aspectRatio*m_orthographic_zoom,
                             -m_orthographic_zoom,
                              m_orthographic_zoom,
                              vc::near_clip,
                              vc::far_clip );
}

void Viewport::update_draw()
{
    view = m_initial_view * m_camera.computeTransform();

    m_grid.draw();
    m_projText.draw();
    m_axis.draw();




    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);

    auto MVP = projection * view * ngl::Mat4();

    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f));

    sdvao->bind();
    sdvao->setData(ngl::AbstractVAO::VertexData(select_dir.size()*sizeof(ngl::Vec3),select_dir[0].m_x));
    sdvao->setNumIndices(select_dir.size());
    sdvao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    sdvao->draw();
    sdvao->unbind();
}

void Viewport::keyPress(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_R:
            m_projText.title = "persp";
            m_camera.reset(m_initial_lookAt);
            m_mouse.reset();
            m_grid.reset();
            goPersp();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_1:
            m_projText.title = "front";
            m_camera.front();
            m_mouse.reset();
            m_grid.front();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_2:
            m_projText.title = "side";
            m_camera.side();
            m_mouse.reset();
            m_grid.side();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
            break;

        case Qt::Key_3:
            m_projText.title = "top";
            m_camera.top();
            m_mouse.reset();
            m_grid.reset();
            m_orthographic_zoom = 10.f;
            goOrtho();
            m_initial_view = ngl::lookAt(m_camera.getEye(),m_camera.getTarget(),m_camera.getUp());
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

void Viewport::mouseRelease(QMouseEvent *event_)
{
    if(event_->modifiers() & Qt::ControlModifier)
    {
        std::cout<<std::flush;

        /// from here: http://antongerdelan.net/opengl/raycasting.html
        /// Pipeline: Local Space (Vertex) -> Model Matrix (World Space) -> View Matrix (Eye Space) ->
        ///           Projection Matrix (Clip Space) -> Perspective Division (Normalized Device Space) ->
        ///           Viewport Transform (Screen Space).

        int screen_width = 1024;
        int screen_height = 640;

        // get mouse position on Screen Space.
        int mouseX = event_->x();
        int mouseY = event_->y();
        std::cout<< "mouseX = " << mouseX <<std::endl;
        std::cout<< "mouseX = " << mouseY <<std::endl;

        // convert mouse position to Normalized Device Space.
        float normMouseX = (2.f*mouseX)/screen_width - 1.f;
        float normMouseY = 1.f - (2.f*mouseY)/screen_height;
        std::cout<< "normMouseX = " << normMouseX <<std::endl;
        std::cout<< "normMouseY = " << normMouseY <<std::endl;

        // create vector on Clip Space using -1 on the z-depth.
        ngl::Vec4 clip_coordinates(normMouseX,normMouseY,-1.f,1.f);
        std::cout<< "clip_coordinates = " << clip_coordinates <<std::endl;

        // convert the clip coordinates to Eye Space.
        ngl::Vec4 eye_coordinates = clip_coordinates * projection.inverse();
        eye_coordinates.m_z = -1.f;
        eye_coordinates.m_w = 0.f;
        std::cout<< "eye_coordinates = " << eye_coordinates <<std::endl;

        // convert the eye coordinates to World Space.
        ngl::Vec4 world_coordinates = eye_coordinates * view;
        ngl::Vec3 ray_direction = world_coordinates.toVec3();
        ray_direction.normalize();
        std::cout<< "ray_direction = " << ray_direction <<std::endl;
        std::cout<< "======================================" <<std::endl;

        select_dir[0] = m_camera.getEye();
        select_dir[1] = m_camera.getEye()+ray_direction*100.f;
    }
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
                if (m_camera.getCurrentView() != Camera::View::PERSPECTIVE)
                { //--------------- this needs work ---------------
                    std::cout<< m_orthographic_zoom <<std::endl;
                    if ((m_orthographic_zoom < 0.1f) && (m_mouse.getDrag().m_x > 0.f))
                        std::cout<< "that's it!" <<std::endl;
                    else
                        m_orthographic_zoom -= m_mouse.getDrag().m_x * Mouse::slowdown;
                    goOrtho();
                } //-----------------------------------------------
                break;

            default:
                break;
        }
    }
}




