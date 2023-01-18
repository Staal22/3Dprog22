#ifndef TWOVARIABLEFUNCTIONSPACE_H
#define TWOVARIABLEFUNCTIONSPACE_H

#include "visualobject.h"

class TwoVariableFunctionSpace : public VisualObject
{
public:
    TwoVariableFunctionSpace();
    TwoVariableFunctionSpace(std::string filename);
    ~TwoVariableFunctionSpace() override;

    float xMin = 0.f;
    float xMax = 1.0f;
    float yMin = 0.f;
    float yMax = 1.0f;
    float h = 0.01f;
    float z = 0.f;

    void ToFile(std::string filename);
    void readFile(std::string filename);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;
};

#endif // TWOVARIABLEFUNCTIONSPACE_H
