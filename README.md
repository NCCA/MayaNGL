# Maya-like Viewport using NGL

<!-- ![alt tag](mlvn.jpg) -->


This is a simple demo illustrating a Maya-like viewport, camera movement, object selection and object transformation using NGL.

To Do:

- BUG: There's a bug in the updated position of the Camera after dollying through the target object.

- Select: Template overload for ngl::Mat4, ngl::Transformation, ngl::Obj.

- Handles: Create new Transformation handles for move, rotate, scale.

- Viewport: Change mouse icon when <q,w,e,r> are pressed.

- Select: Create a selection wrapper shader.

- Refactor: Use ScreenSize type for width and height.

- Select: Create other Bounding Volumes (Plane, Sphere, AABB, OBB, Cone, Convex Hull) and add it as a template arg to the make_selectable() function.
