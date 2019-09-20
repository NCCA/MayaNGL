
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
bool Gizmo::clicked_on<Gizmo::Handle::X>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[1]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_x < calc_length(m_position.m_x)))
        return true;
    return false;
}

template<>
bool Gizmo::clicked_on<Gizmo::Handle::Y>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[3]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_y < calc_length(m_position.m_y)))
        return true;
    return false;
}

template<>
bool Gizmo::clicked_on<Gizmo::Handle::Z>(const mc::Ray &mouse_ray_)
{
    mc::Ray handle{m_position,m_coordinates[5]};
    auto poi = mc::intersect(mouse_ray_,handle);
    if ((poi != mc::failed) && (poi.m_z < calc_length(m_position.m_z)))
        return true;
    return false;
}

template<>
bool Gizmo::clicked_on<Gizmo::Handle::MID>(const mc::Ray &mouse_ray_)
{
    auto &&direction = camera.get_inv_direction();
    float size = m_uniform_scale*0.6f;
    mc::Plane<false> handle = {m_position,direction,mc::V2(size,size),m_orientation};

    auto poi = mc::intersect(mouse_ray_,handle);
    if (poi != mc::failed)
        return true;
    return false;
}

template<>
void Gizmo::dragged_on<Gizmo::Handle::X>()
{
    auto Tx = mouse_move.m_x * m_hv_axis[0].m_x + mouse_move.m_y * m_hv_axis[1].m_x;
    m_position.m_x += Tx;
    m_object_model->m_30 = m_position.m_x;
}

template<>
void Gizmo::dragged_on<Gizmo::Handle::Y>()
{
    auto Ty = mouse_move.m_y * m_hv_axis[1].m_y;
    m_position.m_y += Ty;
    m_object_model->m_31 = m_position.m_y;
}

template<>
void Gizmo::dragged_on<Gizmo::Handle::Z>()
{
    auto Tz = mouse_move.m_x * m_hv_axis[0].m_z + mouse_move.m_y * m_hv_axis[1].m_z;
    m_position.m_z += Tz;
    m_object_model->m_32 = m_position.m_z;
}

template<>
void Gizmo::dragged_on<Gizmo::Handle::MID>()
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

void Gizmo::load_shader() const
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(mc::axis_shader);

    auto MVP = projection * view * m_model;

    shader->setUniform("MVP",MVP);
}

void Gizmo::load_shader(mc::Colour &&colour_) const
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

void Gizmo::make_moveable(mc::Transform &transform_)
{
    // I may have to store a list of pointers to indiv model matrices.
    m_object_model = &transform_;
    m_position.m_x = m_object_model->m_30;
    m_position.m_y = m_object_model->m_31;
    m_position.m_z = m_object_model->m_32;
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

void Gizmo::find_selected_handle(const mc::Ray &mouse_ray_)
{
    m_handle_selected = Handle::NONE;
    if (clicked_on<Handle::MID>(mouse_ray_))
    {
        m_handle_selected = Handle::MID;
        return;
    }
    if (clicked_on<Handle::X>(mouse_ray_))
    {
        m_handle_selected = Handle::X;
        return;
    }
    if (clicked_on<Handle::Y>(mouse_ray_))
    {
        m_handle_selected = Handle::Y;
        return;
    }
    if (clicked_on<Handle::Z>(mouse_ray_))
    {
        m_handle_selected = Handle::Z;
        return;
    }
}

void Gizmo::dragged_on_axis(const mc::V2 &mouse_drag_)
{
    float dist_from_cam = camera.get_lookAt().calc_dist()*0.002f;
    mouse_move = mouse_drag_ * dist_from_cam;
    m_hv_axis = camera.calc_local_hv_axis();

    switch(m_handle_selected)
    {
        case Handle::X:
            dragged_on<Handle::X>();
            break;

        case Handle::Y:
            dragged_on<Handle::Y>();
            break;

        case Handle::Z:
            dragged_on<Handle::Z>();
            break;

        case Handle::MID:
            dragged_on<Handle::MID>();
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
        load_shader();
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
        load_shader(mc::Colour(1.f,0.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        m_orientation.identity();
        m_orientation.rotateX(-90.f);
        m_model = m_uniform_scale * m_orientation;
        m_model.translate(m_position.m_x,m_position.m_y+m_average_dist,m_position.m_z);
        load_shader(mc::Colour(0.f,1.f,0.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        m_model.scale(m_uniform_scale,m_uniform_scale,m_uniform_scale);
        m_model.translate(m_position.m_x,m_position.m_y,m_position.m_z+m_average_dist);
        load_shader(mc::Colour(0.f,0.f,1.f,1.f));
        prim->draw("arrow_head");
    }

    m_model.identity();
    {
        auto &&direction = camera.get_inv_direction();
        auto &&shadow = camera.get_inv_shadow();

        mc::Rotation Rz;
        Rz.rotateZ(45.f);

        auto theta = atan2(direction.m_x,direction.m_z);
        mc::Rotation Ry = mc::y_matrix(-theta);

        auto rotation_axis = camera.get_up().cross(shadow);
        rotation_axis.normalize();
        auto phi = atan2(rotation_axis.dot(direction.cross(shadow)) , direction.dot(shadow));
        mc::Rotation Rx = mc::axis_matrix(phi,rotation_axis);

        m_orientation = Rx * Ry;

        m_model = m_uniform_scale * (m_orientation * Rz);
        m_model.translate(m_position.m_x,m_position.m_y,m_position.m_z);

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        load_shader(mc::Colour(1.f,1.f,0.f,1.f));
        prim->draw("central");
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

Gizmo::~Gizmo() noexcept
{
    m_object_model = nullptr;
}
