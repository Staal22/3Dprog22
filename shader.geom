#version 330

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec4 fragNormal[];

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_PointSize = 5.0;
    gl_Position = projection * model * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = projection * model * (gl_in[0].gl_Position + fragNormal[0]);
    EmitVertex();

    EndPrimitive();
}
