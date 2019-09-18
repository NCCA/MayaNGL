#pragma once

#include "MayaNGL/Camera/Camera_Def.hpp"


class Gizmo
{
    private:
        enum class Handle {NONE,X,Y,Z,MID};

        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const Camera &camera;
        mc::Transform *m_object_model;

    private:
        mc::Position position;
        mc::Size<float> average_dist;
        mc::Size<float> uniform_scale;
        mc::Transform m_model;
        Vertices m_coordinates;
        VAOPtr m_vao;

        bool display = false;
        Handle handle_selected = Handle::NONE;

    private:
        void loadShader() const;
        void loadShader(mc::Colour &&colour_) const;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const Camera &camera_ );

        GET_MEMBER(m_object_model,ObjectTransform)

        void initialize();
        void setPosition(float x_, float y_, float z_);
        void draw();

        float calcHandleLenght(float p_)
        {
            float length = p_+m_coordinates[0].m_x+average_dist+(uniform_scale*0.7f);
            return length;
        }

        void make_moveable(mc::Transform &transform_)
        {
            m_object_model = &transform_;
            setPosition(m_object_model->m_30,m_object_model->m_31,m_object_model->m_32);
        }

        void show()
        {
            display = true;
        }

        void hide()
        {
            display = false;
        }

        bool is_enabled()
        {
            return display;
        }

        bool is_selected()
        {
            return (handle_selected != Gizmo::Handle::NONE);
        }

        void deselect()
        {
            handle_selected = Gizmo::Handle::NONE;
        }

        void findSelectedHandle(const mc::Ray &mouseRay_)
        {
            handle_selected = Handle::NONE;
            if (clickedOnX(mouseRay_))
            {
                std::cout<< "Clicked on X" <<std::endl;
                handle_selected = Handle::X;
            }
            if (clickedOnY(mouseRay_))
            {
                std::cout<< "Clicked on Y" <<std::endl;
                handle_selected = Handle::Y;
            }
            if (clickedOnZ(mouseRay_))
            {
                std::cout<< "Clicked on Z" <<std::endl;
                handle_selected = Handle::Z;
            }
        }

        bool clickedOnX(const mc::Ray &mouseRay_)
        {
            mc::Ray handle{position,ngl::Vec3::right()};
            auto poi = mc::intersect(mouseRay_,handle);
            if ((poi != mc::failed) && (poi.m_x < calcHandleLenght(position.m_x)))
                return true;
            return false;
        }

        bool clickedOnY(const mc::Ray &mouseRay_)
        {
            mc::Ray handle{position,ngl::Vec3::up()};
            auto poi = mc::intersect(mouseRay_,handle);
            if ((poi != mc::failed) && (poi.m_y < calcHandleLenght(position.m_y)))
                return true;
            return false;
        }

        bool clickedOnZ(const mc::Ray &mouseRay_)
        {
            mc::Ray handle{position,ngl::Vec3::in()};
            auto poi = mc::intersect(mouseRay_,handle);
            if ((poi != mc::failed) && (poi.m_z < calcHandleLenght(position.m_z)))
                return true;
            return false;
        }

        void dragOnX(float x_)
        {
            position.m_x += x_*0.08f;
            m_object_model->m_30 = position.m_x;
        }

        void dragOnY(float y_)
        {
            position.m_y -= y_*0.08f;
            m_object_model->m_31 = position.m_y;
        }

        void dragOnZ(float z_)
        {
            position.m_z -= z_*0.08f;
            m_object_model->m_32 = position.m_z;
        }

        void dragOnAxis(const mc::V2 &drag_)
        {
            switch(handle_selected)
            {
                case Handle::X:
                    dragOnX(drag_.m_x);
                    break;

                case Handle::Y:
                    dragOnY(drag_.m_y);
                    break;

                case Handle::Z:
                    dragOnZ(drag_.m_x);
                    break;

                case Handle::MID:
                    break;

                default:
                    break;
            }
        }

        ~Gizmo() noexcept
        {
            m_object_model = nullptr;
        }
};



