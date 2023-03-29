#ifndef HOUSE_H
#define HOUSE_H

#include "visualobject.h"

class House : public VisualObject
{
public:
    House();
    ~House() override;

    void init() override;
    void draw(GLint shader) override;
    void open();
    void close();

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;
    bool doorContains(QVector3D point) const;

private:
    void houseVertices();

    // custom bounding box
    QVector3D min_;
    QVector3D max_;
    QVector3D doorMin_;
    QVector3D doorMax_;
    QVector3D door1;
    QVector3D door2;
    QVector3D door3;
    QVector3D door4;
    QVector3D corner1;
    QVector3D corner2;
    QVector3D corner3;
    QVector3D corner4;

    GLuint mIBO{0};

    bool doorOpen = false;

//    class QOpenGLTexture* texture;
};

#endif // HOUSE_H
