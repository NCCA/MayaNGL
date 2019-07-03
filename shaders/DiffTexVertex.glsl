#version 330 core


uniform mat4 MVP;
uniform mat3 normalMatrix;

layout (location=0) in vec3 inVert;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inUV;

out vec3 fragmentNormal;
out vec2 vertUV;

void main()
{
    gl_Position = MVP*vec4(inVert, 1.0);
    fragmentNormal = (normalMatrix*inNormal);
    vertUV=inUV.st;
}
