#include "lightswitch.h"

LightSwitch::LightSwitch(float x, float y, float z)
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

    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{x,y,z} - QVector3D(0.5f, 1, 0.5f);
    max_ = QVector3D{x,y,z} + QVector3D(0.5f, 1, 0.5f);

    indexed = false;
}

LightSwitch::~LightSwitch()
{

}

bool LightSwitch::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
           point.z() >= min_.z() && point.z() <= max_.z();
}

void LightSwitch::ToggleLight()
{
    light->ToggleOnOff();
}
