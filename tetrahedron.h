#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "visualobject.h"

class Tetrahedron : public VisualObject
{
public:
    Tetrahedron();
    ~Tetrahedron() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;
    void writeFile(std::string filename);

protected:
    GLuint mIBO{0};                 // for glDrawElements()

};

#endif // TETRAHEDRON_H
