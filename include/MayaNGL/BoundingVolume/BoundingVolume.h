#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


namespace ngl
{
    class Obj;
}


template<typename BV>
class BoundingVolume;

template<>
class BoundingVolume<mc::Sphere>
{
    private:
        mc::Sphere bv;
        mc::Transform transform;
        mc::Position local_centre;

    public:
        GET_MEMBER(bv,volume)

        template<typename PRIM>
        void compute_volume(const PRIM &, const mc::Transform &transform_, std::false_type)
        {
            update(transform_);
        }

        template<typename PRIM>
        void compute_volume(const PRIM &prim_, const mc::Transform &transform_, std::true_type)
        {
            auto vtx_list = prim_->getVertexList();

            mc::Position max_x;
            for (auto itr=std::cbegin(vtx_list); itr!=std::cend(vtx_list); ++itr)
            {
                auto &&vtx = *itr;

                if(max_x.m_x < vtx.m_x)
                    max_x = vtx;
            }
            std::cout<< max_x <<std::endl;
        }

        template<typename PRIM>
        void compute_volume(PRIM &&prim_, const mc::Transform &transform_)
        {
            typedef typename std::remove_reference<PRIM>::type nRType;
            typedef typename std::remove_const<nRType>::type nCType;
            typedef typename mc::remove_smart_ptr<nCType>::type PrimType;

            this->compute_volume(std::forward<PRIM>(prim_),transform_,std::is_same<PrimType,ngl::Obj>());
        }

        void update(const mc::Transform &transform_)
        {
            bv.position = local_centre + mc::Position{transform_.m_30,transform_.m_31,transform_.m_32};
            bv.radius = /*add local rad*/ std::max({transform_.m_00,transform_.m_11,transform_.m_22});

            transform.m_30 = bv.position.m_x;
            transform.m_31 = bv.position.m_y;
            transform.m_32 = bv.position.m_z;

            transform.m_00 = bv.radius;
            transform.m_11 = bv.radius;
            transform.m_22 = bv.radius;
        }

        void initialize()
        {
            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
            prim->createSphere("bv_sphere",1.f,20);
        }

        void draw(const mc::Model &model_, const mc::View &view_, const mc::Projection &projection_)
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(ngl::nglColourShader);
            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

            update(model_);

            auto MVP = projection_ * view_ * transform;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,0.263f,0.639f,1.f));
            prim->draw("bv_sphere");

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
};
