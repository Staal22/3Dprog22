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

   void init();
   void draw(GLint shader);
   virtual void rotate() { }
   // move for interaktive objekter
   virtual void move(float x, float y, float z) { }
   virtual void move(float x, float y, float z, class GraphFunction* function) { }
   // move for simulering (for eksempel NPC)
   virtual void move(float dt) { }
   // just a test please don't be mad Dag y.y
   virtual void move(float dt, class GraphFunction* function) { }

   void computeVertexNormals();

   std::pair<float,float> getPosition2D();
   QVector3D getPosition3D();
   QMatrix4x4 getModelMatrix();

   bool hide = false;
   bool indexed = true;
   int drawMethod = GL_TRIANGLES;

public:
   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices; 	// Til indeksering
   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mIBO{0};
   GLint modelUniform{0};
   QMatrix4x4 model;

   // Velger å lagre posisjon, rotasjon og translasjon
   // i hver sin 4x4 matrise
//   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;
   // Legger til rette for simulering
   QVector3D mVelocity{-1, 0, 0};

   bool hasTexture = false;
   bool hasHeightMap = false;

   class QOpenGLTexture* texture;
   QString texturepath;

   QVector3D objectColor{1,1,1};


};

#endif // VISUALOBJECT_H
