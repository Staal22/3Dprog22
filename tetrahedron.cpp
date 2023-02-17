#include "tetrahedron.h"

Tetrahedron::Tetrahedron()
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
}

Tetrahedron::~Tetrahedron()
{

}

void Tetrahedron::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
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
    glBindVertexArray(0);
}

void Tetrahedron::draw()
{
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
