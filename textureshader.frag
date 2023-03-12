#version 410 core

in vec4 color;
in vec2 textureCoord;

out vec4 fragmentColor;     //color sent to fragment on screen

uniform sampler2D textureSampler;
uniform bool hasTexture;

void main()
{
    if (hasTexture)
        fragmentColor = texture2D(textureSampler, textureCoord);
    else
        fragmentColor = color;
}
