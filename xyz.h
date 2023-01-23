#ifndef XYZ_H
#define XYZ_H

#include "visualobject.h"

class XYZ : public VisualObject
{
public:
    XYZ();
    ~XYZ() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;
};

#endif // XYZ_H
