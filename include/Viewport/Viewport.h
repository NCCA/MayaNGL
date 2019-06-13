#pragma once

#include "Viewport/Camera.h"
#include "Viewport/Mouse.h"
#include "Viewport/ProjectionText.h"
#include "Viewport/ViewAxis.h"
#include <QKeyEvent>
#include <QMouseEvent>


class Viewport
{
    private:
        typedef ngl::Transformation Transform;
        typedef ngl::Mat4 ViewMat;
        typedef ngl::Mat4 ProjectionMat;

    private:
        const int &screenWidth;
        const int &screenHeight;
        ViewMat &view;
        ViewMat orig_view;
        ProjectionMat &projection;

    private:
        float m_aspectRatio;
        float m_ortho_zoom;
        Mouse m_mouse;
        Camera m_camera;
        ProjectionText m_projText;
        Transform m_model;
        ViewAxis m_axis;

    private:
        void loadLineColourShader();
        ProjectionMat goPersp();
        ProjectionMat goOrtho();

    public:
        explicit Viewport( const int &screenWidth_,
                           const int &screenHeight_,
                           ngl::Mat4 &view_,
                           ngl::Mat4 &projection_ );

        GET_MEMBER(m_camera,Camera)

        void initialize();
        void resize();
        void update_draw();
        void keyPress(QKeyEvent *event_);
        void mousePress(QMouseEvent *event_);
        void mouseMove(QMouseEvent *event_);

        ~Viewport() noexcept = default;
};
