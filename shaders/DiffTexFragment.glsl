#version 330 core


uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec4 lightDiffuse;

in vec3 fragmentNormal;
in vec2 vertUV;

layout (location=0)out vec4 outColour;

void main ()
{
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(lightPos);
    outColour = texture(tex,vertUV)*lightDiffuse *dot(L, N);
}
