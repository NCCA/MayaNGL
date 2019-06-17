#version 410 core

layout (location=0) in vec3 inVert;

uniform mat4 MV;
uniform vec3 camEye;

//out vec4 eye_position;

void main()
{
    gl_Position = vec4(inVert,1.0);

    vec4 eye_position = MV * vec4(camEye,1.f);
}
