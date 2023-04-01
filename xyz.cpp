#include "xyz.h"

XYZ::XYZ()
{
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{1,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,1,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,0,0,1});
    mVertices.push_back(Vertex{0,0,1,0,0,1});
    model.setToIdentity();

    drawMethod = GL_LINES;
    indexed = false;
}

XYZ::~XYZ()
{

}

void XYZ::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}
