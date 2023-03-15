#include "visualobject.h"

VisualObject::VisualObject()
{
    model.setToIdentity();
}

VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

std::pair<float, float> VisualObject::getPosition2D()
{
    auto col = mPosition.column(3);
    return std::pair<float,float>(col.x(), col.y());
}

QVector3D VisualObject::getPosition3D()
{
    auto col = mPosition.column(3);
    return QVector3D(col.x(), col.y(), col.z());
}
