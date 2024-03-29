#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QMatrix4x4>

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
   Camera();
   ~Camera() { }
   void init(GLint pMatrixUniform, GLint vMatrixUniform);
   void perspective(int degrees, double aspect, double nearplane, double farplane);
   void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);
   void update();
   void translate(float dx, float dy, float dz);
   void setPos(float x, float y, float z);
   void togglePerspective();

   QMatrix4x4 getProjecionMatrix();
   QMatrix4x4 getViewMatrix();

   QVector3D mEye;
   QVector3D left = {-1,0,0};
   QVector3D up = {0,1,0};
   QVector3D initPos{-5, 4, 5};

   bool thirdPerson = true;


private:
   GLint  mPmatrixUniform;        //OpenGL reference to the Uniform in the shader program
   GLint  mVmatrixUniform;        //OpenGL reference to the Uniform in the shader program

   QMatrix4x4 mPmatrix{};         // denne,
   QMatrix4x4 mVmatrix{};         // og denne, skal legges inn i kameraklasse

};

#endif // CAMERA_H
