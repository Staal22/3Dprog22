#include "camera.h"

Camera::Camera()
{

}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform)
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
    mPmatrixUniform = pMatrixUniform;
    mVmatrixUniform = vMatrixUniform;
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
    //invertering?
    mVmatrix.setToIdentity();
    mVmatrix.lookAt(eye, at, up);
    mVmatrix.inverted();
}

void Camera::update()
{
    initializeOpenGLFunctions();
    glUniformMatrix4fv(mPmatrixUniform, 1, GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv(mVmatrixUniform, 1, GL_FALSE, mVmatrix.constData());

}

void Camera::translate(float dx, float dy, float dz)
{
    mEye.setX(mEye.x() + dx);
    mEye.setY(mEye.y() + dy);
    mEye.setZ(mEye.z() + dz);
}
