#pragma once


#include "MayaNGL/Mouse/Mouse.h"
//#include "Viewport/Viewport_Def.hpp"
//#include "Selection/Select_Def.hpp"

#include <QKeyEvent>
#include <QMouseEvent>


struct MayaNGL
{
    mc::View &view;
    mc::Projection &projection;

    Mouse m_mouse;
//        Viewport m_viewport;
//        Select<true> m_select; // decouple Selection from Viewport.

    explicit MayaNGL( mc::View &view_,
                      mc::Projection &projection_ );

    void initialize();
    void resize(int w_, int h_);
    void update_draw();

    void keyPress(QKeyEvent *event_);
    void mousePress(QMouseEvent *event_);
    void mouseMove(QMouseEvent *event_);

    ~MayaNGL() noexcept = default;
};




