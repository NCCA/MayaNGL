
#include "Viewport/Camera.h"


Camera::Direction Camera::updateInverse()
{
    m_inverse = m_position-m_lookAt;
    m_inverse.normalize();
    return m_inverse;
}

Camera::Camera( const Mouse &mouse_ )
                :
                mouse(mouse_),
                m_position(28.f,21.f,28.f),
                m_lookAt(ngl::Vec3::zero()),
                m_inverse(updateInverse()),
                m_refInverse(m_inverse.m_x,0.f,m_inverse.m_z),
                m_currentView(View::PERSPECTIVE)
{;}

/*
 * I have a strong fealing that Maya uses only one Transformation matrix for all the objects in the
 * scene. The user is only able to modify the transformations of each object at a program level, which
 * get accumulated to the final graphical transformation matrix. When the camera rotates in Maya, I
 * think that it rotates the common Graphics Transformation Matrix, not the View Matrix. When I implement
 * this logic with NGL using only one ngl::Transformation for all objects, then it works just like Maya.
 * However, the ngl::Transformation is not a Singleton and we can have multiple Graphical Transforms.
 * Also, the getMatrix is not a const & which does not help with the logic above.
 * The method below uses a reference of the inverse vector which rotates only about the Y-axis. The
 * correct rotation axis is then calculated using the reference vector instead of the currect inverse.
*/
void Camera::pan()
{
    updateInverse();

    Rotation Ry = sm::Y_Matrix(sm::toRads(mouse.getVelocity()*mouse.getDirection().m_x));
    m_refInverse = m_refInverse*Ry;

    auto rotationAxis = ngl::Vec3::up().cross(m_refInverse);
    rotationAxis.normalize();

    Rotation Rx = sm::Axis_Matrix(sm::toRads(mouse.getVelocity()*mouse.getDirection().m_y),rotationAxis);
    Rotation localR = Rx*Ry;

    m_position -= m_lookAt;
    m_position = m_lookAt + (m_position * localR);
    Rot *= localR.inverse();
}

void Camera::dolly()
{
//    updateInverse();

//    auto dist = (m_position-m_lookAt).length();

//    if(dist<0.5f)
//    {
//        auto dir = -m_inverse;
//        m_lookAt += dist*dir;
//    }

//    auto Tx = mouse.getVelocity() * mouse.getDirection().m_x * m_inverse * Mouse::slowdown;
//    m_position -= Tx;

//    Rot.m_30 += Tx.m_x;
//    Rot.m_31 += Tx.m_y;
//    Rot.m_32 += Tx.m_z;
}

void Camera::track()
{
    updateInverse();

    auto horizontal_axis = ngl::Vec3::up().cross(m_refInverse);
    horizontal_axis.normalize();

    auto vertical_axis = horizontal_axis.cross(m_inverse);
    vertical_axis.normalize();

    auto mouse_move = mouse.getVelocity() * mouse.getDirection() * Mouse::slowdown;

    auto Tx = mouse_move.m_x * horizontal_axis.m_x +  mouse_move.m_y * vertical_axis.m_x;
    auto Ty = mouse_move.m_y * vertical_axis.m_y;
    auto Tz = mouse_move.m_x * horizontal_axis.m_z +  mouse_move.m_y * vertical_axis.m_z;

    m_position.m_x -= Tx;
    m_position.m_y += Ty;
    m_position.m_z -= Tz;

    m_lookAt.m_x -= Tx;
    m_lookAt.m_y += Ty;
    m_lookAt.m_z -= Tz;

    Tra.m_30 += Tx;
    Tra.m_31 += Ty;
    Tra.m_32 += Tz;
}

void Camera::reset(Position &&pos_, View panel_)
{
    m_position = pos_;
    m_lookAt = ngl::Vec3::zero();
    m_inverse = updateInverse();
    m_refInverse = ngl::Vec3(m_inverse.m_x,0.f,m_inverse.m_z);
    m_currentView = panel_;
    Rot.identity();
    Tra.identity();
}

void Camera::focusOn(const Position &target_)
{

}

void Camera::front()
{
    reset({0.f,0.f,28.f},View::FRONT);
}

void Camera::side()
{
    reset({-28.f,0.f,0.f},View::SIDE);
}

void Camera::top()
{
    reset({0.f,28.f,0.f},View::TOP);
}


