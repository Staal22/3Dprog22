#version 330 core

in vec2 textureCoord;

out vec4 fragmentColor;

//uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec3 lightColor;

void main()
{
    fragmentColor = texture2D(textureSampler, textureCoord) * vec4(lightColor , 1);
}
