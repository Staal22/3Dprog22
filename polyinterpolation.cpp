#include "polyinterpolation.h"

PolyInterpolation::PolyInterpolation(bool inPoints)
{
    points = inPoints;
    mVertices.push_back(Vertex{x1[0], y1[0],0,1,0,0});
    mVertices.push_back(Vertex{x1[1], y1[1],0,1,0,0});
    mVertices.push_back(Vertex{x1[2], y1[2],0,1,0,0});
    mVertices.push_back(Vertex{x1[3], y1[3],0,1,0,0});
    model.setToIdentity();

    for (int i = 0; i < 4; i++)
    {
        double t = 1.0;
        for (int j = 0; j < 4; j++)
        {
            A(i, j) = t;
            t *= x1[i];
        }
    }
    c = A.inverted() * QVector4D(y1[0], y1[1], y1[2], y1[3]);

    if (points)
        drawMethod = GL_POINTS;
    else
        drawMethod = GL_LINE_LOOP;

    indexed = false;
}

PolyInterpolation::~PolyInterpolation()
{

}

void PolyInterpolation::replace(double xMin, double xMax)
{
    mVertices.clear();
    double h = 0.05f;
    for (double x = xMin; x < xMax; x += h)
    {
        mVertices.push_back(Vertex{static_cast<float>(x),static_cast<float>(evaluate(x)),0,0,1,0});
    }
    model.setToIdentity();
}

double PolyInterpolation::evaluate(double x) const
{
    double t = 1.0;
    double y = 0.0;
    for (int i = 0; i < 4; i++) {
        y += t * c[i];
        t *= x;
    }
    return y;
}

std::pair<double, double> PolyInterpolation::range() const
{
    return std::make_pair(min, max);
}

void PolyInterpolation::toggleFunction()
{
    mVertices.clear();
    if (toggled)
    {
        mVertices.push_back(Vertex{x1[0], y1[0],0,1,0,0});
        mVertices.push_back(Vertex{x1[1], y1[1],0,1,0,0});
        mVertices.push_back(Vertex{x1[2], y1[2],0,1,0,0});
        mVertices.push_back(Vertex{x1[3], y1[3],0,1,0,0});
        for (int i = 0; i < 4; i++)
        {
            double t = 1.0;
            for (int j = 0; j < 4; j++)
            {
                A(i, j) = t;
                t *= x1[i];
            }
        }
        c = A.inverted() * QVector4D(y1[0], y1[1], y1[2], y1[3]);
    }
    else
    {
        mVertices.push_back(Vertex{x2[0], y2[0],0,1,0,0});
        mVertices.push_back(Vertex{x2[1], y2[1],0,1,0,0});
        mVertices.push_back(Vertex{x2[2], y2[2],0,1,0,0});
        mVertices.push_back(Vertex{x2[3], y2[3],0,1,0,0});
        for (int i = 0; i < 4; i++)
        {
            double t = 1.0;
            for (int j = 0; j < 4; j++)
            {
                A(i, j) = t;
                t *= x2[i];
            }
        }
        c = A.inverted() * QVector4D(y2[0], y2[1], y2[2], y2[3]);
    }
//    mMatrix.setToIdentity();
    replace(-3, 3);
    toggled = !toggled;
}

