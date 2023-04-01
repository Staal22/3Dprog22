#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "visualobject.h"

class LightSource : public VisualObject
{
public:
    LightSource();
    ~LightSource();

    QVector3D lightColor{1.0f, 1.0f, 1.0f};
};

#endif // LIGHTSOURCE_H
