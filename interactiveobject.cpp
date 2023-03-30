#include "interactiveobject.h"

InteractiveObject::InteractiveObject()
{
    // face
    mVertices.push_back(Vertex{0.f, 0.f, 0.f,  1.f, 1.f, 0.f});
    mVertices.push_back(Vertex{1.f, 0.f, 0.f,  1.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.5f, 1.f, 0.f,  1.f, 1.f, 0.f});
    // nose
    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  1.f, 0.f, 0.f});

    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(0);
    mIndices.push_back(2);
    mIndices.push_back(3);
    mIndices.push_back(0);
    mIndices.push_back(3);
    mIndices.push_back(1);
    mIndices.push_back(3);
    mIndices.push_back(2);
    mIndices.push_back(1);

    drawMethod = GL_TRIANGLES;
}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // Her kommer et tillegg som har med Index Buffer Object og indeksarray å gjøre
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    mRotation.setToIdentity();

    //release vertex array bind(0) = release lol
    glBindVertexArray(0);
}

void InteractiveObject::move(float x, float y, float z)
{
    mx += x;
    my += y;
    mz += z;
    model.translate(x, y, z);
}
