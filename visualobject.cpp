#include "visualobject.h"

VisualObject::VisualObject()
{
    mMatrix.setToIdentity();
}

VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}
