#include "curve.h"

Curve::Curve()
{
    mVertices.clear();
    for (auto x = xMin; x < xMax; x += h)
    {
        z = function(1, x);
        mVertices.push_back(Vertex{x, z, 0, 0, z, 0});
    }
    model.setToIdentity();

    drawMethod = GL_LINES;
    indexed = false;
}

Curve::~Curve()
{

}

void Curve::writeFile(std::string filename)
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

void Curve::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);

}

float Curve::function(int variables, float x, float y)
{
    return cos(x + (M_PI/2)) * cos(x + (M_PI/2)); //Oppgave 2
}
