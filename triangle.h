#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "visualobject.h"

class Triangle : public VisualObject
{
public:
    Triangle();
    ~Triangle() override;
    void init() override;
    void rotate() override;
};

#endif // TRIANGLE_H
