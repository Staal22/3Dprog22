#ifndef LIGHTSWITCH_H
#define LIGHTSWITCH_H

#include "visualobject.h"
#include "lightsource.h"

class LightSwitch : public VisualObject
{
public:
    LightSwitch(float x = 0, float y = 0, float z = 0);
    ~LightSwitch();

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;

    void ToggleLight();

    LightSource* light;

private:
    // custom bounding box
    QVector3D min_;
    QVector3D max_;

};

#endif // LIGHTSWITCH_H
