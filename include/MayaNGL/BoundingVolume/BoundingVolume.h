#pragma once

#include "MayaNGL/Common/Common_Def.hpp"
#include <glm/gtx/perpendicular.hpp>


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

        // AABB
        typedef std::array<mc::Position,num_of_sides> Dimensions;
        Dimensions aabb_dimensions;

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

        mc::Position point_on_sphere(const mc::Position &centre_, float radius_, mc::V2 &&coordinates_)
        {
            float fi = 2.f * M_PI * coordinates_.m_x;
            float theta = 2.f * M_PI * coordinates_.m_y;

            mc::Position poc;
            poc.m_x = centre_.m_x + radius_ * sin(fi) * cos(theta);
            poc.m_y = centre_.m_y + radius_ * sin(fi) * sin(theta);
            poc.m_z = centre_.m_z + radius_ * cos(fi);

            return poc;
        }

        int getSign(float val)
        {
            int sign = (0.0 < val) - (val < 0.0);
            return (sign == 0) ? 1 : sign;
        }

        mc::V2 calc_spherical_coordinates(const mc::Position &centre_, const mc::Position &point_)
        {
            mc::V2 coordinates;

            float radius = (point_-centre_).length();
            float phi = mc::round( acos(point_.m_z/radius) * getSign(point_.m_x) ,4);


            float theta = 0.f;
            if (phi == 0.f)
                theta = asin(point_.m_y/radius);
            else
                theta = asin((point_.m_y/radius)/sin(phi));


            coordinates.m_x = mc::round( (phi*0.5f)/M_PI, 4);
            coordinates.m_y = mc::round( (theta*0.5f)/M_PI, 4);
            return coordinates;
        }

        bool lies_on_sphere(const mc::Position &centre_, float radius_, const mc::Position &pnt_)
        {
            return ( mc::round((pow((pnt_.m_x-centre_.m_x),2) +
                                pow((pnt_.m_y-centre_.m_y),2) +
                                pow((pnt_.m_z-centre_.m_z),2)),4) == mc::round(pow(radius_,2),4) );
        }

        template<typename PRIM>
        void compute_volume(const PRIM &prim_, const mc::Transform &transform_, std::true_type)
        {
            auto vtx_list = prim_->getVertexList();

            for (auto itr=std::cbegin(vtx_list); itr!=std::cend(vtx_list); ++itr)
            {
                auto &&vtx = *itr;

                aabb_dimensions[left]   = std::max(aabb_dimensions[left], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_x>v2_.m_x);});
                aabb_dimensions[right]  = std::max(aabb_dimensions[right], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_x<v2_.m_x);});
                aabb_dimensions[bottom] = std::max(aabb_dimensions[bottom], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_y>v2_.m_y);});
                aabb_dimensions[top]    = std::max(aabb_dimensions[top], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_y<v2_.m_y);});
                aabb_dimensions[back]   = std::max(aabb_dimensions[back], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_z>v2_.m_z);});
                aabb_dimensions[front]  = std::max(aabb_dimensions[front], vtx, [](auto &&v1_,auto &&v2_){return (v1_.m_z<v2_.m_z);});
            }

            for (auto itr=std::cbegin(vtx_list); itr!=std::cend(vtx_list); ++itr)
            {
                auto &&vtx = *itr;

                tight(vtx,aabb_dimensions[left],0);
                tight(vtx,aabb_dimensions[right],0);
                tight(vtx,aabb_dimensions[bottom],1);
                tight(vtx,aabb_dimensions[top],1);
                tight(vtx,aabb_dimensions[back],2);
                tight(vtx,aabb_dimensions[front],2);
            }

            mc::Position aabb_centre;
            aabb_centre.m_x = (aabb_dimensions[left].m_x + aabb_dimensions[right].m_x) * 0.5f;
            aabb_centre.m_y = (aabb_dimensions[bottom].m_y + aabb_dimensions[top].m_y) * 0.5f;
            aabb_centre.m_z = (aabb_dimensions[back].m_z + aabb_dimensions[front].m_z) * 0.5f;



            mc::Position l(-1.f,0.f,0.f);   //-0.25 , 0
            mc::Position r(1.f,0.f,0.f);    // 0.25 , 0
            mc::Position b(0.f,-1.f,0.f);   // 0.25 ,-0.25
            mc::Position t(0.f,1.f,0.f);    // 0.25 , 0.25
            mc::Position bc(0.f,0.f,-1.f);  // 0.5  , 0
            mc::Position f(0.f,0.f,1.f);    // 0    , 0

            std::cout<< calc_spherical_coordinates(mc::Position::zero(),l) <<std::endl;
            std::cout<< calc_spherical_coordinates(mc::Position::zero(),r) <<std::endl;
            std::cout<< calc_spherical_coordinates(mc::Position::zero(),b) <<std::endl;
            std::cout<< calc_spherical_coordinates(mc::Position::zero(),t) <<std::endl;
            std::cout<< calc_spherical_coordinates(mc::Position::zero(),bc) <<std::endl;
            std::cout<< calc_spherical_coordinates(mc::Position::zero(),f) <<std::endl;


            auto p = mc::Position(-4,2,1);
            auto c = calc_spherical_coordinates(mc::Position::zero(),p);
            std::cout<< c <<std::endl;
            auto s = point_on_sphere(mc::Position::zero(), 1.f, std::move(c));
            std::cout<< s <<std::endl;



//            Dimensions spherical_points;
//            spherical_points[left]   = point_on_sphere(aabb_centre,(aabb_dimensions[left]-aabb_centre).length(), {-0.25f,0.f});
//            spherical_points[right]  = point_on_sphere(aabb_centre,(aabb_dimensions[right]-aabb_centre).length(), {0.25f,0.f});
//            spherical_points[bottom] = point_on_sphere(aabb_centre,(aabb_dimensions[bottom]-aabb_centre).length(), {0.25f,-0.25f});
//            spherical_points[top]    = point_on_sphere(aabb_centre,(aabb_dimensions[top]-aabb_centre).length(), {0.25f,0.25f});
//            spherical_points[back]   = point_on_sphere(aabb_centre,(aabb_dimensions[back]-aabb_centre).length(), {0.5f,0.f});
//            spherical_points[front]  = point_on_sphere(aabb_centre,(aabb_dimensions[front]-aabb_centre).length(), {0.f,0.f});

//            std::sort(spherical_points.begin(),spherical_points.end(),[&aabb_centre](auto &&v1_,auto &&v2_) -> bool
//                                                                      {
//                                                                          float v1_dist = (v1_-aabb_centre).lengthSquared();
//                                                                          float v2_dist = (v2_-aabb_centre).lengthSquared();
//                                                                          return (v1_dist > v2_dist);
//                                                                      });

//            float radius = (spherical_points[1]-spherical_points[0]).length() * 0.5f;
//            std::cout<< radius <<std::endl;

//            centre = glm::perp()


//            std::cout<< liesOnSphere(centre, radius, dimensions[1]) <<std::endl;



//            mc::Position average = std::accumulate(std::begin(dimensions),std::end(dimensions),mc::Position::zero()) / num_of_sides;


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

            auto point = aabb_dimensions[left];
            PT.translate(point.m_x,point.m_y,point.m_z);
            auto MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,0.f,1.f,1.f));
            prim->draw("bv_sphere");

            point = aabb_dimensions[right];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,1.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = aabb_dimensions[bottom];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,1.f,1.f,1.f));
            prim->draw("bv_sphere");

            point = aabb_dimensions[top];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,0.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = aabb_dimensions[back];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.f,0.f,0.f,1.f));
            prim->draw("bv_sphere");

            point = aabb_dimensions[front];
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,1.f,0.f,1.f));
            prim->draw("bv_sphere");

        }
};
