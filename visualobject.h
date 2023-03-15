#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   ~VisualObject();

   virtual void init()=0;
   virtual void draw(GLint shader)=0;
   virtual void rotate() { }
   // move for interaktive objekter
   virtual void move(float x, float y, float z) { }
   virtual void move(float x, float y, float z, class GraphFunction* function) { }
   // move for simulering (for eksempel NPC)
   virtual void move(float dt) { }
   // just a test please don't be mad Dag y.y
   virtual void move(float dt, class GraphFunction* function) { }

   std::pair<float,float> getPosition2D();
   QVector3D getPosition3D();

   bool hide = false;

public:
   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices; 	// Til indeksering
   GLuint mVAO{0};
   GLuint mVBO{0};
   GLint modelUniform{0};
   QMatrix4x4 model;

   // Velger å lagre posisjon, rotasjon og translasjon
   // i hver sin 4x4 matrise
   QMatrix4x4 mPosition{0,0,0};
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;
   // Legger til rette for simulering
   QVector3D mVelocity{-1, 0, 0};

   bool hasTexture = false;

};

#endif // VISUALOBJECT_H
