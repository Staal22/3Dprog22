#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT
{
    vec4 normal;
} gs_in[];

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = gl_in[0].gl_Position;
    gl_Position = projection * view * model * pos;

    gl_Position.x += 5 * gs_in[0].normal.x;
    gl_Position.y += 5 * gs_in[0].normal.y;
    gl_Position.z += 5 * gs_in[0].normal.z;

    EmitVertex();

    gl_Position = projection * view * model * pos;
    EmitVertex();

    EndPrimitive();
}
