#pragma once

#include "MayaNGL/Common/Common_Def.hpp"
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/vector_angle.hpp>


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

        // AABB tbr
        typedef std::array<mc::Position,num_of_sides> Dimensions;
        Dimensions aabb_dimensions;
        mc::Position bisector; //tbr

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

        mc::Position point_on_sphere(const mc::Position &centre_, float radius_, const mc::V2 &coordinates_)
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
            float phi = acos(point_.m_z/radius)*getSign(point_.m_x);
            float theta = 0.f;
            if (mc::round(phi,4) == 0.f)
                theta = asin(point_.m_y/radius);
            else
                theta = asin((point_.m_y/radius)/sin(phi));

            coordinates.m_x = (phi*0.5f)/M_PI;
            coordinates.m_y = (theta*0.5f)/M_PI;

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

            // populate the AABB faces.
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

            // sort out multiple vtxs that are on the same max dist from the centre.
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

            // calculate the centre of the AABB.
            mc::Position centre;
            centre.m_x = (aabb_dimensions[left].m_x + aabb_dimensions[right].m_x) * 0.5f;
            centre.m_y = (aabb_dimensions[bottom].m_y + aabb_dimensions[top].m_y) * 0.5f;
            centre.m_z = (aabb_dimensions[back].m_z + aabb_dimensions[front].m_z) * 0.5f;

            // find the two most distant vtxs from the centre.
            std::sort(aabb_dimensions.begin(),aabb_dimensions.end(),[&centre](auto &&v1_,auto &&v2_) -> bool
                                                                    {
                                                                        float v1_dist = (v1_-centre).lengthSquared();
                                                                        float v2_dist = (v2_-centre).lengthSquared();
                                                                        return (v1_dist > v2_dist);
                                                                    });

            // place the two max dist vtxs on opposite sides upon their spherical coordinates.
            std::array<mc::Position,2> points_on_sphere;
            points_on_sphere[0] = point_on_sphere(centre,(aabb_dimensions[0]-centre).length(),{0.f,0.f});
            points_on_sphere[1] = point_on_sphere(centre,(aabb_dimensions[1]-centre).length(),{0.5f,0.f});

            // compute the radius of the sphere that captures both max dist points.
            float radius = (points_on_sphere[1]-points_on_sphere[0]).length() * 0.5f;

            // find the centre of the sphere using two points and a radius
            std::array<mc::V2,2> max_coordinates;
            max_coordinates[0] = calc_spherical_coordinates(centre, aabb_dimensions[0]);
            max_coordinates[1] = calc_spherical_coordinates(centre, aabb_dimensions[1]);
            // place points on the same 2D circle by locking the theta angle on the z-axis
            points_on_sphere[0] = point_on_sphere(centre,radius,{max_coordinates[0].m_x,0.25f});
            points_on_sphere[1] = point_on_sphere(centre,radius,{max_coordinates[1].m_x,0.25f});


            aabb_dimensions[0] = points_on_sphere[0];
            aabb_dimensions[1] = points_on_sphere[1];



            bisector = (points_on_sphere[0]+points_on_sphere[1]) * 0.5f;
            mc::Direction c_dir = centre - bisector;
            c_dir.normalize();
            mc::Direction b_dir = (points_on_sphere[1]-points_on_sphere[0]);
            b_dir.normalize();
            mc::Direction N;
            if (c_dir.m_y > 0)
                N = b_dir.cross(mc::Direction::right());
            else
                N = mc::Direction::right().cross(b_dir);
            std::cout<< "N = " << N <<std::endl;


//            float radsq = powf(radius,2);
//            float q = (points_on_sphere[1]-points_on_sphere[0]).length();
//            mc::Position bisector = (points_on_sphere[0]+points_on_sphere[1]) * 0.5f;

//            mc::Direction N = centre - bisector;
//            N.normalize();

//            float cy = bisector.m_y + N.m_y * sqrt(radsq - powf((q*0.5f),2)) * ((points_on_sphere[1].m_y - points_on_sphere[0].m_y)/q);
//            float cz = bisector.m_z + N.m_z * sqrt(radsq - powf((q*0.5f),2)) * ((points_on_sphere[1].m_z - points_on_sphere[0].m_z)/q);

//            std::cout<< "0.0," << cy << "," << cz <<std::endl;



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

            point = bisector;
            PT.translate(point.m_x,point.m_y,point.m_z);
            MVP = projection_ * view_ * PT;
            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f,1.f));
            prim->draw("bv_sphere");


        }
};
