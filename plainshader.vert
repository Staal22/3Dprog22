#version 330 core

// Define the input vertex attributes
layout(location = 0) in vec4 positionIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 textureCoordIn;

// Define the output variables
out vec4 color;
out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;


// Define the uniform variables
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform sampler2D heightmap;

// Define constants
const float heightScale = 0.2;

void main()
{
//    // Get the height value from the heightmap texture
//    float height = texture(heightmap, textureCoordIn).r * heightScale;

//    // Displace the vertex position along the Y-axis
//    vec4 positionDisplaced = vec4(positionIn.x, positionIn.y + height, positionIn.z, 1.0);

//    // Transform the vertex position into world space
//    vec4 worldPos = model * positionDisplaced;

//    // Calculate the normal in world space
//    normal = mat3(transpose(inverse(model))) * vec3(0, 1, 0);

//    // Transform the vertex position and normal into view space
//    fragPos = vec3(view * worldPos);
//    normal = normalize(mat3(view * model) * normalOut);

    color = colorIn;
    textureCoord = textureCoordIn;
//    gl_Position = projection * view * worldPos;
    gl_Position = projection * view * model * positionIn;

}
