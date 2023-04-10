#include "player.h"
#include "graphfunction.h"

Player::Player()
{
    mVertices.reserve(3 * 8 * pow(4, m_recursions));
    octahedronUnitBall();
    model.setToIdentity();

    drawMethod = GL_TRIANGLES;
    indexed = false;
    objectColor = QVector3D(0, 0.24f, 0.66f);
}

Player::~Player()
{

}

void Player::move(float x, float y, float z)
{
    model.translate(x, y, z);
//    model = mPosition;
}

void Player::move(float x, float y, float z, GraphFunction *function)
{
    QVector4D pos4D = model.column(3);
    QVector3D pos3D = pos4D.toVector3D();
    float distance = 0.f;
    // movement
    float value = function->evaluate(pos3D.x(), pos3D.z());
    if (value > 0)
        distance = pos3D.distanceToPoint(QVector3D(pos3D.x(), value, pos3D.z()));
    if (pos3D.y() > value)
        distance *= -1;
    if (pos3D.x() > -5 &&
            pos3D.x() < 5 &&
            pos3D.z() > -10 &&
            pos3D.z() < 10)
        distance = 0;
    model.translate(x, distance, z);
    model *= mRotation;
}

void Player::turn(float y)
{
    model.rotate(y, 0.f, 1.f, 0.f);
}

void Player::octahedronUnitBall()
{
    QVector3D v0{0, 0, 1};
    QVector3D v1{1, 0, 0};
    QVector3D v2{0, 1, 0};
    QVector3D v3{-1, 0, 0};
    QVector3D v4{0, -1, 0};
    QVector3D v5{0, 0, -1};

    subDivide(v0, v1, v2, m_recursions);
    subDivide(v0, v2, v3, m_recursions);
    subDivide(v0, v3, v4, m_recursions);
    subDivide(v0, v4, v1, m_recursions);
    subDivide(v5, v2, v1, m_recursions);
    subDivide(v5, v3, v2, m_recursions);
    subDivide(v5, v4, v3, m_recursions);
    subDivide(v5, v1, v4, m_recursions);
}

void Player::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
    if (n>0)
    {
        QVector3D v1 = a+b; v1.normalize();
        QVector3D v2 = a+c; v2.normalize();
        QVector3D v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else
    {
        makeTriangle(a, b, c);
    }
}

void Player::makeTriangle(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    mVertices.push_back(Vertex(v1.x(), v1.y(), v1.z()));
    mVertices.push_back(Vertex(v2.x(), v2.y(), v2.z()));
    mVertices.push_back(Vertex(v3.x(), v3.y(), v3.z()));
}
