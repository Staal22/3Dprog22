#include "xyz.h"

XYZ::XYZ()
{
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{1,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,1,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,0,0,1});
    mVertices.push_back(Vertex{0,0,1,0,0,1});
    model.setToIdentity();

    drawMethod = GL_LINES;
    indexed = false;
}

XYZ::~XYZ()
{

}

void XYZ::init()
{
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

void XYZ::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}
