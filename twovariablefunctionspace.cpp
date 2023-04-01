#include "twovariablefunctionspace.h"

TwoVariableFunctionSpace::TwoVariableFunctionSpace()
{
    {
        float y = 0.f;
        for (auto x = min; x < max; x += h)
        {
            for (auto z = min; z < max; z += h)
            {
                y = 0;
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x, z);
                mVertices.push_back(Vertex{x, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x, z + h);
                mVertices.push_back(Vertex{x, y, z + h, 0, 0.6f, y});
                mVertices.push_back(Vertex{x, y, z + h, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z);
                mVertices.push_back(Vertex{x + h, y, z, 0, 0.6f, y});
                if (!(5 > x && x > -5 && 10 > z && z > -10))
                    y = evaluate(x + h, z + h);
                mVertices.push_back(Vertex{x + h, y, z + h, 0, 0.6f, y});
            }
        }
    }
    model.setToIdentity();

    drawMethod = GL_TRIANGLES;
    indexed = false;
}

TwoVariableFunctionSpace::~TwoVariableFunctionSpace()
{

}

void TwoVariableFunctionSpace::writeFile(std::string filename)
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

void TwoVariableFunctionSpace::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}

std::pair<double, double> TwoVariableFunctionSpace::range() const
{
    return std::make_pair(min, max);
}

float TwoVariableFunctionSpace::numericIntegral()
{
    double dx = h;
    double dy = h;
    double area = dx * dy;
    double cumSum = 0.f;
    for (float x = 0 + h; x < 1.0f; x += h)
    {
        for (float y = 0 + h; y < 1.0f - x; y += h)
        {
            cumSum += area * functionNumeric(x, y);
        }
    }
    return cumSum;
}

double TwoVariableFunctionSpace::evaluate(double x, double z) const
{
    return sin(M_PI * x) * sin(M_PI * z); //Oppgave 1
}

float TwoVariableFunctionSpace::functionNumeric(double x, double z)
{
    return (1 - x - z);//oppgave 3
}

float TwoVariableFunctionSpace::funcX(float x, float z)
{
    return (M_PI*cos(M_PI*x) * sin(M_PI*z));
}

float TwoVariableFunctionSpace::funcY(float x, float z)
{
    return (M_PI*cos(M_PI*z) * sin(M_PI*x));
}
