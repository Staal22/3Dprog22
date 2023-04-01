#version 330 core

in VS_OUT
{
    vec4 normal;
} fs_in;

out vec4 fragmentColor;

void main()
{
    fragmentColor = fs_in.normal;
}
