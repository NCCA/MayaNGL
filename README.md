# Maya-like Viewport using NGL

<!-- ![alt tag](mlvn.jpg) -->


This is a simple demo illustrating a Maya-like viewport, camera movement and object selection using NGL.

To Do:

- Camera: Focus on target (selected object). ~nearly there...

- Select: Template overload for ngl::Mat4, ngl::Transformation, ngl::Obj.

- Select: Create other Bounding Volumes (Plane, Sphere, AABB, OBB, Cone, Convex Hull) and add it as a template arg to the make_selectable() function.

- Handles: Create new Transformation handles for move, rotate, scale.

- Viewport: Change mouse icon when <q,w,e,r> are pressed.

- Select: Create a selection wrapper shader.

- Refactor: Use ScreenSize type for width and height.

