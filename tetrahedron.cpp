#include "tetrahedron.h"
#include "graphfunction.h"

Tetrahedron::Tetrahedron()
{
//    // Indexed
//    // face
//    mVertices.push_back(Vertex{0.f, 0.f, 0.f,  1.f, 1.f, 0.f});
//    mVertices.push_back(Vertex{1.f, 0.f, 0.f,  1.f, 1.f, 0.f});
//    mVertices.push_back(Vertex{0.5f, 1.f, 0.f,  1.f, 1.f, 0.f});
//    // nose
//    mVertices.push_back(Vertex{0.5f, 0.5f, 1.f,  1.f, 0.f, 0.f});

//    mIndices.push_back(0);
//    mIndices.push_back(1);
//    mIndices.push_back(2);
//    mIndices.push_back(0);
//    mIndices.push_back(2);
//    mIndices.push_back(3);
//    mIndices.push_back(0);
//    mIndices.push_back(3);
//    mIndices.push_back(1);
//    mIndices.push_back(3);
//    mIndices.push_back(2);
//    mIndices.push_back(1);

    // Un-indexed
    float size = 1.0f; // Size of the tetrahedron

    // Generate the vertices of the tetrahedron
    mVertices.push_back(Vertex{QVector3D(-size, 0.0f, -size), QVector3D(1,0,1)}); // Left bottom
    mVertices.push_back(Vertex{QVector3D(size, 0.0f, -size), QVector3D(1,0,1)}); // Right bottom
    mVertices.push_back(Vertex{QVector3D(0.0f, size, 0.0f), QVector3D(1,0,1)}); // Top
    mVertices.push_back(Vertex{QVector3D(0.0f, 0.0f, size), QVector3D(1,0,1)}); // Front

    mVertices.push_back(Vertex{QVector3D(size, 0.0f, -size), QVector3D(1,0,1)}); // Right bottom
    mVertices.push_back(Vertex{QVector3D(-size, 0.0f, -size), QVector3D(1,0,1)}); // Left bottom
    mVertices.push_back(Vertex{QVector3D(0.0f, 0.0f, size), QVector3D(1,0,1)}); // Front
    mVertices.push_back(Vertex{QVector3D(0.0f, size, 0.0f), QVector3D(1,0,1)}); // Top

    mVertices.push_back(Vertex{QVector3D(-size, 0.0f, -size), QVector3D(1,0,1)}); // Left bottom
    mVertices.push_back(Vertex{QVector3D(0.0f, size, 0.0f), QVector3D(1,0,1)}); // Top
    mVertices.push_back(Vertex{QVector3D(0.0f, 0.0f, size), QVector3D(1,0,1)}); // Front
    mVertices.push_back(Vertex{QVector3D(size, 0.0f, -size), QVector3D(1,0,1)}); // Right bottom

    computeVertexNormals();
    drawMethod = GL_TRIANGLES;
    indexed = false;
}

Tetrahedron::~Tetrahedron()
{

}

void Tetrahedron::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}

void Tetrahedron::move(float dt, GraphFunction* function)
{
    QVector4D pos4D = mPosition.column(3);
    QVector3D pos3D = pos4D.toVector3D();
    // movement
    if (pos3D.x() > function->range().second)
    {
        mVelocity.setX(1);
    }
    else if (pos3D.x() < function->range().first)
    {
        mVelocity.setX(-1);
    }
    QVector3D ds=mVelocity*dt;
    float y = function->evaluate(pos3D.x());
    float distance = pos3D.distanceToPoint(QVector3D(pos3D.x(), y, pos3D.z()));
    if (pos3D.y() > y)
        distance *= -1;
    mPosition.translate(ds.x(), distance, ds.z());
    model = mPosition;
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
