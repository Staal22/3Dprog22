#include "lightsource.h"

LightSource::LightSource(float x, float y, float z)
{
    // Vertex positions and uv
    // Front face
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    // Back face
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, -0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    // Left face
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, -0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    // Right face
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, 0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    // Top face
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, 0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, 0.5f, -0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(-0.5f, 0.5f, -0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    // Bottom face
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, -0.5f), QVector2D(1.0f, 0.0f)}); // Bottom right
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right

    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, -0.5f), QVector2D(0.0f, 0.0f)}); // Bottom left
    mVertices.push_back(Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector2D(1.0f, 1.0f)}); // Top right
    mVertices.push_back(Vertex{QVector3D(-0.5f, -0.5f, 0.5f), QVector2D(0.0f, 1.0f)}); // Top left

    model.translate(x, y, z);

    indexed = false;
    drawMethod = GL_TRIANGLES;
}

LightSource::~LightSource()
{

}
