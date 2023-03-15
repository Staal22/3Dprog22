#ifndef POLYINTERPOLATION_H
#define POLYINTERPOLATION_H

#include "graphfunction.h"

class PolyInterpolation : public GraphFunction
{
public:
    PolyInterpolation(bool inPoints = false);
    ~PolyInterpolation() override;

    void init() override;
    void draw(GLint shader) override;
    void replace(double xMin, double xMax);
    double evaluate(double x) const override;
    virtual std::pair<double, double> range() const override;
    void toggleFunction();

private:
    const QVector<float> x1 = {-2, -1, .23f, 2.1f};
    const QVector<float> y1 = {1, .66f, 1.49f, 3};
    const QVector<float> x2 = {-2, -1, 1, 2.1f};
    const QVector<float> y2 = {0, 3.1f, 2.23f, 5};
    QMatrix4x4 A;
    QVector4D c;

    double min = -3.0f;
    double max = 3.1f;

    bool points = false;
    bool toggled = false;
};

#endif // POLYINTERPOLATION_H
