#ifndef DISC_H
#define DISC_H

#include "visualobject.h"

class Disc : public VisualObject
{
public:
    Disc();
    ~Disc() override;

    void init() override;
//    void construct();
    void move(float dt) override;
    void writeFile(std::string filnavn);

protected:
    std::vector<GLuint> mIndices;   	// Til indeksering
    GLuint mIBO{0};                 	// for glDrawElements()
    float mRadius{0.5};

};

#endif // DISC_H
