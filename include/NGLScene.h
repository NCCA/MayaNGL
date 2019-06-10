#pragma once

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include "Camera/Camera.h"
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QMouseEvent>


class NGLScene : public QOpenGLWindow
{
    Q_OBJECT

    private:
        ngl::Transformation m_model;
        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;

        Camera m_camera;
        std::unique_ptr<ngl::Text> m_projection_title;

        void loadDiffuseShader();
        void loadLineColourShader();

        void keyPressEvent(QKeyEvent *event_ = nullptr) override;
        void mousePressEvent(QMouseEvent *event_ = nullptr) override;
        void mouseMoveEvent(QMouseEvent *event_ = nullptr) override;

    public:
        NGLScene();

        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w_, int h_) override;

        ~NGLScene() override = default;
};
