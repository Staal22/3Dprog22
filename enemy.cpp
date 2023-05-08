#include "enemy.h"
#include "objectloader.h"


Enemy::Enemy(float x, float y, float z)
{
    ObjectLoader loader;
    mVertices = loader.loadObjFile("../3DProg22/Objects/Car.obj");

    // calculate the minimum and maximum points of the bounding box
    min_ = QVector3D{x,y,z} - QVector3D(0.6f, 5, 1.4f);
    max_ = QVector3D{x,y,z} + QVector3D(0.6f, 5, 1.4f);

    model.translate(x, y, z);

    indexed = false;
    objectColor = QVector3D(0.7f, 0, 0);
}

Enemy::~Enemy()
{

}

bool Enemy::contains(QVector3D point) const
{
    return point.x() >= min_.x() && point.x() <= max_.x() &&
           point.y() >= min_.y() && point.y() <= max_.y() &&
           point.z() >= min_.z() && point.z() <= max_.z();
}
