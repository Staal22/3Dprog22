#version 330 core

in vec4 color;
in vec2 textureCoord;

out vec4 fragmentColor;

uniform bool hasTexture;
uniform sampler2D textureSampler;

void main()
{
    if (hasTexture)
    fragmentColor = texture2D(textureSampler, textureCoord);
    else
        fragmentColor = color;
}
