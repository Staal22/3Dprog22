#ifndef CURVE_H
#define CURVE_H

#include "visualobject.h"

class Curve : public VisualObject
{
public:
    Curve();
    ~Curve() override;

    void toFile(std::string filename);
//    void readFile(std::string filename);
    void init(GLint shader) override;
    void draw() override;
    void rotate() override;

private:
    float function(int variables, float x, float y = 0.f);

    float xMin = -1.f;
    float xMax = 1.0f;
    float yMin = -1.f;
    float yMax = 1.0f;
    float h = 0.01f;
    float z = 0.f;
};

#endif // CURVE_H
