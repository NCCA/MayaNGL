
#include "MayaNGL/BoundingVolumes/BV_Sphere_Def.hpp"


mc::Position BoundingVolume<mc::Sphere, false>::point_on_sphere(const mc::Position &centre_, float radius_, const mc::V2 &coordinates_)
{
    float fi = 2.f * M_PI * coordinates_.m_x;
    float theta = 2.f * M_PI * coordinates_.m_y;

    mc::Position poc;
    poc.m_x = centre_.m_x + radius_ * sin(fi) * cos(theta);
    poc.m_y = centre_.m_y + radius_ * sin(fi) * sin(theta);
    poc.m_z = centre_.m_z + radius_ * cos(fi);

    return poc;
}

int BoundingVolume<mc::Sphere, false>::getSign(float val)
{
    int sign = (0.f < val) - (val < 0.f);
    return (sign == 0) ? 1 : sign;
}

mc::V2 BoundingVolume<mc::Sphere, false>::calc_spherical_coordinates(const mc::Position &centre_, const mc::Position &point_)
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

mc::Position BoundingVolume<mc::Sphere, false>::calc_centre_of_sphere(const mc::Position &aabb_centre_, const DoubleOf<mc::Position> &points_, float radius_)
{
    mc::Position centre(0.f,0.f,0.f);

    mc::Position bisector = (points_[0]+points_[1]) * 0.5f;
    mc::Direction c_dir = aabb_centre_ - bisector;
    c_dir.normalize();

    float q = (points_[1]-points_[0]).length();
    if (getSign(c_dir.m_y) > 0)
    {
        centre.m_z = bisector.m_z + (sqrt(pow(radius_,2)-pow((q*0.5f),2))*(points_[0].m_y-points_[1].m_y)) / q;
        centre.m_y = bisector.m_y + (sqrt(pow(radius_,2)-pow((q*0.5f),2))*(points_[1].m_z-points_[0].m_z)) / q;
    }
    else
    {
        centre.m_z = bisector.m_z - (sqrt(pow(radius_,2)-pow((q*0.5f),2))*(points_[0].m_y-points_[1].m_y)) / q;
        centre.m_y = bisector.m_y - (sqrt(pow(radius_,2)-pow((q*0.5f),2))*(points_[1].m_z-points_[0].m_z)) / q;
    }

    return centre;
}

void BoundingVolume<mc::Sphere, false>::calc_tight_bound_volume(const VertexList &vtx_list_)
{
    // constuct an AABB from the primitive's vertices.
    mc::AABB box;
    box.calc_end_points(vtx_list_);
    box.calc_centre();

    // find the two most distant vtxs from the centre and calc their radii.
    std::sort(std::begin(box.end_points),std::end(box.end_points),[&box](auto &&v1_,auto &&v2_) -> bool
                                                                  {
                                                                      float v1_dist = (v1_-box.centre).lengthSquared();
                                                                      float v2_dist = (v2_-box.centre).lengthSquared();
                                                                      return (v1_dist > v2_dist);
                                                                  });

    // calculate the centre of sphere
    DoubleOf<float> max_radii;
    max_radii[0] = (box.end_points[0]-box.centre).length();
    max_radii[1] = (box.end_points[1]-box.centre).length();

    // place the two max dist vtxs on opposite sides upon their spherical coordinates.
    DoubleOf<mc::Position> points_on_sphere;
    points_on_sphere[0] = point_on_sphere(box.centre,max_radii[0],{0.f,0.f});
    points_on_sphere[1] = point_on_sphere(box.centre,max_radii[1],{0.5f,0.f});

    // compute the radius of the sphere that captures both max dist points.
    bv.radius = (points_on_sphere[1]-points_on_sphere[0]).length() * 0.5f;

    // place points on the 2D circle by locking the theta angle on the z-axis
    DoubleOf<mc::V2> max_coordinates;
    for (std::size_t i=0; i<points_on_sphere.size(); ++i)
    {
        max_coordinates[i] = calc_spherical_coordinates(box.centre, box.end_points[i]);
        points_on_sphere[i] = point_on_sphere(box.centre,max_radii[i],{max_coordinates[i].m_x,0.25f});
    }

    // calculate the centre of the new sphere
    bv.centre = calc_centre_of_sphere(box.centre,points_on_sphere,bv.radius);
}


//===================================================================//


void BoundingVolume<mc::Sphere, true>::initialize()
{
    ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
    prim->createSphere("bv_sphere",1.f,20);
}

