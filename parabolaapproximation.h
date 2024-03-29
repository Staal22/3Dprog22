#ifndef PARABOLAAPPROXIMATION_H
#define PARABOLAAPPROXIMATION_H

#include "graphfunction.h"

class ParabolaApproximation : public GraphFunction
{
public:
    ParabolaApproximation(bool inPoints = false);
    ~ParabolaApproximation() override;

    void fit(const std::vector<Vertex> &points);
    void replace(double xMin, double xMax);

private:
    double evaluate(double x) const override;
    std::vector<double> solve(const std::vector<std::vector<double>> &A, const std::vector<double> &b);
    double dotProduct(const std::vector<std::vector<double>> &A, const std::vector<double> &b, int j);

    bool points = false;
    double a = 0, b = 0, c = 0;
};

#endif // PARABOLAAPPROXIMATION_H
