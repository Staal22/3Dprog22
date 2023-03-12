#version 410 core

layout(location = 0) in vec4 positionIn;     // 1st attribute buffer = position
layout(location = 1) in vec4 colorIn;        // 2nd attribute buffer = normal
layout(location = 2) in vec2 textureCoordIn; // 3rd attribute buffer = uv

out vec4 color;
out vec2 textureCoord;

uniform mat4 pmatrix; // projection matrix
uniform mat4 vmatrix; // view matrix
uniform mat4 matrix;  // model matrix

void main()
{
   color = colorIn;
   textureCoord = textureCoordIn;
   gl_Position = pmatrix * vmatrix * matrix * positionIn;
}
