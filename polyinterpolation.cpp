#include "polyinterpolation.h"

PolyInterpolation::PolyInterpolation(bool inPoints)
{
    points = inPoints;
    mVertices.push_back(Vertex{-2,1,0,1,0,0});
    mVertices.push_back(Vertex{-1,.66f,0,1,0,0});
    mVertices.push_back(Vertex{.23f,1.49f,0,1,0,0});
    mVertices.push_back(Vertex{2.1f,3,0,1,0,0});
    mMatrix.setToIdentity();

    for (int i = 0; i < 4; i++)
    {
        double t = 1.0;
        for (int j = 0; j < 4; j++)
        {
            A(i, j) = t;
            t *= x[i];
        }
    }
    c = A.inverted() * QVector4D(y[0], y[1], y[2], y[3]);
}

PolyInterpolation::~PolyInterpolation()
{

}

void PolyInterpolation::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    //VAO is a conntainer that holds VBOs
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    //since the mVAO is bound, this VBO will belong to that VAO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //send vertex data to the GPU
    glBufferData( GL_ARRAY_BUFFER,  //buffer type
                  mVertices.size()*sizeof( Vertex ),    //buffer size
                  mVertices.data(), //the vertices themselves
                  GL_STATIC_DRAW ); //whether we update the buffer on the GPU

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0,                                 //attribute - must match layout
                          3,                                 //size
                          GL_FLOAT,                          //data type
                          GL_FALSE,                          //normalized?
                          sizeof(Vertex),                    //stride
                          reinterpret_cast<GLvoid*>(0));     //array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    // Same paramater list as above, but attribute and offset are adjusted accordingly
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    //release vertex array bind(0) = release lol
    glBindVertexArray(0);
}

void PolyInterpolation::draw()
{
    //what object to draw
    glBindVertexArray(mVAO);
    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //Must be here to update each frame - if static object, it could be set only once
    glUniformMatrix4fv(mMatrixUniform,          //the location of the matrix in the shader
                       1,                       //count
                       GL_FALSE,                //transpose the matrix before sending it?
                       mMatrix.constData());    //the data of the matrix
    //DRAW CALL MOMENT
    if (points)
        glDrawArrays(GL_POINTS, 0, mVertices.size());
    else
        glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());
}


void PolyInterpolation::replace(double xMin, double xMax)
{
    mVertices.clear();
    double h = 0.05f;
    for (double x = xMin; x < xMax; x += h)
    {
        mVertices.push_back(Vertex{static_cast<float>(x),static_cast<float>(evaluate(x)),0,0,1,0});
    }
    mMatrix.setToIdentity();
}

double PolyInterpolation::evaluate(double x)
{
    double t = 1.0;
    double y = 0.0;
    for (int i = 0; i < 4; i++) {
        y += t * c[i];
        t *= x;
    }
    return y;
}

