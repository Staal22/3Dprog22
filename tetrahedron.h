#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "visualobject.h"

class Tetrahedron : public VisualObject
{
public:
    Tetrahedron();
    ~Tetrahedron() override;

    void rotate() override;
    void move(float dt, GraphFunction* function) override;
    void writeFile(std::string filename);

protected:
    GLuint mIBO{0};                 // for glDrawElements()

};

#endif // TETRAHEDRON_H
