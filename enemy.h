#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"

class Enemy : public VisualObject
{
public:
    Enemy(float x = 0, float y = 0, float z = 0);
    ~Enemy() override;

    // check if a point is inside the bounding box
    bool contains(QVector3D point) const;

private:
    // custom bounding box
    QVector3D min_;
    QVector3D max_;

};

#endif // ENEMY_H
