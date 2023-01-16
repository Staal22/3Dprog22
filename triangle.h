#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "visualobject.h"

class Triangle : public VisualObject
{
public:
    Triangle();
    ~Triangle() override;
    void init(GLint matrixUniform) override;
    void draw() override;

};

#endif // TRIANGLE_H
