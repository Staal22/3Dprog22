#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "visualobject.h"

class LightSource : public VisualObject
{
public:
    LightSource(float x = 0, float y = 0, float z = 0);
    ~LightSource();

    QVector3D lightColor{1.0f, 1.0f, 1.0f};
};

#endif // LIGHTSOURCE_H
