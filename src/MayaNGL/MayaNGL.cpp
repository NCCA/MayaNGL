
#include "MayaNGL/MayaNGL.h"


MayaNGL::MayaNGL( mc::View &view_,
                  mc::Projection &projection_ )
                  :
                  view(view_),
                  projection(projection_),
                  m_initial_lookAt(),
                  m_mouse(),
                  m_camera(m_mouse,m_initial_lookAt),
                  m_viewport(view,projection,m_camera),
                  m_select(view,projection,m_camera),
                  m_gizmo(view,projection,m_camera)
{;}

void MayaNGL::initialize()
{
    mc::initialize_additional_shaders();
    m_viewport.initialize();
    m_gizmo.initialize();
    m_select.initialize();
    m_viewport.set_view<mc::CamView::PERSPECTIVE>();
}

void MayaNGL::resize(int w_, int h_)
{
    m_viewport.resize(w_,h_);
    m_select.resize(w_,h_);
}

void MayaNGL::draw()
{
    m_viewport.update_draw();
    m_select.draw();
}

void MayaNGL::draw_gizmo()
{
    if (m_gizmo.is_enabled())
        m_gizmo.draw();
}

void MayaNGL::key_press(QKeyEvent *event_)
{
    switch ( event_->key() )
    {
        case Qt::Key_F:
            m_mouse.reset();
            m_camera.focus(m_select);
            break;

        case Qt::Key_Space:
            m_mouse.reset();
            m_camera.reset(m_initial_lookAt);
            m_viewport.set_view<mc::CamView::PERSPECTIVE>();
            break;

        case Qt::Key_1:
            m_mouse.reset();
            m_camera.front();
            m_viewport.set_view<mc::CamView::FRONT>();
            break;

        case Qt::Key_2:
            m_mouse.reset();
            m_camera.side();
            m_viewport.set_view<mc::CamView::SIDE>();
            break;

        case Qt::Key_3:
            m_mouse.reset();
            m_camera.top();
            m_viewport.set_view<mc::CamView::TOP>();
            break;

        default:
            break;
    }
}

void MayaNGL::mouse_press(QMouseEvent *event_)
{
    bool lmb = (event_->button() == Qt::LeftButton);
    if (lmb)
    {
        m_mouse.set_anchor(event_->x(),event_->y());

        bool alt = (event_->modifiers() & Qt::AltModifier);
        if (!alt)
        {
            m_select.emit_ray(event_->x(),event_->y());
            if (m_gizmo.is_enabled())
                m_gizmo.find_selected_handle(m_select.get_ray());
        }
    }
}

void MayaNGL::mouse_move(QMouseEvent *event_)
{
    m_mouse.set_transform(event_->x(),event_->y());

    bool alt = (event_->modifiers() & Qt::AltModifier);
    if (alt)
    {
        switch(event_->buttons())
        {
            case Qt::LeftButton:
                if (m_camera.get_current_view() == mc::CamView::PERSPECTIVE)
                    m_camera.pan();
                break;

            case Qt::MiddleButton:
                m_camera.track();
                break;

            case Qt::RightButton:
                m_camera.dolly();
                if (m_camera.get_current_view() != mc::CamView::PERSPECTIVE)
                    m_viewport.ortho_zoom(m_mouse.get_drag());
                break;

            default:
                break;
        }
    }
    else
    {
        if (m_gizmo.is_selected())
        {
            m_gizmo.dragged_on_axis(m_mouse.get_drag());
            auto last_elem = m_select.get_currently_selected().back();
            m_select.set_primitive_transform(last_elem,*m_gizmo.get_object_transform());
        }
    }
}

void MayaNGL::mouse_release(QMouseEvent *event_)
{
    bool alt = (event_->modifiers() & Qt::AltModifier);
    bool lmb = (event_->button() == Qt::LeftButton);

    if (!alt && lmb)
    {
        bool shft = (event_->modifiers() == Qt::ShiftModifier);
        if(shft)
            m_select.enable_multi_selection();

        if (!m_gizmo.is_selected())
        {
            m_gizmo.hide();
            m_select.pick();
        }

        m_gizmo.deselect();
        if (!m_select.get_currently_selected().empty())
        {
            auto last_elem = m_select.get_currently_selected().back();
            auto is_selected_moveable = m_select.get_all_selectables().at(last_elem).get_is_moveable();
            if (is_selected_moveable)
                m_gizmo.show();
        }
    }
}


