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
    void rotate() override;
    virtual std::pair<double, double> range() const override;
    float numericIntegral();
    double evaluate (double x, double z) const override;

    bool intersectsLine(QVector3D start, QVector3D end) const;
    QVector3D barycentric(QVector2D A, QVector2D B, QVector2D C, QVector2D P);

protected:
    // custom bounding box
    QVector3D min_;
    QVector3D max_;

private:
    float functionNumeric(double x, double z);
    float funcX(float x, float z);
    float funcY(float x, float z);

    float min = -20.f;
    float max = 20.0f;
    float h = 0.5f;

};

#endif // TWOVARIABLEFUNCTIONSPACE_H
