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

void InteractiveObject::move(float x, float y, float z)
{
    mx += x;
    my += y;
    mz += z;
    model.translate(x, y, z);
}
