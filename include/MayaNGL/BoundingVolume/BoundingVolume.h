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
        typedef std::array<mc::Position,6> MaxDimensions;
        enum {left,right,bottom,top,back,front};
        mc::Sphere bv;
        mc::Transform transform;
        mc::Position local_centre;
        MaxDimensions dimensions;

    private:
        inline mc::Position centre_pivot()
        {
            return mc::Position((dimensions[left].m_x+dimensions[right].m_x)*0.5f,
                                (dimensions[bottom].m_y+dimensions[top].m_y)*0.5f,
                                (dimensions[back].m_z+dimensions[front].m_z)*0.5f);
        }

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

            for (auto itr=std::cbegin(vtx_list); itr!=std::cend(vtx_list); ++itr)
            {
                auto &&vtx = *itr;

                dimensions[left] = std::max(dimensions[left], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_x>v2_.m_x);});
                dimensions[right] = std::max(dimensions[right], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_x<v2_.m_x);});

                dimensions[bottom] = std::max(dimensions[bottom], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_y>v2_.m_y);});
                dimensions[top] = std::max(dimensions[top], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_y<v2_.m_y);});

                dimensions[back] = std::max(dimensions[back], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_z>v2_.m_z);});
                dimensions[front] = std::max(dimensions[front], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_z<v2_.m_z);});
            }

            std::cout<< (dimensions[left]+dimensions[right])*0.5f <<std::endl;
            std::cout<< (dimensions[bottom]+dimensions[top])*0.5f <<std::endl;
            std::cout<< (dimensions[back]+dimensions[front])*0.5f <<std::endl;

            std::cout<< std::accumulate(dimensions.begin(),dimensions.end(),mc::Position()) / dimensions.size() <<std::endl;

//            mc::Position centre = centre_pivot();
//            std::cout<< centre <<std::endl;
//            std::sort(dimensions.begin(),dimensions.end(),[&centre](auto &&v1_,auto &&v2_) -> bool
//                                                          {
//                                                              float v1_dist = (v1_-centre).lengthSquared();
//                                                              float v2_dist = (v2_-centre).lengthSquared();
//                                                              return (v1_dist <= v2_dist);
//                                                          });
//            float radius = (*dimensions.begin()-centre).length();
//            std::cout<< radius <<std::endl;
//            radius = (*(dimensions.end()-1)-centre).length();
//            std::cout<< radius <<std::endl;

//            bv.position = centre;
//            bv.radius = radius;
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
//            bv.position = local_centre + mc::Position{transform_.m_30,transform_.m_31,transform_.m_32};
//            bv.radius = /*add local rad*/ std::max({transform_.m_00,transform_.m_11,transform_.m_22});

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
