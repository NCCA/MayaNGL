#pragma once

#include "Viewport/Camera/Camera.h"


template<typename SEL>
void Camera::focus(const SEL &select_)
{
    Focus<Camera> focus(*this);
    focus.onSelected(select_);
}
