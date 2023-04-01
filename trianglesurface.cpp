//#include <Open3D/Open3D.h>
#include "trianglesurface.h"
#include "qimage.h"
#include "qopengltexture.h"
#include <cstddef>

TriangleSurface::TriangleSurface(float size, int numVertices)
{
    if (numVertices == 4)
    {
        float halfSize = size / 2.0f;

        // Define the four corners of the plane
        v0 = QVector3D(-halfSize, 0.0f, -halfSize);
        v1 = QVector3D(halfSize, 0.0f, -halfSize);
        v2 = QVector3D(halfSize, 0.0f, halfSize);
        v3 = QVector3D(-halfSize, 0.0f, halfSize);

        // Add the vertices to the vertex buffer
        mVertices.push_back(Vertex(v0, QVector3D(1.0f, 1.0f, 1.0f)));
        mVertices.push_back(Vertex(v1, QVector3D(1.0f, 1.0f, 1.0f)));
        mVertices.push_back(Vertex(v2, QVector3D(1.0f, 1.0f, 1.0f)));
        mVertices.push_back(Vertex(v3, QVector3D(1.0f, 1.0f, 1.0f)));

        // Define the indices of the plane
        mIndices.push_back(0);
        mIndices.push_back(1);
        mIndices.push_back(2);
        mIndices.push_back(0);
        mIndices.push_back(2);
        mIndices.push_back(3);

        // calculate the minimum and maximum points of the bounding box
        min_ = QVector3D{0,0,0} - QVector3D(size / 2, 0.5f / 2, size / 2);
        max_ = QVector3D{0,0,0} + QVector3D(size / 2, 0.5f / 2, size / 2);
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
                mVertices.push_back(Vertex{QVector3D(x, 0, z), QVector2D(u, v)});  // centered at (0,0,0)
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

    model.setToIdentity();
    //    model.translate(- size / 2, 0.0f, - size / 2);

    hasHeightMap = true;
    computeVertexNormals();
    drawMethod = GL_TRIANGLES;
    texturepath = "heightmap.bmp";
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

void TriangleSurface::changeTerrain()
{
    // Load the image using QImage
    QImage image;
    image.load("heightmap.bmp");

    // Create an OpenGL texture object and bind the image to it
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setData(image);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);

    texture->bind(1);
}

// TODO Make this a class or header
//Eigen::Vector3d QVector3DToEigen(const QVector3D& v) {
//    return Eigen::Vector3d(v.x(), v.y(), v.z());
//}
//std::vector<Eigen::Vector3d> QVector3DArrayToEigen(const std::vector<QVector3D>& vertices) {
//    std::vector<Eigen::Vector3d> result;
//    result.reserve(vertices.size());
//    for (const auto& v : vertices) {
//        result.push_back(QVector3DToEigen(v));
//    }
//    return result;
//}
//Eigen::Vector3i Int3ToEigen(const std::vector<unsigned int>& v) {
//    return Eigen::Vector3i(v[0], v[1], v[2]);
//}
//std::vector<Eigen::Vector3i> Int3ArrayToEigen(const std::vector<unsigned int>& indices) {
//    std::vector<Eigen::Vector3i> result;
//    result.reserve(indices.size() / 3);
//    for (int i = 0; i < indices.size(); i += 3) {
//        result.push_back(Int3ToEigen(std::vector<unsigned int>{indices[i], indices[i+1], indices[i+2]}));
//    }
//    return result;
//}

//void TriangleSurface::subdivide(int subdivisions)
//{
//    // create a mesh object with vertices and faces
//    open3d::geometry::TriangleMesh mesh;
//    std::vector<QVector3D> vertPos;

//    for (auto& vertex : mVertices){
//        vertPos.push_back(vertex.m_xyz);
//    }
//    std::vector<Eigen::Vector3d> eigen_vertices = QVector3DArrayToEigen(vertPos);
//    mesh.vertices_ = eigen_vertices;

//    std::vector<Eigen::Vector3i> eigen_triangles = Int3ArrayToEigen(mIndices);
//    mesh.triangles_ = eigen_triangles;

//    // apply subdivision to the mesh
//    std::shared_ptr<open3d::geometry::TriangleMesh> smooth_mesh_ptr = mesh.SubdivideLoop(subdivisions);
//    open3d::geometry::TriangleMesh smooth_mesh(*smooth_mesh_ptr);

//    // get the new vertices and faces
//    auto new_vertices = smooth_mesh.vertices_;
//    auto new_faces = smooth_mesh.triangles_;
//    mVertices.clear();
//    for (int i = 0; i < new_vertices.size(); ++i)
//    {
//        mVertices.push_back(Vertex(new_vertices[i].x(), new_vertices[i].y(), new_vertices[i].z()));
//    }
//    mIndices.clear();
//    for (int j = 0; j < new_faces.size(); ++j) {
//        mIndices.push_back(new_faces[j].x());
//        mIndices.push_back(new_faces[j].y());
//        mIndices.push_back(new_faces[j].z());
//    }
//    init();
//}

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
