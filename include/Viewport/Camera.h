#pragma once

#include "Viewport/Mouse.h"


class Camera
{
    private:
        typedef ngl::Vec3 Position;
        typedef ngl::Vec3 Direction;

    public:
        enum class View {PERSPECTIVE,FRONT,SIDE,TOP};

    private:
        const Mouse &mouse;

    private:
        Position m_position;
        Position m_lookAt;
        Direction m_inverse;
        Direction m_upVector;
        View m_currentView;

        Direction updateInverse()
        {
            m_inverse = m_position-m_lookAt;
            m_inverse.normalize();
            return m_inverse;
        }

        void updateUpVector() // needs work
        {
            bool is_perp = (fabs(m_upVector.dot(m_inverse))>=0.98f);
            if (is_perp)
            {
                auto side = m_upVector.cross(m_inverse);
                m_upVector = m_inverse.cross(side);
                m_upVector.normalize();
            }

            std::cout<< m_upVector <<std::endl;

        }

    public:
        explicit Camera(const Mouse &mouse_);

        GET_MEMBER(m_position,Position)
        GET_MEMBER(m_lookAt,LookAt)
        GET_MEMBER(m_upVector,UpVector)
        GET_MEMBER(m_currentView,CurrentView)

        void pan();
        void dolly();
        void track();
        void reset(Position &&pos_={28.f,21.f,28.f},View view_=View::PERSPECTIVE,Direction &&up_=ngl::Vec3::up());
        void focusOn(const Position &target_);
        void front();
        void side();
        void top();

        ~Camera() noexcept = default;
};
