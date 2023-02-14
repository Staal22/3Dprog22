#ifndef POLYINTERPOLATION_H
#define POLYINTERPOLATION_H

#include "visualobject.h"

class PolyInterpolation : public VisualObject
{
public:
    PolyInterpolation(bool inPoints = false);
    ~PolyInterpolation() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;

    void replace(double xMin, double xMax);

private:
    double evaluate(double x);

    QVector<double> x = {-2, -1, .23f, 2.1f};
    QVector<double> y = {1, .66f, 1.49f, 3};
    QMatrix4x4 A;
    QVector4D c;

    bool points = false;
};

#endif // POLYINTERPOLATION_H
