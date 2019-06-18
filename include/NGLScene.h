#pragma once

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include "Viewport/Viewport_Def.hpp"


class NGLScene : public QOpenGLWindow
{
    Q_OBJECT

    private:
        ngl::Transformation m_model;
        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;

        Viewport m_viewport;

    private:
        void loadDiffuseShader();
        void keyPressEvent(QKeyEvent *event_ = nullptr) override;
        void mousePressEvent(QMouseEvent *event_ = nullptr) override;
        void mouseReleaseEvent(QMouseEvent *event_ = nullptr) override;
        void mouseMoveEvent(QMouseEvent *event_ = nullptr) override;

    public:
        NGLScene();

        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w_, int h_) override;

        ~NGLScene() noexcept override = default;
};
