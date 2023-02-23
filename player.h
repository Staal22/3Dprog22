#ifndef PLAYER_H
#define PLAYER_H

#include "visualobject.h"

class Player : public VisualObject
{
public:
    Player();
    ~Player() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z) override;
    void move(float x, float y, float z, GraphFunction* function) override;
    void turn(float y);

    int score = 0;

private:
    float mx{}, my{}, mz{}; //position
    GLuint mIBO{0};  // for glDrawElements()
    int m_recursions = 5;

    void octahedronUnitBall();
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void makeTriangle(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);

};

#endif // PLAYER_H
