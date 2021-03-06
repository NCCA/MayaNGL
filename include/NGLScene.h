#pragma once

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Obj.h>
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include "MayaNGL/MayaNGL_Def.hpp"


class NGLScene : public QOpenGLWindow
{
    Q_OBJECT

    private:
        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;

        // Used Mat4 and Transformation for demonstration
        // purposes. However, movable objects must provide
        // a Mat4 model matrix.
        ngl::Mat4 m_teapot_model;
        ngl::Mat4 m_fish_model;
        ngl::Transformation m_transform;

        // Used two ngl::Obj objects (ptr and concrete) for
        // demonstration purposes. Only one is needed.
        std::unique_ptr<ngl::Obj> m_fish_mesh;
        ngl::Obj m_airplane_mesh;

        MayaNGL m_maya;

    private:
        template<bool using_texture = false>
        void loadShader(const ngl::Mat4 &mat_);

        void keyPressEvent(QKeyEvent *event_ = nullptr) override;
        void mousePressEvent(QMouseEvent *event_ = nullptr) override;
        void mouseMoveEvent(QMouseEvent *event_ = nullptr) override;
        void mouseReleaseEvent(QMouseEvent *event_ = nullptr) override;

    public:
        NGLScene();

        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w_, int h_) override;

        ~NGLScene() noexcept override = default;
};
