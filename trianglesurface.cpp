#include "trianglesurface.h"

TriangleSurface::TriangleSurface(float size)
{
    float halfSize = size / 2.0f;

    // Define the four corners of the plane
    QVector3D v0 = QVector3D(-halfSize, 0.0f, -halfSize);
    QVector3D v1 = QVector3D(halfSize, 0.0f, -halfSize);
    QVector3D v2 = QVector3D(halfSize, 0.0f, halfSize);
    QVector3D v3 = QVector3D(-halfSize, 0.0f, halfSize);

    // Add the vertices to the vertex buffer
    mVertices.push_back(Vertex(v0, QVector3D(1.0f, 1.0f, 1.0f)));
    mVertices.push_back(Vertex(v1, QVector3D(1.0f, 1.0f, 1.0f)));
    mVertices.push_back(Vertex(v2, QVector3D(1.0f, 1.0f, 1.0f)));
    mVertices.push_back(Vertex(v3, QVector3D(1.0f, 1.0f, 1.0f)));

    // Define the indices of the plane
    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(0);
    mIndices.push_back(2);
    mIndices.push_back(3);

    mMatrix.setToIdentity();
}

//TriangleSurface::TriangleSurface(std::string filename)
//{
//    readFile(filename);
//    mMatrix.setToIdentity();
//}

TriangleSurface::~TriangleSurface()
{

}

void TriangleSurface::readFile(std::string filename, bool IndexedVertices)
{
    if (IndexedVertices)
    {
        std::ifstream inn;
        inn.open(filename.c_str());
        if (inn.is_open()) {
        int ni, nv, indeks;
        inn >> ni;
        mIndices.reserve(ni);
        inn >> nv;
        mVertices.reserve(nv);
        for (int i=0; i<ni; i++)
        {
            inn >> indeks; mIndices.push_back(indeks);
        }
        Vertex vertex;
        for (int i=0; i<nv; i++)
        {
            inn >> vertex; mVertices.push_back(vertex);
        }
        inn.close();
        }

    }
    else
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
}

void TriangleSurface::init(GLint shader)
{
    mMatrixUniform = shader;
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

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    mRotation.setToIdentity();

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    if (hide)
    {
        return;
    }

    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void TriangleSurface::rotate()
{
    mMatrix.rotate(2.f, 0.f, 1.f, 0.f);
}
