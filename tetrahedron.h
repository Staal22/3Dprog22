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

protected:
    std::vector<GLuint> mIndices; 	// Til indeksering
    GLuint mIBO{0};			// for glDrawElements()

};

#endif // TETRAHEDRON_H
