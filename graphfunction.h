#ifndef GRAPHFUNCTION_H
#define GRAPHFUNCTION_H

#include "visualobject.h"

class GraphFunction : public VisualObject
{
public:
    GraphFunction();
    ~GraphFunction() override;

    virtual double evaluate(double x) const { }
    virtual double evaluate (double x, double y) const { }
    virtual std::pair<double, double> range() const { };
};

#endif // GRAPHFUNCTION_H
