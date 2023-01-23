#include "twovariablefunctionspace.h"

TwoVariableFunctionSpace::TwoVariableFunctionSpace(int variables)
{
    mVertices.clear();
    if (variables == 1)
    {
        for (auto x = xMin; x < xMax; x += h)
        {
            z = function(1, x);
            mVertices.push_back(Vertex{x, z, 0, 0, z, 0});
        }
    }
    else if (variables == 2)
    {
        for (auto x = xMin; x < xMax; x += h)
        {
            for (auto y = yMin; y < yMax; y += h)
            {
                z = function(2, x, y);
                mVertices.push_back(Vertex{x, y, z, 0, 0, z});
                z = function(2, x + h, y);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0, z});
                z = function(2, x, y + h);
                mVertices.push_back(Vertex{x, y + h, z, 0, 0, z});
                mVertices.push_back(Vertex{x, y + h, z, 0, 0, z});
                z = function(2, x + h, y);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0, z});
                z = function(2, x + h, y + h);
                mVertices.push_back(Vertex{x + h, y + h, z, 0, 0, z});
            }
        }
    }
    mMatrix.setToIdentity();
}

TwoVariableFunctionSpace::TwoVariableFunctionSpace(std::string filename)
{
    readFile(filename);
    mMatrix.setToIdentity();
}

TwoVariableFunctionSpace::~TwoVariableFunctionSpace()
{

}

void TwoVariableFunctionSpace::ToFile(std::string filename)
{
    std::ofstream outf{ filename };
    if (!outf)
    {
        std::cerr << "Uh oh, vertices.txt could not be opened for writing!\n";
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

void TwoVariableFunctionSpace::readFile(std::string filename)
{
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
             inn >> vertex;
             mVertices.push_back(vertex);
        }
        inn.close();
    }
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

float TwoVariableFunctionSpace::function(int variables, float x, float y)
{
    if (variables == 1)
    {
        return cos(x + (M_PI/2)) * cos(x + (M_PI/2)); //Oppgave 2
    }
    else if (variables == 2)
    {
        return sin(M_PI * x) * sin(M_PI * y); //Oppgave 1
    }
    else
        return -1.f;
}
