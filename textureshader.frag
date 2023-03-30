#version 330 core

in vec2 textureCoord;
in VS_OUT
{
    vec4 normal;
} ts_in;

out vec4 fragmentColor;

uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec3 lightPos;


void main()
{
    if (hasTexture)
        fragmentColor = texture2D(textureSampler, textureCoord);
    else
        fragmentColor = ts_in.normal;
}
