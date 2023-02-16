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

protected:
    GLuint mIBO{0};                 // for glDrawElements()

};

#endif // TRIANGLESURFACE_H
