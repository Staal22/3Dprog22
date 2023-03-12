#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface(float size = 0);
//    TriangleSurface(std::string filename);
    ~TriangleSurface() override;

    void readFile(std::string filename, bool IndexedVertices);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;

    void subDivide();

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;
    // check if a line segment intersects the bounding box
    bool intersectsLine(QVector3D start, QVector3D end) const;
    QVector3D surfaceIntersection(const QVector3D& start, const QVector3D& end, const QVector3D& surfaceNormal);

protected:
    GLuint mIBO{0};                 // for glDrawElements()
    // custom bounding box
    QVector3D min_;
    QVector3D max_;

};

#endif // TRIANGLESURFACE_H
