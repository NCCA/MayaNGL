#pragma once

#include "Viewport/Camera.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOFactory.h>
#include <ngl/Quaternion.h>
#include <ngl/NGLStream.h>


struct ViewAxis
{
    typedef ngl::Transformation Transform;
    typedef ngl::Mat4 ViewMat;
    typedef ngl::Mat4 ProjectionMat;

    const ViewMat &scene_view;

    Transform m_model;
    ViewMat view;
    ProjectionMat projection;
    std::array<ngl::Vec3,6> m_coordinates;
    std::unique_ptr<ngl::AbstractVAO> m_vao;

    ViewAxis( const ViewMat &scene_view_ )
              :
              scene_view(scene_view_),
              m_model(),
              view(ngl::lookAt(ngl::Vec3::in(),ngl::Vec3::zero(),ngl::Vec3::up())),
              projection(),
              m_coordinates{{
                                ngl::Vec3::zero(),
                                ngl::Vec3::right(),
                                ngl::Vec3::zero(),
                                ngl::Vec3::up(),
                                ngl::Vec3::zero(),
                                ngl::Vec3::in()
                           }},
              m_vao()
    {;}

    void initialize()
    {
        m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
        m_vao->bind();
        m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(ngl::Vec3),m_coordinates[0].m_x));
        m_vao->setNumIndices(m_coordinates.size());
        m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
        m_vao->unbind();
    }

    void resize(float aspectRatio_)
    {
        float zoom = 20.f;
        projection = ngl::ortho(-aspectRatio_*zoom,aspectRatio_*zoom,-zoom,zoom,0.1f,200.f);

        auto bl = ngl::Vec3(1.5f-(aspectRatio_*zoom),1.5f-zoom,0.f);
        m_model.setPosition(bl);
    }

    void loadLineColourShader()
    {
        ngl::ShaderLib *shader = ngl::ShaderLib::instance();
        shader->use("LineColour");

        auto rot = scene_view;
        rot.translate(0.f,0.f,0.f);
        auto MVP = projection * view * m_model.getMatrix() * rot;

        shader->setUniform("MVP",MVP);
        shader->setUniform("Dist",abs(view.m_32));
        shader->setUniform("enableViewAxisColours",true);
    }

    void draw()
    {
        loadLineColourShader();
        m_vao->bind();
        m_vao->draw();
        m_vao->unbind();
    }
};
