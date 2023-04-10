#include "disc.h"

Disc::Disc()
{
    mVertices.push_back(Vertex{0,0,0,1,1,1});
    for (int i=0; i<13; i++)
    {
        double angle = 30 * i * M_PI / 180;
        qDebug() << "angle = " << angle;
        float x = cos(angle);
        float y = sin(angle);
        float z = 0.0f;
        float r = i%2;
        float g = 0.f;
        float b = 0.5f;
        mVertices.push_back(Vertex{x,y,z,r,g,b});
    }
    for (GLuint i=0; i<14; i++) mIndices.push_back(i);

    drawMethod = GL_TRIANGLE_FAN;
}

Disc::~Disc()
{

}

void Disc::move(float dt)
{
    // movement
    QVector3D ds=mVelocity*dt;
    model.translate(ds.x(), ds.y(), ds.z());	// hvis mPosisjon er Matrix4x4
    // normalen kan generelt være en parameter inn
    QVector3D normal = QVector3D{0.0f, 1.0f, 0.0f};
    // bruker kryssprodukt for å finne rotasjonsvektor
    QVector3D rotation = QVector3D::crossProduct(normal, mVelocity);
    rotation.normalize();

    // bruk formelen ds = r dt ==> dt = ds/r
    // for å finne ut hvor mye hjulet har rotert
    // og oppdater rotasjonsmatrisen
    // husk å starte med mRotation som identitetsmatrise
    //rotation
    float degrees = (180 * dt) / M_PI;
    mRotation.rotate(degrees, 0, 0, 1);

    model *= mRotation;		// hvis mPosition og mRotation er Matrix4x4
}

void Disc::writeFile(std::string filnavn)
{

}
