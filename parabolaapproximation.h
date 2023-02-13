#ifndef PARABOLAAPPROXIMATION_H
#define PARABOLAAPPROXIMATION_H

#include "visualobject.h"
#include <QVector>
#include <QPointF>

class ParabolaApproximation : public VisualObject
{
public:
    ParabolaApproximation();
//    ParabolaApproximation(double xMin, double xMax);
    ~ParabolaApproximation() override;

    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate() override;

    void fit(const std::vector<Vertex> &points);
    void replace(double xMin, double xMax);

private:
    std::vector<double> solve(const std::vector<std::vector<double>> &A, const std::vector<double> &b);
    double dotProduct(const std::vector<std::vector<double>> &A, const std::vector<double> &b, int j);

    // Evaluate the fitted parabola at a given x
    double evaluate(double x) const;


    double a = 0, b = 0, c = 0;
};

#endif // PARABOLAAPPROXIMATION_H
