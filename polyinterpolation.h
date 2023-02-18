#ifndef POLYINTERPOLATION_H
#define POLYINTERPOLATION_H

#include "graphfunction.h"

class PolyInterpolation : public GraphFunction
{
public:
    PolyInterpolation(bool inPoints = false);
    ~PolyInterpolation() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void replace(double xMin, double xMax);
    double evaluate(double x) const override;
    virtual std::pair<double, double> range() const override;

private:
    QVector<double> x = {-2, -1, .23f, 2.1f};
    QVector<double> y = {1, .66f, 1.49f, 3};
    QMatrix4x4 A;
    QVector4D c;

    double min = -3.0f;
    double max = 3.1f;

    bool points = false;
};

#endif // POLYINTERPOLATION_H
