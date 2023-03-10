#ifndef TROPHY_H
#define TROPHY_H

#include "visualobject.h"

class Trophy : public VisualObject
{
public:
    Trophy(float x = 0, float z = 0);
    ~Trophy() override;

    void init(GLint matrixUniform) override;
    void draw() override;

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;

private:
    // Texture
    QOpenGLTexture* texture;

    float radius = 1.0f;
    float height = 2.0f;
    int numSlices = 20;

    // custom bounding box
    QVector3D min_;
    QVector3D max_;

    GLuint mIBO{0};

};

#endif // TROPHY_H
