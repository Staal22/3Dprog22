#ifndef HOUSE_H
#define HOUSE_H

#include "visualobject.h"

class House : public VisualObject
{
public:
    House();
    ~House() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void open();

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;

private:
    void houseVertices();

    // custom bounding box
    QVector3D min_;
    QVector3D max_;
    QVector3D door1;
    QVector3D door2;
    QVector3D door3;
    QVector3D door4;

    GLuint mIBO{0};

};

#endif // HOUSE_H
