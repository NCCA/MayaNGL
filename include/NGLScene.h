#pragma once

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include "MayaNGL/MayaNGL_Def.hpp"


class NGLScene : public QOpenGLWindow
{
    Q_OBJECT

    private:
        ngl::Mat4 m_model;
        ngl::Transformation m_transform;

        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;

        MayaNGL m_maya;

    private:
        void loadDiffuseShader(const ngl::Mat4 &mat_);
        void keyPressEvent(QKeyEvent *event_ = nullptr) override;
        void mousePressEvent(QMouseEvent *event_ = nullptr) override;
        void mouseMoveEvent(QMouseEvent *event_ = nullptr) override;

    public:
        NGLScene();

        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w_, int h_) override;

        ~NGLScene() noexcept override = default;
};
