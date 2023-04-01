#version 330 core

// Define the input vertex attributes
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 normalIn;
layout(location = 2) in vec2 textureCoordIn;

// Define the output variables
//out vec4 color;
out vec3 fragPos;
out vec2 textureCoord;
out VS_OUT
{
    vec4 normal;
} vs_out;

// Define the uniform variables
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform sampler2D heightmap;
uniform bool hasHeightMap;

// Define constants
const float heightScale = -2;

void main()
{

    if (hasHeightMap)
    {
        // Get the height value from the heightmap texture
        float height = texture(heightmap, textureCoordIn).r * heightScale;

        // Displace the vertex position along the Y-axis
        vec4 positionDisplaced = vec4(positionIn.x, positionIn.y + height, positionIn.z, 1.0);

        // Transform the vertex position into world space
        vec4 worldPos = model * positionDisplaced;

//        // Calculate the normal in world space
//        normal = mat3(transpose(inverse(model))) * vec3(0, 1, 0);

        // Transform the vertex position and normal into view space
        fragPos = vec3(view * worldPos);
//        vs_out.normal = vec4(textureCoordIn.x, textureCoordIn.y, 0.0, 1.0);
        gl_Position = projection * view * worldPos;
    }
    else
    {
        gl_Position = projection * view * model * positionIn;
//        color = normalIn;
    }

    textureCoord = textureCoordIn;
    vs_out.normal = normalIn;
}
