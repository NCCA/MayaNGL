#pragma once

//#include <type_traits>
//#include "Camera/Camera_Def.hpp"
//#include "ProjectionText/ProjectionText.h"
//#include "ViewAxis/ViewAxis.h"
//#include "Grid/Grid_Def.hpp"


class Viewport
{
//    private:
//        vc::View &view;
//        vc::Projection &projection;

//    private:
//        float m_aspectRatio;
//        float m_orthographic_zoom;

//        LookAt m_initial_lookAt;
//        vc::View m_initial_view;

////        Mouse m_mouse;
//        Camera m_camera;
//        Grid m_grid;
//        ProjectionText m_projText;
//        ViewAxis m_axis;

//    private:
//        void goPersp();
//        void goOrtho();

//    public:
//        explicit Viewport( vc::View &view_,
//                           vc::Projection &projection_ );

//        GET_MEMBER(m_camera,Camera)

//        template<typename T>
//        void initialize(const T &from_, const T &to_, const T &up_);

//        template< typename T, std::size_t N, template<typename,std::size_t> class CNT >
//        void initialize(const CNT<T,N> &lookAt_);

//        template< typename T, template<typename,typename = std::allocator<T>> class CNT >
//        void initialize(const CNT<T> &lookAt_);

//        template<typename PRIM>
//        void make_selectable(std::size_t id_, PRIM &&prim_, const vc::Transform &transform_);

//        template<typename PRIM, typename T, typename =  std::enable_if_t<
//                                                            std::is_same<
//                                                                decltype(std::declval<T>().getMatrix()),vc::Transform
//                                                                        >::value
//                                                                        >>
//        void make_selectable(std::size_t id_, PRIM &&prim_, /*const*/ T &transform_);

//        void initialize();
//        void resize(int w_, int h_);
//        void update_draw();

//        void keyPress(QKeyEvent *event_);
//        void mousePress(QMouseEvent *event_);
//        void mouseMove(QMouseEvent *event_);

//        ~Viewport() noexcept = default;
};
