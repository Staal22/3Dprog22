#include "trophy.h"

Trophy::Trophy(float x, float z)
{
    // Calculate the angle between each slice
    float angle = (2 * M_PI) / numSlices;

    // Generate vertices for the base of the cone
    mVertices.push_back(Vertex{QVector3D(0, 0, 0), QVector3D{1,1,0}});
    for (int i = 0; i <= numSlices; i++)
    {
        float x = radius * cos(i * angle);
        float z = radius * sin(i * angle);
        mVertices.push_back(Vertex{QVector3D(x, 0, z), QVector3D{1,1,0}});
    }

    // Generate vertices for the top of the cone
    mVertices.push_back(Vertex{QVector3D(0, height, 0), QVector3D{1,1,0}});
    for (int i = 0; i <= numSlices; i++)
    {
        float x = radius * cos(i * angle);
        float z = radius * sin(i * angle);
        mVertices.push_back(Vertex{QVector3D(x, height, z), QVector3D{1,1,0}});
    }

    // Generate vertices for the sides of the cone
    for (int i = 1; i <= numSlices; i++)
    {
        mVertices.push_back(Vertex{QVector3D(0.0f, 0.0f, 0.0f), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos(i * angle), 0.0f, radius * sin(i * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos((i - 1) * angle), 0.0f, radius * sin((i - 1) * angle)), QVector3D{1,1,0}});

        mVertices.push_back(Vertex{QVector3D(0.0f, height, 0.0f), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos((i - 1) * angle), 0.0f, radius * sin((i - 1) * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos(i * angle), 0.0f, radius * sin(i * angle)), QVector3D{1,1,0}});

        mVertices.push_back(Vertex{QVector3D(radius * cos(i * angle), 0.0f, radius * sin(i * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos((i - 1) * angle), 0.0f, radius * sin((i - 1) * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos((i - 1) * angle), height, radius * sin((i - 1) * angle)), QVector3D{1,1,0}});

        mVertices.push_back(Vertex{QVector3D(radius * cos(i * angle), 0.0f, radius * sin(i * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos((i - 1) * angle), height, radius * sin((i - 1) * angle)), QVector3D{1,1,0}});
        mVertices.push_back(Vertex{QVector3D(radius * cos(i * angle), height, radius * sin(i * angle)), QVector3D{1,1,0}});
    }

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
