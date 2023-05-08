#include "trianglesurface.h"

TriangleSurface::TriangleSurface(float size, int numVertices)
{
    float y = -3;

    float halfSize = size / 2.0f;
    if (numVertices == 4)
    {
        // Define the four corners of the plane
        v0 = QVector3D(-halfSize, y, -halfSize);
        v1 = QVector3D(halfSize, y, -halfSize);
        v2 = QVector3D(halfSize, y, halfSize);
        v3 = QVector3D(-halfSize, y, halfSize);

        // Add the vertices to the vertex buffer
        mVertices.push_back(Vertex(v0, QVector3D(0, 1.0f, 0)));
        mVertices.push_back(Vertex(v1, QVector3D(0, 1.0f, 0)));
        mVertices.push_back(Vertex(v2, QVector3D(0, 1.0f, 0)));
        mVertices.push_back(Vertex(v3, QVector3D(0, 1.0f, 0)));

        // Define the indices of the plane
        mIndices.push_back(0);
        mIndices.push_back(1);
        mIndices.push_back(2);
        mIndices.push_back(0);
        mIndices.push_back(2);
        mIndices.push_back(3);
    }

    else
    {
        int numVertices = size * size;  // number of vertices in the plane
        int numIndices = (size - 1) * (size - 1) * 6;  // number of indices in the plane

        // create vector of vertices
        mVertices.reserve(numVertices);
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                float x = ((j / (size-1.0f)) - 0.5f) * size;
                float z = ((i / (size-1.0f)) - 0.5f) * size;
                float u = j / (float)(size - 1);
                float v = i / (float)(size - 1);
                mVertices.push_back(Vertex{QVector3D(x, y, z), QVector3D(0, 1.0f, 0), QVector2D(u, v)});  // centered at (0,0,0)
            }
        }

        // create vector of indices
        mIndices.reserve(numIndices);
        for (int i = 0; i < size-1; ++i) {
            for (int j = 0; j < size-1; ++j) {
                int index = i * size + j;
                mIndices.push_back(index);
                mIndices.push_back(index + 1);
                mIndices.push_back(index + size);
                mIndices.push_back(index + size + 1);
                mIndices.push_back(index + size);
                mIndices.push_back(index + 1);
            }
        }
    }

    objectColor = QVector3D(0,0.42f,0);

    model.setToIdentity();
    //    model.translate(- size / 2, 0.0f, - size / 2);

    hasHeightMap = true;
    drawMethod = GL_TRIANGLES;
    texturepath = "../3DProg22/Textures/heightmap.bmp";

    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{0,0,0} - QVector3D(halfSize, 0.1f / 2, halfSize);
    max_ = QVector3D{0,0,0} + QVector3D(halfSize, 0.1f / 2, halfSize);
}

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

void TriangleSurface::rotate()
{
    model.rotate(2.f, 0.f, 1.f, 0.f);
}

bool TriangleSurface::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
            point.y() >= min_.y() && point.y() <= max_.y() &&
            point.z() >= min_.z() && point.z() <= max_.z();
}

bool TriangleSurface::intersectsLine(QVector3D start, QVector3D end) const
{
    // check if the line segment intersects the x,y,z planes of the bounding box
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

QVector3D TriangleSurface::surfaceIntersection(const QVector3D &start, const QVector3D &end, const QVector3D &surfaceNormal)
{
    // Calculate the direction vector of the line segment
    QVector3D segmentDir = end - start;

    // Calculate the distance from the start point to the intersection point
    float t = QVector3D::dotProduct(surfaceNormal, start) / QVector3D::dotProduct(surfaceNormal, segmentDir);

    // Calculate the intersection point
    QVector3D intersection = start + t * segmentDir;

    return intersection;
}

QVector3D TriangleSurface::barycentric(QVector2D A, QVector2D B, QVector2D C, QVector2D P)
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
