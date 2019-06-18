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
layout (triangle_strip, max_vertices = 36) out;
//layout(line_strip, max_vertices = 8) out;

uniform mat4 MVP;
uniform bool enableViewAxisColours;
uniform vec3 cam_eye;

out vec4 axisColour;


void createFace(vec4 vtx1_, vec4 vtx2_, vec4 vtx3_)
{
    gl_Position = vtx1_;
    EmitVertex();
    gl_Position = vtx2_;
    EmitVertex();
    gl_Position = vtx3_;
    EmitVertex();
}

//void createLine(vec4 vtx1_, vec4 vtx2_)
//{
//    gl_Position = vtx1_;
//    EmitVertex();
//    gl_Position = vtx2_;
//    EmitVertex();
//}

void main()
{
    vec3 vtx0 = gl_in[0].gl_Position.xyz;
    vec3 vtx1 = gl_in[1].gl_Position.xyz;

    vec3 lineVec = normalize(vtx1 - vtx0);

    float Thickness = 0.06f;
    axisColour = WHITE;
    if (enableViewAxisColours == false)
    {
        axisColour = LIGHT_GRAY;
        float dist = distance(cam_eye,lineVec);
//        float attenuation = 0.9f/((1.f+0.6f*dist)+(1.f+0.001f*pow(dist,2)));
        Thickness = dist * 0.0006f;

        if ( vtx0.x == 0.0 || vtx0.z == 0.0)
        {
            axisColour = DARK_GRAY;
            Thickness = dist * 0.001f;
        }
    }
    else
    {
        if (lineVec == UP)
            axisColour = GREEN;
        if (lineVec == RIGHT)
            axisColour = RED;
        if (lineVec == FRONT)
            axisColour = BLUE;
    }

    vec3 upVec = UP;
    if (dot(lineVec,upVec)==1.0)
        upVec = FRONT;

    vec3 normalVec = normalize(cross(lineVec,upVec));

    vec4 v0 = MVP * vec4((vtx0 - Thickness * normalVec),1.0);
    vec4 v1 = MVP * vec4((vtx0 - Thickness * upVec),1.0);
    vec4 v2 = MVP * vec4((vtx0 + Thickness * upVec),1.0);
    vec4 v3 = MVP * vec4((vtx0 + Thickness * normalVec),1.0);
    vec4 v4 = MVP * vec4((vtx1 + Thickness * upVec),1.0);
    vec4 v5 = MVP * vec4((vtx1 + Thickness * normalVec),1.0);
    vec4 v6 = MVP * vec4((vtx1 - Thickness * normalVec),1.0);
    vec4 v7 = MVP * vec4((vtx1 - Thickness * upVec),1.0);

    createFace(v0, v1, v2);
    createFace(v2, v1, v3);
    createFace(v2, v3, v4);
    createFace(v4, v3, v5);
    createFace(v4, v5, v6);
    createFace(v6, v5, v7);
    createFace(v6, v7, v0);
    createFace(v0, v7, v1);
    createFace(v1, v7, v3);
    createFace(v3, v7, v5);
    createFace(v6, v0, v4);
    createFace(v4, v0, v3);

    EndPrimitive();
}
