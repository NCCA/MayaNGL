#version 410 core

#define UP      vec3(0.f,1.f,0.f)
#define RIGHT   vec3(1.f,0.f,0.f)
#define FRONT   vec3(0.f,0.f,1.f)

#define LIGHT_GRAY  vec4(0.3f,0.3f,0.3f,1.f);
#define DARK_GRAY   vec4(0.25f,0.25f,0.25f,1.f);

layout (lines) in;
layout (triangle_strip, max_vertices = 36) out;

uniform mat4 MVP;
uniform vec3 cam_eye;

out vec4 gridColour;


void createFace(vec4 vtx1_, vec4 vtx2_, vec4 vtx3_)
{
    gl_Position = MVP * vtx1_;
    EmitVertex();
    gl_Position = MVP * vtx2_;
    EmitVertex();
    gl_Position = MVP * vtx3_;
    EmitVertex();
}

void main()
{
    vec3 vtx0 = gl_in[0].gl_Position.xyz;
    vec3 vtx1 = gl_in[1].gl_Position.xyz;

    vec3 dir = normalize(vtx1 - vtx0);

    float dist = distance(cam_eye,dir);
    // float attenuation = 0.9f/((1.f+0.6f*dist)+(1.f+0.001f*pow(dist,2)));

    float thickness = dist * 0.0006f;
    gridColour = LIGHT_GRAY;

    if ( vtx0.x == 0.f || vtx0.z == 0.f)
    {
        thickness = dist * 0.001f;
        gridColour = DARK_GRAY;
    }

    // This is a small condition to compensate for vertical
    //  lines. Ideally I need to compute the up vector using
    //  the same approach as with the camera.
    vec3 upVec = UP;
    if (dot(dir,upVec)==1.f)
        upVec = FRONT;

    vec3 normalVec = normalize(cross(dir.xyz,upVec.xyz));

    vec4 v0 = vec4(vtx0 - thickness * normalVec,1.f);
    vec4 v1 = vec4(vtx0 - thickness * upVec,1.f);
    vec4 v2 = vec4(vtx0 + thickness * upVec,1.f);
    vec4 v3 = vec4(vtx0 + thickness * normalVec,1.f);
    vec4 v4 = vec4(vtx1 + thickness * upVec,1.f);
    vec4 v5 = vec4(vtx1 + thickness * normalVec,1.f);
    vec4 v6 = vec4(vtx1 - thickness * normalVec,1.f);
    vec4 v7 = vec4(vtx1 - thickness * upVec,1.f);

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
