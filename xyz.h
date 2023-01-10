#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"

class XYZ : protected QOpenGLFunctions_4_1_Core
{
public:
    XYZ();

    float noseLength;
    std::vector<Vertex> mVertices;
    //void setContext(QOpenGLContext *context);
    void init(GLuint mVAO, GLuint mVBO);
    void draw();
};

#endif // XYZ_H
