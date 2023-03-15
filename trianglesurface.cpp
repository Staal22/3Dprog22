#include "trianglesurface.h"
#include "qimage.h"
#include "qopengltexture.h"

TriangleSurface::TriangleSurface(float size)
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

    model.setToIdentity();
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

void TriangleSurface::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    mRotation.setToIdentity();

    glBindVertexArray(0);
}

void TriangleSurface::draw(GLint shader)
{
    modelUniform = shader;
    if (hide)
    {
        return;
    }
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( modelUniform, 1, GL_TRUE, model.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
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

void TriangleSurface::subdivide(int subdivisions)
{
    // Loop through each subdivision level
    for (int i = 0; i < subdivisions; i++) {
        // Store the number of indices before subdividing
        int numIndicesBefore = mIndices.size();

        // Loop through each pair of adjacent vertices
        for (int j = 0; j < numIndicesBefore; j+=3) {
            // Get the indices of the current triangle's vertices
            int i0 = mIndices[j];
            int i1 = mIndices[j+1];
            int i2 = mIndices[j+2];

            // Get the positions of the current triangle's vertices
            QVector3D v0 = mVertices[i0].m_xyz;
            QVector3D v1 = mVertices[i1].m_xyz;
            QVector3D v2 = mVertices[i2].m_xyz;

            // Calculate the midpoints of each edge
            QVector3D v01 = (v0 + v1) / 2.0f;
            QVector3D v12 = (v1 + v2) / 2.0f;
            QVector3D v20 = (v2 + v0) / 2.0f;

            // Add the new vertices to the vertex buffer
            mVertices.push_back(Vertex(v01, QVector3D(1.0f, 1.0f, 1.0f)));
            mVertices.push_back(Vertex(v12, QVector3D(1.0f, 1.0f, 1.0f)));
            mVertices.push_back(Vertex(v20, QVector3D(1.0f, 1.0f, 1.0f)));

            // Update the indices to form new triangles using the new vertices
            int i3 = mVertices.size() - 3;
            int i4 = mVertices.size() - 2;
            int i5 = mVertices.size() - 1;

            mIndices[j] = i0;
            mIndices[j+1] = i3;
            mIndices[j+2] = i5;

            mIndices.push_back(i3);
            mIndices.push_back(i1);
            mIndices.push_back(i4);

            mIndices.push_back(i5);
            mIndices.push_back(i4);
            mIndices.push_back(i2);
        }
    }
    init();
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
