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
        enum {left,right,bottom,top,back,front,num_of_sides};
        mc::Sphere bv;
        mc::Transform transform;
        mc::Position local_centre;

        typedef std::array<mc::Position,num_of_sides> MaxDimensions;
        MaxDimensions dimensions;

    public:
        GET_MEMBER(bv,volume)

        template<typename PRIM>
        void compute_volume(const PRIM &, const mc::Transform &transform_, std::false_type)
        {
            update(transform_);
        }


        void tight(const mc::Position &vtx_, mc::Position &orig_, std::size_t axis_)
        {
            if (vtx_ != orig_)
            {
                if (std::abs(orig_[axis_]-vtx_[axis_]) < std::numeric_limits<float>::epsilon())
                    orig_ = (orig_ + vtx_) * 0.5f;
            }
        }

        mc::Position pointOnSphere(float radius_, mc::V2 &&coordinates_)
        {
            float fi = 2.f * M_PI * coordinates_.m_x;
            float theta = 2.f * M_PI * coordinates_.m_y;

            mc::Position poc;
            poc.m_x = radius_ * sin(fi) * cos(theta);
            poc.m_y = radius_ * sin(fi) * sin(theta);
            poc.m_z = radius_ * cos(fi);

            return poc;
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

            // maybe not needed.
            for (auto itr=std::cbegin(vtx_list); itr!=std::cend(vtx_list); ++itr)
            {
                auto &&vtx = *itr;

                tight(vtx,dimensions[left],0);
                tight(vtx,dimensions[right],0);
                tight(vtx,dimensions[bottom],1);
                tight(vtx,dimensions[top],1);
                tight(vtx,dimensions[back],2);
                tight(vtx,dimensions[front],2);
            }

            mc::Position centre;
            centre.m_x = (dimensions[left].m_x + dimensions[right].m_x) * 0.5f;
            centre.m_y = (dimensions[bottom].m_y + dimensions[top].m_y) * 0.5f;
            centre.m_z = (dimensions[back].m_z + dimensions[front].m_z) * 0.5f;

            mc::Position left_poc = centre + pointOnSphere((dimensions[left]-centre).length(), {0.75f,0.f});
            mc::Position right_poc = centre + pointOnSphere((dimensions[right]-centre).length(), {0.25f,0.f});

            mc::Position bottom_poc = centre + pointOnSphere((dimensions[bottom]-centre).length(), {0.75f,0.25f});
            mc::Position top_poc = centre + pointOnSphere((dimensions[top]-centre).length(), {0.25f,0.25f});

            mc::Position back_poc = centre + pointOnSphere((dimensions[back]-centre).length(), {0.5f,0.f});
            mc::Position front_poc = centre + pointOnSphere((dimensions[front]-centre).length(), {0.f,0.f});

            dimensions[left] = left_poc;
            dimensions[right] = right_poc;
            dimensions[bottom] = bottom_poc;
            dimensions[top] = top_poc;
            dimensions[back] = back_poc;
            dimensions[front] = front_poc;

            mc::Position average = std::accumulate(std::begin(dimensions),std::end(dimensions),mc::Position::zero()) / num_of_sides;
            std::cout<< average <<std::endl;

//            mc::Position average = std::accumulate(std::begin(dimensions),std::end(dimensions),mc::Position::zero()) / num_of_sides;
//            std::sort(dimensions.begin(),dimensions.end(),[&average](auto &&v1_,auto &&v2_) -> bool
//                                                          {
//                                                              float v1_dist = (v1_-average).lengthSquared();
//                                                              float v2_dist = (v2_-average).lengthSquared();
//                                                              return (v1_dist > v2_dist);
//                                                          });

//            for (auto &&o : dimensions)
//                std::cout<< o <<std::endl;


//            float radius = (*dimensions.begin()-centre).length();
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

//            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

//            update(model_);

//            auto MVP = projection_ * view_ * transform;
//            shader->setUniform("MVP",MVP);
//            shader->setUniform("Colour",ngl::Vec4(1.f,0.263f,0.639f,1.f));
//            prim->draw("bv_sphere");

//            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

            mc::Transform PT;
            PT.scale(0.025f,0.025f,0.025f);

            auto point = dimensions[left];
            PT.translate(point.m_x,point.m_y,point.m_z);
            auto MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,0.f,1.f,1.f));
            prim->draw("bv_sphere");

            point = dimensions[right];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,1.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = dimensions[bottom];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,1.f,1.f,1.f));
            prim->draw("bv_sphere");

            point = dimensions[top];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,0.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = dimensions[back];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,0.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = dimensions[front];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,1.f,0.f,1.f));
            prim->draw("bv_sphere");

        }
};
