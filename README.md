# Maya-like Viewport using NGL

<!-- ![alt tag](mlvn.jpg) -->


This is a demo that illustrates a Maya-like viewport experience using NGL. It uses similar shortcuts to switch between different tools and camera movements.
It is also completely portable to other NGL demos - just copy the "Viewport" directory from this project's "include/", "src/" and "shaders/" into your own project and look at this demo's NGLScene on how to use it.

This demo supports/displays the following:

- Camera pan, track and dolly. (Shortcuts: Alt+LMB = pan, Alt+RMB = dolly, Alt+MMB = track)

- Camera focus on target object. (Shortcuts: f = focus on selected object, SPACE = reset to origin)

- Maya-like grid.

- Mouse controls.

- Title text projection.

- Valid with ngl::VAOPrimitives using ngl::Mat4 and ngl::Transformation.

- Object selection via ray casting.

- Maya-like view axis.

- Transformation controls to translate, rotate and scale. (Shortcuts: q = select, w = translate, e = rotate, r = scale)


## To Do:

- Support ngl::Obj types with the Selection.

- Convert selected ids to std::set instead of std::vector???

- Refactor: Use ScreenSize type for width and height.

- Handles: Create new Transformation handles for move, rotate, scale.

- Viewport: Change mouse icon when <q,w,e,r> are pressed.


## Improvements:

- Select: Create other Bounding Volumes (AABB, OBB, Cone, Convex Hull) and add it as a template arg to the make_selectable() function.

- Replace the functions in the Common.h with native NGL functions.



