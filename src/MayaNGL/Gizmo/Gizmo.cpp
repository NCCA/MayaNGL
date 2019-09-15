
#include "MayaNGL/Gizmo/Gizmo.h"


Gizmo::Gizmo( const mc::View &view_,
              const mc::Projection &projection_,
              const Camera &camera_ )
              :
              view(view_),
              projection(projection_),
              camera(camera_),
              position(mc::Position::zero()),
              m_model(),
              m_coordinates{{
                                mc::Direction(0.2f,0.f,0.f),
                                mc::Direction::right(),
                                mc::Direction(0.f,0.2f,0.f),
                                mc::Direction::up(),
                                mc::Direction(0.f,0.f,0.2f),
                                mc::Direction::in()
                           }},
              m_vao(),
              display(false)
{;}

void Gizmo::initialize()
{
    m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
    m_vao->bind();
    m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(mc::Position),m_coordinates[0].m_x));
    m_vao->setNumIndices(m_coordinates.size());
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_vao->unbind();

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createCone("arrow_head",0.2f,0.7f,10,1);
    prim->createDisk("central",0.5f,4);
}

void Gizmo::setPosition(float x_, float y_, float z_)
{
    position.m_x = x_;
    position.m_y = y_;
    position.m_z = z_;
}

void Gizmo::loadShader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(mc::AxisShader);

    auto MVP = projection * view * m_model;

    shader->setUniform("MVP",MVP);
}

void Gizmo::loadShader(mc::Colour &&colour_) const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);

    auto MVP = projection * view * m_model;

    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour",std::move(colour_));
}

void Gizmo::draw() const
{
    glClear(GL_DEPTH_BUFFER_BIT);

    auto average_dist = ((view.m_30+view.m_31+view.m_32)/3)*(-0.25f);
    mc::Size<float> uniform_scale = average_dist*0.25f;

    m_model.identity();
    {
        m_model.scale(average_dist,average_dist,average_dist);
        m_model.translate(position.m_x,position.m_y,position.m_z);
        loadShader();
        m_vao->bind();
        m_vao->draw();
        m_vao->unbind();
    }

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    m_model.identity();
    {
        mc::Rotation R;
        R.rotateY(90.f);
        m_model = uniform_scale * R;
        m_model.translate(position.m_x+average_dist,position.m_y,position.m_z);
        loadShader(mc::Colour(1.f,0.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        mc::Rotation R;
        R.rotateX(-90.f);
        m_model = uniform_scale * R;
        m_model.translate(position.m_x,position.m_y+average_dist,position.m_z);
        loadShader(mc::Colour(0.f,1.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        m_model.scale(uniform_scale,uniform_scale,uniform_scale);
        m_model.translate(position.m_x,position.m_y,position.m_z+average_dist);
        loadShader(mc::Colour(0.f,0.f,1.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        auto &&direction = camera.getInvDirection();
        auto &&shadow = camera.getInvShadow();

        mc::Rotation Rz;
        Rz.rotateZ(45.f);

        auto theta = atan2(direction.m_x,direction.m_z);
        mc::Rotation Ry = mc::Y_Matrix(-theta);

        auto rotationAxis = camera.getUp().cross(shadow);
        rotationAxis.normalize();
        auto phi = atan2(rotationAxis.dot(direction.cross(shadow)) , direction.dot(shadow));
        mc::Rotation Rx = mc::Axis_Matrix(phi,rotationAxis);

        mc::Rotation R = Rx * Ry * Rz;

        m_model = uniform_scale * R;
        m_model.translate(position.m_x,position.m_y,position.m_z);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        loadShader(mc::Colour(1.f,1.f,0.f,1.f));
        prim->draw("central");
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}
