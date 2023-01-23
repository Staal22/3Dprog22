#ifndef LINESURFACE_H
#define LINESURFACE_H

#include "visualobject.h"

class LineSurface : public VisualObject
{
public:
    LineSurface();
    LineSurface(std::string filename);
    ~LineSurface();

    void readFile(std::string filename);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;
};

#endif // LINESURFACE_H
