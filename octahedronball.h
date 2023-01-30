#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"

class OctahedronBall : public VisualObject
{
public:
    OctahedronBall(int n);
    ~OctahedronBall() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;

private:
    int m_recursions;
    int m_index;               // brukes i rekursjon, til å bygge m_vertices
    void makeTriangle(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void octahedronUnitBall();
};

#endif // OCTAHEDRONBALL_H
