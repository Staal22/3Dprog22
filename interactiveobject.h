#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;
    void move(float x, float y, float z) override;

private:
    float mx{}, my{}, mz{}; //position
};

#endif // INTERACTIVEOBJECT_H
