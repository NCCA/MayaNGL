#pragma once

#include "Mouse/Mouse.h"
#include "Viewport/Viewport_Def.hpp"
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
        Select<true> m_select;

    public:
        explicit MayaNGL( mc::View &view_,
                          mc::Projection &projection_ );

        GET_MEMBER(m_camera,Camera)

        template<typename T, std::size_t N, template<typename,std::size_t> class CNT>
        void initialize(const CNT<T,N> &lookAt_);

        template<typename T, template<typename,typename = std::allocator<T>> class CNT>
        void initialize(const CNT<T> &lookAt_);

        template<typename T>
        void initialize(const T &eye_, const T &target_, const T &up_);

        template<typename PRIM, typename T, typename =  std::enable_if_t<
                                                            std::is_same<
                                                                decltype(std::declval<T>().getMatrix()),mc::Transform
                                                                        >::value
                                                                        >>
        void make_selectable(std::size_t id_, PRIM &&prim_, T &transform_);

        template<typename PRIM>
        void make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_);

        void initialize();
        void resize(int w_, int h_);
        void draw();
        void draw_gizmos();

        void keyPress(QKeyEvent *event_);
        void mousePress(QMouseEvent *event_);
        void mouseMove(QMouseEvent *event_);

        ~MayaNGL() noexcept = default;
};




