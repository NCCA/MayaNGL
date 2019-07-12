#version 410 core

#define UP      vec3(0.f,1.f,0.f)
#define RIGHT   vec3(1.f,0.f,0.f)
#define FRONT   vec3(0.f,0.f,1.f)

#define WHITE       vec4(1.f,1.f,1.f,1.f)
#define RED         vec4(1.f,0.f,0.f,1.f)
#define GREEN       vec4(0.f,1.f,0.f,1.f)
#define BLUE        vec4(0.f,0.f,1.f,1.f)
#define LIGHT_GRAY  vec4(0.3f,0.3f,0.3f,1.f);
#define DARK_GRAY   vec4(0.25f,0.25f,0.25f,1.f);

layout (lines) in;
layout(line_strip, max_vertices = 2) out;

out vec4 axisColour;


void createLine(vec4 vtx1_, vec4 vtx2_)
{
    gl_Position = vtx1_;
    EmitVertex();
    gl_Position = vtx2_;
    EmitVertex();
}

void main()
{
    vec3 vtx0 = gl_in[0].gl_Position.xyz;
    vec3 vtx1 = gl_in[1].gl_Position.xyz;

    vec3 lineVec = normalize(vtx1 - vtx0);

    if (lineVec == UP)
        axisColour = GREEN;
    if (lineVec == RIGHT)
        axisColour = RED;
//    if (lineVec == FRONT)
//        axisColour = BLUE;

    createLine(vec4(vtx0,1.f),vec4(vtx1,1.f));

    EndPrimitive();
}
