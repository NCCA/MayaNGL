#pragma once

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <QOpenGLWindow>
#include <QKeyEvent>
#include <QMouseEvent>


class NGLScene : public QOpenGLWindow
{
    Q_OBJECT

    private:
        int width,height;

        ngl::Transformation m_transform;
        ngl::Mat4 m_view;
        ngl::Mat4 m_projection;

        void loadMatricesToShader();
        void keyPressEvent(QKeyEvent *_event) override;
//        void mouseMoveEvent(QMouseEvent *_event) override;
//        void mousePressEvent(QMouseEvent *_event) override;
//        void mouseReleaseEvent(QMouseEvent *_event) override;
//        void wheelEvent(QWheelEvent *_event) override;

    public:
        NGLScene();

        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int _w, int _h) override;

        ~NGLScene() override = default;
};
