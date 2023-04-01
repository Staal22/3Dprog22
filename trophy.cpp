#include "trophy.h"
#include "qimage.h"
#include <cstddef>

Trophy::Trophy(float x, float z)
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

    model.translate(x, 0.0f, z);

    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{x,0,z} - QVector3D(radius / 2, height / 2, radius / 2);
    max_ = QVector3D{x,0,z} + QVector3D(radius / 2, height / 2, radius / 2);

    hasTexture = true;
    drawMethod = GL_TRIANGLES;
    indexed = false;
    texturepath = "brick_texture.bmp";
}

Trophy::~Trophy()
{
    // Disable the vertex attribute arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    texture->release();
}

bool Trophy::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
            point.z() >= min_.z() && point.z() <= max_.z();
}
