#version 330 core

in vec2 textureCoord;
in VS_OUT
{
    vec4 normal;
    vec3 fragPos;
} fs_in;

out vec4 fragmentColor;

//uniform bool hasTexture;
uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

const float ambientStrength = 0.1;
const float specularStrength = 0.5;

void main()
{
    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec4 norm = normalize(fs_in.normal);
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(norm, vec4(lightDir, 1)), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, vec3(norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combined - we get Phong shading
    vec3 result = (ambient + diffuse + specular) * vec3(fs_in.normal);
    fragmentColor = texture2D(textureSampler, textureCoord)/* * vec4(result, 1.0)*/;
}
