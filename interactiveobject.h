#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void move(float x, float y, float z) override;

private:
    float mx{}, my{}, mz{}; //position
    GLuint mIBO{0};                 // for glDrawElements()

};

#endif // INTERACTIVEOBJECT_H
