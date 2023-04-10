#include "twovariablefunctionspace.h"

TwoVariableFunctionSpace::TwoVariableFunctionSpace()
{
    // Keep track of the last index that was used for vertices
    int lastIndex = 0;

    // Loop over z values
    for (float x = min; x < max; x += h)
    {
        // Loop over x values
        for (float z = min; z < max; z += h)
        {
            // Evaluate the function
            float y1 = evaluate(x, z);
            float y2 = evaluate(x + h, z);
            float y3 = evaluate(x, z + h);
            float y4 = evaluate(x + h, z + h);

            // Create the vertices and add them to a mesh
            Vertex v1 = {x, y1, z};
            Vertex v2 = {x + h, y2, z};
            Vertex v3 = {x, y3, z + h};
            Vertex v4 = {x + h, y4, z + h};
            mVertices.push_back(v1);
            mVertices.push_back(v2);
            mVertices.push_back(v3);
            mVertices.push_back(v4);

//            mIndices.push_back(lastIndex);
//            mIndices.push_back(lastIndex + 1);
//            mIndices.push_back(lastIndex + 2);
//            mIndices.push_back(lastIndex + 1);
//            mIndices.push_back(lastIndex + 3);
//            mIndices.push_back(lastIndex + 2);
            mIndices.push_back(lastIndex);
            mIndices.push_back(lastIndex + 2);
            mIndices.push_back(lastIndex + 1);
            mIndices.push_back(lastIndex + 1);
            mIndices.push_back(lastIndex + 2);
            mIndices.push_back(lastIndex + 3);

            lastIndex += 4; // Update the last index
        }
    }

    model.setToIdentity();

    drawMethod = GL_TRIANGLES;

    int halfsize = (max - min) / 2;
    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{0,0,0} - QVector3D(halfsize, 0.5f / 2, halfsize);
    max_ = QVector3D{0,0,0} + QVector3D(halfsize, 0.5f / 2, halfsize);

    objectColor = QVector3D(0,0.42f,0);
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
    return sin(M_PI * x/3) * sin(M_PI * z/3); //Oppgave 1
}

bool TwoVariableFunctionSpace::intersectsLine(QVector3D start, QVector3D end) const
{
    float tmin = (min_.x() - start.x()) / (end.x() - start.x());
    float tmax = (max_.x() - start.x()) / (end.x() - start.x());
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min_.y() - start.y()) / (end.y() - start.y());
    float tymax = (max_.y() - start.y()) / (end.y() - start.y());
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (min_.z() - start.z()) / (end.z() - start.z());
    float tzmax = (max_.z() - start.z()) / (end.z() - start.z());
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    return true;
}

QVector3D TwoVariableFunctionSpace::barycentric(QVector2D A, QVector2D B, QVector2D C, QVector2D P)
{
    QVector2D v0 = B - A, v1 = C - A, v2 = P - A;
    float d00 = QVector2D::dotProduct(v0, v0);
    float d01 = QVector2D::dotProduct(v0, v1);
    float d11 = QVector2D::dotProduct(v1, v1);
    float d20 = QVector2D::dotProduct(v2, v0);
    float d21 = QVector2D::dotProduct(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0 - v - w;
    return QVector3D(u, v, w);
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
