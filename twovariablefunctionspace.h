#ifndef TWOVARIABLEFUNCTIONSPACE_H
#define TWOVARIABLEFUNCTIONSPACE_H

#include "visualobject.h"

class TwoVariableFunctionSpace : public VisualObject
{
public:
    TwoVariableFunctionSpace();
    TwoVariableFunctionSpace(std::string filename);
    ~TwoVariableFunctionSpace() override;

    void toFile(std::string filename);
//    void readFile(std::string filename);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;

    float numericIntegral();

private:
    float function(float x, float y);
    float functionNumeric(double x, double y);
    float funcX(float x, float y);
    float funcY(float x, float y);

    float xMin = -1.f;
    float xMax = 1.0f;
    float yMin = -1.f;
    float yMax = 1.0f;
    float h = 0.01f;
    float z = 0.f;


};

#endif // TWOVARIABLEFUNCTIONSPACE_H
