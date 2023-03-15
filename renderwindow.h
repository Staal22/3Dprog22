#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include <unordered_map>
#include "camera.h"
#include "objectgroup.h"
#include "trophy.h"
#include "visualobject.h"
#include "quadtree.h"
//#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    bool mRotate{false};     //Check if triangle should rotate

private slots:
    void render();          //the actual render - function


public:
    GLint modelMatrixUniform;              //OpenGL reference to the Uniform in the shader program
    GLint projectionMatrixUniform;
    GLint viewMatrixUniform;

    Camera mCamera;

private:
    void init();            //initialize things we need before rendering

    QOpenGLShaderProgram* vertexShader;
    QOpenGLShaderProgram* plainShader;
    QOpenGLShaderProgram* textureShader;
    QOpenGLShaderProgram* terrainShader;

    std::vector<ObjectGroup*> groups;
    ObjectGroup* plainObjects;
    ObjectGroup* texturedObjects;
    ObjectGroup* terrainObjects;

    // Containers
    std::vector<VisualObject*> mObjects;
    std::unordered_map<std::string, VisualObject*> mMap;
    gsml::QuadTree<std::string, VisualObject*> mQuadTree;
    std::vector<Trophy*> trophies;

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    Shader *mShaderProgram{nullptr};    //holds pointer the GLSL shader program

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

protected:
  
    //The QWindow that we inherit from have these functions to capture mouse and keyboard.
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
