#ifndef PLAYER_H
#define PLAYER_H

#include "interactiveobject.h"

class Player : public InteractiveObject
{
public:
    Player();
    ~Player() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z) override;
    void turn(float y);

private:
    float mx{}, my{}, mz{}; //position
    GLuint mIBO{0};                 // for glDrawElements()
};

#endif // PLAYER_H
