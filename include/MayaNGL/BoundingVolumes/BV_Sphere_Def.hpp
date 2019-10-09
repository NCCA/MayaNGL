#pragma once

#include "BV_Sphere.h"


template<typename PRIM>
void BoundingVolume<mc::Sphere, false>::compute_volume(const PRIM &prim_)
{
    const auto &vtx_list = prim_.getVertexList();
    calc_tight_bound_volume(vtx_list);
}

template <typename T, template<typename, typename = std::default_delete<T> > class SmPtr>
void BoundingVolume<mc::Sphere, false>::compute_volume(const SmPtr<T> &prim_)
{
    const auto &vtx_list = prim_->getVertexList();
    calc_tight_bound_volume(vtx_list);
}


//===================================================================//


template<typename PRIM>
void BoundingVolume<mc::Sphere, true>::update(const PRIM &prim_)
{
    auto &&prim_transform = prim_.get_transform();
    auto &&prim_bv = prim_.bv.get_volume();

    m_bv.centre = prim_bv.centre * prim_transform;
    m_bv.centre.m_x += prim_transform.m_30;
    m_bv.centre.m_y += prim_transform.m_31;
    m_bv.centre.m_z += prim_transform.m_32;
    m_bv.radius = prim_bv.radius * std::max({prim_transform.m_00,prim_transform.m_11,prim_transform.m_22});

    m_transform.m_30 = m_bv.centre.m_x;
    m_transform.m_31 = m_bv.centre.m_y;
    m_transform.m_32 = m_bv.centre.m_z;

    m_transform.m_00 = m_bv.radius;
    m_transform.m_11 = m_bv.radius;
    m_transform.m_22 = m_bv.radius;
}

template<typename PRIM>
void BoundingVolume<mc::Sphere, true>::draw(const PRIM &prim_, const mc::View &view_, const mc::Projection &projection_)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use(ngl::nglColourShader);
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    update(prim_);
    auto MVP = projection_ * view_ * m_transform;
    shader->setUniform("MVP",MVP);
    shader->setUniform("Colour",ngl::Vec4(1.f,0.263f,0.639f,1.f));
    prim->draw("bv_sphere");
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


