#ifndef VERTEX_H
#define VERTEX_H

#include "qvectornd.h"
#include <iostream>
#include <fstream>

class Vertex {
   //! Overloaded ostream operator which writes all vertex data on an open textfile stream
   friend std::ostream& operator<< (std::ostream&, const Vertex&);

   //! Overloaded istream operator which reads all vertex data from an open textfile stream
   friend std::istream& operator>> (std::istream&, Vertex&);

public:
   QVector3D m_xyz{};       	// position in 3D
   QVector3D m_normal{1, 1, 1}; // normal in 3D or rgb colors
   QVector2D m_uv{};         	// texture coordinates (uv)

public:
   Vertex();
   Vertex(float x, float y, float z);
   Vertex(QVector3D pos);
   Vertex(float x, float y, float z, float r, float g, float b);
   Vertex(QVector3D pos, QVector3D normal);
   Vertex(float x, float y, float z, float u, float v);
   Vertex(QVector3D pos, QVector2D uv);
   Vertex(float x, float y, float z, float r, float g, float b, float u, float v);
   Vertex(QVector3D pos, QVector3D normal, QVector2D uv);
};

#endif // VERTEX_H
