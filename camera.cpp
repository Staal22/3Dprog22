#include "camera.h"

Camera::Camera()
{
    translate(initPos.x(), initPos.y(), initPos.z());
}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform)
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
    mPmatrixUniform = pMatrixUniform;
    mVmatrixUniform = vMatrixUniform;
}

void Camera::perspective(int fov, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(fov, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
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

void Camera::setPos(float x, float y, float z)
{
    mEye.setX(x);
    mEye.setY(y);
    mEye.setZ(z);
}

void Camera::togglePerspective()
{
    if(thirdPerson)
    {
        translate(-1 * initPos.x(), -1 * initPos.y(), -1 * initPos.z());
        translate(0, 1, 0);
        thirdPerson = false;
    }
    else
    {
        translate(initPos.x(), initPos.y(), initPos.z());
        translate(0, -1, 0);
        thirdPerson = true;
    }
}

QMatrix4x4 Camera::getProjecionMatrix()
{
    return mPmatrix;
}

QMatrix4x4 Camera::getViewMatrix()
{
    return mVmatrix;
}
