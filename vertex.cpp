#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;

}

Vertex::Vertex(QVector3D pos, QVector3D normal)
{
    m_xyz[0] = pos.x();
    m_xyz[1] = pos.y();
    m_xyz[2] = pos.z();
    m_normal[0] = normal.x();
    m_normal[1] = normal.y();
    m_normal[2] = normal.z();
}

Vertex::Vertex(float x, float y, float z, float u, float v)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_uv[0] = u;
    m_uv[1] = v;
}

Vertex::Vertex(QVector3D pos, QVector2D uv)
{
    m_xyz[0] = pos.x();
    m_xyz[1] = pos.y();
    m_xyz[2] = pos.z();
    m_uv[0] = uv.x();
    m_uv[1] = uv.y();
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
    m_uv[0] = u;
    m_uv[1] = v;
}

Vertex::Vertex(QVector3D pos, QVector3D normal, QVector2D uv)
{
    m_xyz[0] = pos.x();
    m_xyz[1] = pos.y();
    m_xyz[2] = pos.z();
    m_normal[0] = normal.x();
    m_normal[1] = normal.y();
    m_normal[2] = normal.z();
    m_uv[0] = uv.x();
    m_uv[1] = uv.y();
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_uv[0] << ", " << v.m_uv[1] << ") ";
  os << std::endl;
  return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
// dummy variables for parentheses and commas
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
  return is;
}
