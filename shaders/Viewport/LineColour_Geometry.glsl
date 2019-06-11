#version 410 core

layout (lines) in;
layout (triangle_strip, max_vertices = 36) out;
//layout(line_strip, max_vertices = 8) out;

uniform mat4 MVP;
uniform float Dist;

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

    axisColour = vec4(0.3f,0.3f,0.3f,1.f);
    float Thickness = Dist * 0.025f;

    if ( vtx0.x == 0.f || vtx0.z == 0.f)
    {
        axisColour = vec4(0.25f,0.25f,0.25f,1.f);
        Thickness = Dist * 0.05f;
    }

    vec3 lineVec = normalize(vtx1 - vtx0);

    vec3 upVec = vec3(0.0,1.0,0.0);
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
