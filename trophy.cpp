#include "trophy.h"

Trophy::Trophy(float x, float z)
{
    // Vertex positions
    Vertex v0 (QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0,1,1));
    Vertex v1 (QVector3D(0.5f, -0.5f, -0.5f), QVector3D(1,0,1));
    Vertex v2 (QVector3D(0.5f, 0.5f, -0.5f), QVector3D(1,1,0));
    Vertex v3 (QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(1,1,1));
    Vertex v4 (QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(0,0,1));
    Vertex v5 (QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0,1,0));
    Vertex v6 (QVector3D(0.5f, 0.5f, 0.5f), QVector3D(1,0,0));
    Vertex v7 (QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(1,1,0));

    // Front face
    mVertices.push_back(v0); mVertices.push_back(v1); mVertices.push_back(v2);
    mVertices.push_back(v0); mVertices.push_back(v2); mVertices.push_back(v3);

    // Back face
    mVertices.push_back(v4); mVertices.push_back(v6); mVertices.push_back(v5);
    mVertices.push_back(v4); mVertices.push_back(v7); mVertices.push_back(v6);

    // Top face
    mVertices.push_back(v3); mVertices.push_back(v2); mVertices.push_back(v6);
    mVertices.push_back(v3); mVertices.push_back(v6); mVertices.push_back(v7);

    // Bottom face
    mVertices.push_back(v0); mVertices.push_back(v4); mVertices.push_back(v5);
    mVertices.push_back(v0); mVertices.push_back(v5); mVertices.push_back(v1);

    // Left face
    mVertices.push_back(v0); mVertices.push_back(v3); mVertices.push_back(v7);
    mVertices.push_back(v0); mVertices.push_back(v7); mVertices.push_back(v4);

    // Right face
    mVertices.push_back(v1); mVertices.push_back(v5); mVertices.push_back(v6);
    mVertices.push_back(v1); mVertices.push_back(v6); mVertices.push_back(v2);

    mMatrix.translate(x, 0.0f, z);

    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{x,0,z} - QVector3D(radius / 2, height / 2, radius / 2);
    max_ = QVector3D{x,0,z} + QVector3D(radius / 2, height / 2, radius / 2);
}

Trophy::~Trophy()
{

}

void Trophy::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //send vertex data to the GPU
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Trophy::draw()
{
    if (hide)
        return;
    glBindVertexArray(mVAO);
    glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

bool Trophy::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
           point.z() >= min_.z() && point.z() <= max_.z();
}
