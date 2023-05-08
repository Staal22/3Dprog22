#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "visualobject.h"

class LightSource : public VisualObject
{
public:
    LightSource(float x = 0, float y = 0, float z = 0);
    ~LightSource();

    void ToggleOnOff();

    QVector3D initialColor{.6f, .6f, .6f};
    QVector3D lightColor{};
    bool on = true;

};

#endif // LIGHTSOURCE_H
