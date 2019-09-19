
#include "MayaNGL/Gizmo/Gizmo.h"


Gizmo::Gizmo( const mc::View &view_,
              const mc::Projection &projection_,
              const Camera &camera_ )
              :
              view(view_),
              projection(projection_),
              camera(camera_),
              m_object_model(nullptr),
              m_position(mc::Position::zero()),
              m_average_dist(0.f),
              m_uniform_scale(1.f),
              m_orientation(),
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
              mouse_move(),
              m_hv_axis()
{;}

template<>
bool Gizmo::clickedOn<Gizmo::Handle::X>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[1]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_x < calc_length(m_position.m_x)))
        return true;
    return false;
}

template<>
bool Gizmo::clickedOn<Gizmo::Handle::Y>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[3]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_y < calc_length(m_position.m_y)))
        return true;
    return false;
}

template<>
bool Gizmo::clickedOn<Gizmo::Handle::Z>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[5]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_z < calc_length(m_position.m_z)))
        return true;
    return false;
}

template<>
bool Gizmo::clickedOn<Gizmo::Handle::MID>(const mc::Ray &mouse_ray_)
{
    auto &&direction = camera.getInvDirection();
    float size = m_uniform_scale*0.3f;
    mc::Plane<false> handle = {m_position,direction,mc::V2(size,size),m_orientation};

    auto poi = mc::intersect(mouse_ray_,handle);
    if (poi != mc::failed)
        return true;
    return false;
}

template<>
void Gizmo::dragOn<Gizmo::Handle::X>()
{
    auto Tx = mouse_move.m_x * m_hv_axis[0].m_x + mouse_move.m_y * m_hv_axis[1].m_x;
    m_position.m_x += Tx;
    m_object_model->m_30 = m_position.m_x;
}

template<>
void Gizmo::dragOn<Gizmo::Handle::Y>()
{
    auto Ty = mouse_move.m_y * m_hv_axis[1].m_y;
    m_position.m_y += Ty;
    m_object_model->m_31 = m_position.m_y;
}

template<>
void Gizmo::dragOn<Gizmo::Handle::Z>()
{
    auto Tz = mouse_move.m_x * m_hv_axis[0].m_z + mouse_move.m_y * m_hv_axis[1].m_z;
    m_position.m_z += Tz;
    m_object_model->m_32 = m_position.m_z;
}

template<>
void Gizmo::dragOn<Gizmo::Handle::MID>()
{
    auto Tx = mouse_move.m_x * m_hv_axis[0].m_x + mouse_move.m_y * m_hv_axis[1].m_x;
    auto Ty = mouse_move.m_y * m_hv_axis[1].m_y;
    auto Tz = mouse_move.m_x * m_hv_axis[0].m_z + mouse_move.m_y * m_hv_axis[1].m_z;

    m_position.m_x += Tx;
    m_position.m_y += Ty;
    m_position.m_z += Tz;

    m_object_model->m_30 = m_position.m_x;
    m_object_model->m_31 = m_position.m_y;
    m_object_model->m_32 = m_position.m_z;
}

float Gizmo::calc_length(float p_)
{
    float length = p_+m_coordinates[0].m_x+m_average_dist+(m_uniform_scale*0.7f);
    return length;
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
    m_position.m_x = x_;
    m_position.m_y = y_;
    m_position.m_z = z_;
}

void Gizmo::make_moveable(mc::Transform &transform_)
{
    m_object_model = &transform_;
    setPosition(m_object_model->m_30,m_object_model->m_31,m_object_model->m_32);
}

void Gizmo::show()
{
    m_display = true;
}

void Gizmo::hide()
{
    m_display = false;
}

bool Gizmo::is_enabled()
{
    return m_display;
}

bool Gizmo::is_selected()
{
    return (m_handle_selected != Gizmo::Handle::NONE);
}

void Gizmo::deselect()
{
    m_handle_selected = Gizmo::Handle::NONE;
}

void Gizmo::findSelectedHandle(const mc::Ray &mouse_ray_)
{
    m_handle_selected = Handle::NONE;
    if (clickedOn<Handle::MID>(mouse_ray_))
    {
        m_handle_selected = Handle::MID;
        return;
    }
//    if (clickedOn<Handle::X>(mouse_ray_))
//    {
//        m_handle_selected = Handle::X;
//        return;
//    }
//    if (clickedOn<Handle::Y>(mouse_ray_))
//    {
//        m_handle_selected = Handle::Y;
//        return;
//    }
//    if (clickedOn<Handle::Z>(mouse_ray_))
//    {
//        m_handle_selected = Handle::Z;
//        return;
//    }
}

void Gizmo::drag_on_axis(const mc::V2 &mouse_drag_)
{
    float dist_from_cam = camera.getLookAt().calcDist()*0.002f;
    mouse_move = mouse_drag_ * dist_from_cam;
    m_hv_axis = camera.calc_local_HV_axis();

    switch(m_handle_selected)
    {
        case Handle::X:
            dragOn<Handle::X>();
            break;

        case Handle::Y:
            dragOn<Handle::Y>();
            break;

        case Handle::Z:
            dragOn<Handle::Z>();
            break;

        case Handle::MID:
            dragOn<Handle::MID>();
            break;

        default:
            break;
    }
}

void Gizmo::draw()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    m_average_dist = ((view.m_30+view.m_31+view.m_32)/3)*(-0.25f);
    m_uniform_scale = m_average_dist*0.25f;

    m_model.identity();
    {
        m_model.scale(m_average_dist,m_average_dist,m_average_dist);
        m_model.translate(m_position.m_x,m_position.m_y,m_position.m_z);
        loadShader();
        m_vao->bind();
        m_vao->draw();
        m_vao->unbind();
    }

    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    m_model.identity();
    {
        m_orientation.identity();
        m_orientation.rotateY(90.f);
        m_model = m_uniform_scale * m_orientation;
        m_model.translate(m_position.m_x+m_average_dist,m_position.m_y,m_position.m_z);
        loadShader(mc::Colour(1.f,0.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        m_orientation.identity();
        m_orientation.rotateX(-90.f);
        m_model = m_uniform_scale * m_orientation;
        m_model.translate(m_position.m_x,m_position.m_y+m_average_dist,m_position.m_z);
        loadShader(mc::Colour(0.f,1.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        m_model.scale(m_uniform_scale,m_uniform_scale,m_uniform_scale);
        m_model.translate(m_position.m_x,m_position.m_y,m_position.m_z+m_average_dist);
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

        m_orientation = Rx * Ry;

        m_model = m_uniform_scale * (m_orientation * Rz);
        m_model.translate(m_position.m_x,m_position.m_y,m_position.m_z);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        loadShader(mc::Colour(1.f,1.f,0.f,1.f));
        prim->draw("central");
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

Gizmo::~Gizmo() noexcept
{
    m_object_model = nullptr;
}
