#include "player.h"

Player::Player()
{
    mVertices.reserve(3 * 8 * pow(4, m_recursions));
    octahedronUnitBall();
    mMatrix.setToIdentity();
}

Player::~Player()
{

}

void Player::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    mRotation.setToIdentity();
    glBindVertexArray(0);
}

void Player::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Player::move(float x, float y, float z)
{
//    mPosition.column(3).setX(mPosition.column(3).x() + x);
//    mPosition.column(3).setY(mPosition.column(3).y() + z);
//    mPosition.column(3).setZ(mPosition.column(3).y() + z);
    mPosition.translate(x, y, z);
    mMatrix.translate(x, y, z);
//    qDebug() << "Moved player and updated position to " << mPosition.column(3).x() << mPosition.column(3).y() << mPosition.column(3).z();
//    for (int i = 0; i < mVertices.size(); i++)
//    {
//        QVector4D transformedVertex = mMatrix * QVector4D(mVertices[i].m_xyz, 1.0f);
//        mVertices[i].m_xyz = QVector3D(transformedVertex);
//    }
}

void Player::turn(float y)
{
    mMatrix.rotate(y, 0.f, 1.f, 0.f);
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
    Vertex v{v1.x(), v1.y(), v1.z(), v1.x(), v1.y(), v1.z()};
    mVertices.push_back(v);
    v = Vertex{v2.x(), v2.y(), v2.z(), v2.x(), v2.y(), v2.z()};
    mVertices.push_back(v);
    v = Vertex{v3.x(), v3.y(), v3.z(), v3.x(), v3.y(), v3.z()};
    mVertices.push_back(v);
}
