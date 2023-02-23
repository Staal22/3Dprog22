#include "twovariablefunctionspace.h"

TwoVariableFunctionSpace::TwoVariableFunctionSpace()
{
    {
        float y = 0.f;
        for (auto x = min; x < max; x += h)
        {
            for (auto z = min; z < max; z += h)
            {
                y = 0;
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x, z);
                mVertices.push_back(Vertex{x, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x, z + h);
                mVertices.push_back(Vertex{x, y, z + h, 0, 0.6f, y});
                mVertices.push_back(Vertex{x, y, z + h, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z + h);
                mVertices.push_back(Vertex{x + h, y, z + h, 0, 0.6f, y});
            }
        }
    }
    mMatrix.setToIdentity();
}

TwoVariableFunctionSpace::~TwoVariableFunctionSpace()
{

}

void TwoVariableFunctionSpace::writeFile(std::string filename)
{
    std::ofstream outf{ filename };
    if (!outf)
    {
        std::cerr << "Uh oh, " << filename << " could not be opened for writing!\n";
    }
    //    outf << "Number of vertices: " << mVertices.size() << " Format is (x,y,z)(r,g,b,)(u,v)" << '\n';
    outf << mVertices.size() << '\n';
    foreach (auto Vertex, mVertices) {
        outf << Vertex;
    }
    outf.close();
    //If we're sending our vertices off to a file to be used somehwere else, we shouldn't
    //leave behind duplicates here
    mVertices.clear();
}

void TwoVariableFunctionSpace::init(GLint shader)
{
    mMatrixUniform = shader;

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

void TwoVariableFunctionSpace::draw()
{
    // Should never draw itself as of now, gets drawn by TriangleSurface, use for debugging
    //what object to draw
    glBindVertexArray(mVAO);
    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
    //Must be here to update each frame - if static object, it could be set only once
    glUniformMatrix4fv(mMatrixUniform,          //the location of the matrix in the shader
                       1,                       //count
                       GL_FALSE,                //transpose the matrix before sending it?
                       mMatrix.constData());    //the data of the matrix
    //DRAW CALL MOMENT
    glDrawArrays(GL_TRIANGLES,
                 0,
                 mVertices.size());
}

void TwoVariableFunctionSpace::rotate()
{
    mMatrix.rotate(2.f, 0.f, 1.f, 0.f);
}

std::pair<double, double> TwoVariableFunctionSpace::range() const
{
    return std::make_pair(min, max);
}

float TwoVariableFunctionSpace::numericIntegral()
{
    double dx = h;
    double dy = h;
    double area = dx * dy;
    double cumSum = 0.f;
    for (float x = 0 + h; x < 1.0f; x += h)
    {
        for (float y = 0 + h; y < 1.0f - x; y += h)
        {
            cumSum += area * functionNumeric(x, y);
        }
    }
    return cumSum;
}

double TwoVariableFunctionSpace::evaluate(double x, double z) const
{
    return sin(M_PI * x) * sin(M_PI * z); //Oppgave 1
}

float TwoVariableFunctionSpace::functionNumeric(double x, double z)
{
    return (1 - x - z);//oppgave 3
}

float TwoVariableFunctionSpace::funcX(float x, float z)
{
    return (M_PI*cos(M_PI*x) * sin(M_PI*z));
}

float TwoVariableFunctionSpace::funcY(float x, float z)
{
    return (M_PI*cos(M_PI*z) * sin(M_PI*x));
}
