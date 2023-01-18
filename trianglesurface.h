#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filename);
    ~TriangleSurface() override;
    void readFile(std::string filename);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;

};

#endif // TRIANGLESURFACE_H
