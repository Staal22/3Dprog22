#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "visualobject.h"

class LightSource : public VisualObject
{
public:
    LightSource(float x = 0, float y = 0, float z = 0);
    ~LightSource();

    QVector3D lightColor{.6f, .6f, .6f};
};

#endif // LIGHTSOURCE_H
