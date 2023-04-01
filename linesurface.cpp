#include "linesurface.h"

LineSurface::LineSurface()
{
    model.setToIdentity();

    drawMethod = GL_LINES;
    indexed = false;
}

LineSurface::LineSurface(std::string filename)
{
    readFile(filename);
    model.setToIdentity();
}

LineSurface::~LineSurface()
{

}

void LineSurface::readFile(std::string filename)
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

void LineSurface::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}


