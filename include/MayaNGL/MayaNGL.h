#pragma once

#include "Mouse/Mouse.h"
#include "Viewport/Viewport_Def.hpp"
#include "MayaNGL/Gizmo/Gizmo.h"
#include "Selection/Select_Def.hpp"

#include <QKeyEvent>
#include <QMouseEvent>


class MayaNGL
{
    private:
        mc::View &view;
        mc::Projection &projection;

    private:
        mc::LookAt m_initial_lookAt;
        Mouse m_mouse;
        Camera m_camera;
        Viewport m_viewport;
        Select<disable_visualization> m_select;
        Gizmo m_gizmo;

    public:
        explicit MayaNGL( mc::View &view_,
                          mc::Projection &projection_ );

        GET_MEMBER(m_camera,camera)

        template<typename T, std::size_t N, template<typename,std::size_t> class CNT>
        void initialize(const CNT<T,N> &lookAt_);

        template<typename T, template<typename,typename = std::allocator<T>> class CNT>
        void initialize(const CNT<T> &lookAt_);

        template<typename T>
        void initialize(const T &eye_, const T &target_, const T &up_);

        template<typename PRIM, typename T, typename = std::enable_if_t<
                                                           std::is_same<
                                                               decltype(std::declval<T>().getMatrix()),mc::Transform
                                                                       >::value
                                                                       >>
        void make_selectable(std::size_t id_, PRIM &&prim_, T &transform_);

        template<typename PRIM>
        void make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_);


        template<typename PRIM, typename T, typename = std::enable_if_t<
                                                           std::is_same<
                                                               decltype(std::declval<T>().getMatrix()),mc::Transform
                                                                       >::value
                                                                       >>
        void make_selectable_and_movable(std::size_t id_, PRIM &&prim_, T &transform_);

        template<typename PRIM>
        void make_selectable_and_movable(std::size_t id_, PRIM &&prim_, mc::Transform &transform_);

        void initialize();
        void resize(int w_, int h_);
        void draw();
        void draw_gizmo();

        void key_press(QKeyEvent *event_);
        void mouse_press(QMouseEvent *event_);
        void mouse_move(QMouseEvent *event_);
        void mouse_release(QMouseEvent *event_);

        ~MayaNGL() noexcept = default;
};




