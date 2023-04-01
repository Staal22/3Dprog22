#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"

class TriangleSurface : public VisualObject
{    
public:
    TriangleSurface(float size = 50, int numVertices = 4);
//    TriangleSurface(std::string filename);
    ~TriangleSurface() override;

    void readFile(std::string filename, bool IndexedVertices);
    void rotate() override;
//    void subdivide(int subdivisions);

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;
    // check if a line segment intersects the bounding box
    bool intersectsLine(QVector3D start, QVector3D end) const;
    QVector3D surfaceIntersection(const QVector3D& start, const QVector3D& end, const QVector3D& surfaceNormal);
    QVector3D barycentric(QVector2D A, QVector2D B, QVector2D C, QVector2D P);

    class QOpenGLTexture* texture;
protected:
    // custom bounding box
    QVector3D min_;
    QVector3D max_;

private:
    // Define the four corners of the plane
    QVector3D v0;
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;

};

#endif // TRIANGLESURFACE_H
