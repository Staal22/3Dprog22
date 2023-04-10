#ifndef TROPHY_H
#define TROPHY_H

#include "visualobject.h"

class Trophy : public VisualObject
{
public:
    Trophy(float x = 0, float y = 0, float z = 0);
    ~Trophy() override;

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;

private:

    // custom bounding box
    QVector3D min_;
    QVector3D max_;

};

#endif // TROPHY_H
