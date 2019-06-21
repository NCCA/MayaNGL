#pragma once

#include "Viewport/Camera/Camera.h"
#include "Viewport/ProjectionText/ProjectionText.h"
#include "Viewport/ViewAxis/ViewAxis.h"
#include "Viewport/Grid/Grid.h"
#include "Viewport/Select/Select_Def.hpp"
#include <QKeyEvent>
#include <QMouseEvent>


class Viewport
{
    private:
        vc::View &view;
        vc::Projection &projection;

    private:
        float m_aspectRatio;
        float m_orthographic_zoom;

        LookAt m_initial_lookAt;
        vc::View m_initial_view;

        Mouse m_mouse;
        Camera m_camera;
        Grid m_grid;
        ProjectionText m_projText;
        ViewAxis m_axis;
        Select<true> m_select;

    private:
        void goPersp();
        void goOrtho();

    public:
        explicit Viewport( vc::View &view_,
                           vc::Projection &projection_ );

        GET_MEMBER(m_camera,Camera)

        template<typename T>
        void initialize(const T &from_, const T &to_, const T &up_);

        template< typename T, std::size_t N, template<typename,std::size_t> class CNT >
        void initialize(const CNT<T,N> &lookAt_);

        template< typename T, template<typename,typename = std::allocator<T>> class CNT >
        void initialize(const CNT<T> &lookAt_);

        template<typename S>
        void make_selectable(std::size_t id_, S &&prim_name_, const vc::Transform &prim_transform_);

        void initialize();
        void resize(int w_, int h_);
        void update_draw();

        void keyPress(QKeyEvent *event_);
        void mousePress(QMouseEvent *event_);
        void mouseMove(QMouseEvent *event_);

        ~Viewport() noexcept = default;
};
