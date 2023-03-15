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
}

Trophy::~Trophy()
{
    // Disable the vertex attribute arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    texture->release();
}

void Trophy::init()
{
    initializeOpenGLFunctions();

    // Load the image using QImage
    QImage image;
    image.load("brick_texture.bmp");

//    // Create an OpenGL texture object and bind the image to it
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    texture->setData(image);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    //send vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_xyz)));

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));

    // 3rd attribute buffer : texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uv)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    texture->bind(0);

    glBindVertexArray(0);
}

void Trophy::draw(GLint shader)
{
    modelUniform = shader;
    // Bind the texture to a texture unit
    if (hide)
        return;
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, model.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

bool Trophy::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
           point.z() >= min_.z() && point.z() <= max_.z();
}
