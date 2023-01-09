#include "xyz.h"

XYZ::XYZ()
{
    //triangle 1
    mVertices.push_back(Vertex{0, 0, 0, 1, 0, 0});
    mVertices.push_back(Vertex{0, 1, 0, 1, 0, 0});
    mVertices.push_back(Vertex{1, 0, 0, 1, 0, 0});

    //triangle 2
    mVertices.push_back(Vertex{0, 0, 0, 0, 1, 0});
    mVertices.push_back(Vertex{0, 0, -1, 0, 1, 0});
    mVertices.push_back(Vertex{1, 0, 0, 0, 1, 0});
}

void XYZ::init(GLuint mVAO, GLuint mVBO)
{
   initializeOpenGLFunctions();

   //Vertex Buffer Object to hold vertices - VBO
   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

   // 1st attribute buffer : vertices
   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   // 2nd attribute buffer : colors
   // Same parameter list as above but attribute and offset is adjusted accoringly
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof( GLfloat ),  (GLvoid*)(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);
}

void XYZ::draw()
{
   //actual draw call
   //setContext(context);
   glDrawArrays(GL_LINES, 0, mVertices.size());
}
