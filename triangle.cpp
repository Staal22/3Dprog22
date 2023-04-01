#include "triangle.h"

Triangle::Triangle()
{
    mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f});
    mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f});
    mVertices.push_back(Vertex{0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f});

    drawMethod = GL_TRIANGLES;
    indexed = false;
}

Triangle::~Triangle()
{

}

void Triangle::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}


