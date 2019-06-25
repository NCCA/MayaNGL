
#include "Viewport/LookAt/LookAt.h"


float LookAt::calcDist() const
{
    return (target-eye).length();
}

vc::Direction LookAt::calcDirection() const
{
    auto dir = target-eye;
    dir.normalize();
    return dir;
}
