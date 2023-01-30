#include "tetrahedron.h"

Tetrahedron::Tetrahedron()
{
    // face
    mVertices.push_back(Vertex{0.f, 0.f, 0.f,  1.f, 1.f, 0.f});
    mVertices.push_back(Vertex{1.f, 0.f, 0.f,  1.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.5f, 1.f, 0.f,  1.f, 1.f, 0.f});
    // nose
    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  1.f, 0.f, 0.f});

//    // side 1
//    mVertices.push_back(Vertex{0.5f, 1.f, 0.f,  1.f, 0.f, 0.f});
//    mVertices.push_back(Vertex{1.f, 0.f, 0.f,  1.f, 0.f, 0.f});
//    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  1.f, 0.f, 0.f});
//    // side 2
//    mVertices.push_back(Vertex{0.5f, 1.f, 0.f,  0.f, 1.f, 0.f});
//    mVertices.push_back(Vertex{0.f, 0.f, 0.f,  0.f, 1.f, 0.f});
//    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  0.f, 1.f, 0.f});
//    // side 3
//    mVertices.push_back(Vertex{0.f, 0.f, 0.f,  0.f, 0.f, 1.f});
//    mVertices.push_back(Vertex{1.f, 0.f, 0.f,  0.f, 0.f, 1.f});
//    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  0.f, 0.f, 1.f});
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

}

Tetrahedron::~Tetrahedron()
{

}

void Tetrahedron::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

//    //Vertex Array Object - VAO
//    //VAO is a conntainer that holds VBOs
//    glGenVertexArrays( 1, &mVAO );
//    glBindVertexArray( mVAO );

//    //Vertex Buffer Object to hold vertices - VBO
//    //since the mVAO is bound, this VBO will belong to that VAO
//    glGenBuffers( 1, &mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

//    //send vertex data to the GPU
//    glBufferData( GL_ARRAY_BUFFER,  //buffer type
//                  mVertices.size()*sizeof( Vertex ),    //buffer size
//                  mVertices.data(), //the vertices themselves
//                  GL_STATIC_DRAW ); //whether we update the buffer on the GPU

//    // 1st attribute buffer : vertices
//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//    glVertexAttribPointer(0,                                 //attribute - must match layout
//                          3,                                 //size
//                          GL_FLOAT,                          //data type
//                          GL_FALSE,                          //normalized?
//                          sizeof(Vertex),                    //stride
//                          reinterpret_cast<GLvoid*>(0));     //array buffer offset
//    glEnableVertexAttribArray(0);

//    // 2nd attribute buffer : colors
//    // Same paramater list as above, but attribute and offset are adjusted accordingly
//    glVertexAttribPointer(1,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          sizeof(Vertex),
//                          reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    // Her kommer et tillegg som har med Index Buffer Object og indeksarray å gjøre
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    //release vertex array bind(0) = release lol
    glBindVertexArray(0);
}

void Tetrahedron::draw()
{
//    //what object to draw
//    glBindVertexArray(mVAO);
//    //Since our shader uses a matrix and we rotate the triangle, we send the current matrix here
//    //Must be here to update each frame - if static object, it could be set only once
//    glUniformMatrix4fv(mMatrixUniform,          //the location of the matrix in the shader
//                       1,                       //count
//                       GL_FALSE,                //transpose the matrix before sending it?
//                       mMatrix.constData());    //the data of the matrix
//    //DRAW CALL MOMENT
//    glDrawArrays(GL_TRIANGLES,
//                 0,
//                 mVertices.size());

    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void Tetrahedron::rotate()
{
    mMatrix.rotate(2.f, 0.f, 1.f, 0.f);
}

void Tetrahedron::writeFile(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());
    if (ut.is_open())
    {
        auto m = mIndices.size();
        auto n = mVertices.size();
        Vertex vertex;
        GLuint Int;
        ut << m << " " << n << std::endl << std::endl;
        int count = 0;
        for (auto it=mIndices.begin(); it != mIndices.end(); it++)
        {
            count++;
            Int = *it;
            ut << Int << " ";
            if (count % 3 == 0)
                ut << std::endl;
        }
        ut << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            vertex = *it;
            ut << vertex;
        }
        ut.close();
    }
}
