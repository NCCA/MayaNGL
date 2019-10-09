#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


template<typename BV, bool visualization>
class BoundingVolume;


//===================================================================//


template<>
class BoundingVolume<mc::Sphere, false>;

template<>
class BoundingVolume<mc::Sphere, true>;


