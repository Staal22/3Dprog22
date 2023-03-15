#ifndef TWOVARIABLEFUNCTIONSPACE_H
#define TWOVARIABLEFUNCTIONSPACE_H

#include "graphfunction.h"

class TwoVariableFunctionSpace : public GraphFunction
{
public:
    TwoVariableFunctionSpace();
    TwoVariableFunctionSpace(std::string filename);
    ~TwoVariableFunctionSpace() override;

    void writeFile(std::string filename);
//    void readFile(std::string filename);
    void init() override;
    void draw(GLint shader) override;
    void rotate() override;
    virtual std::pair<double, double> range() const override;
    float numericIntegral();
    double evaluate (double x, double z) const override;

private:
    float functionNumeric(double x, double z);
    float funcX(float x, float z);
    float funcY(float x, float z);

    float min = -20.f;
    float max = 20.0f;
    float h = 0.05f;

};

#endif // TWOVARIABLEFUNCTIONSPACE_H
