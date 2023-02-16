#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <string>
#include "interactiveobject.h"
#include "octahedronball.h"
#include "parabolaapproximation.h"
#include "polyinterpolation.h"
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "tetrahedron.h"
#include "xyz.h"
#include "twovariablefunctionspace.h"
#include "curve.h"
#include "disc.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow) : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create())
    {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //This is the matrix used to transform (rotate) the triangle
    //You could do without, but then you have to simplify the shader and shader setup
//    mMVPmatrix = new QMatrix4x4{};
//    mMVPmatrix->setToIdentity();    //1, 1, 1, 1 in the diagonal of the matrix


    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    mMap.insert(std::pair<std::string, VisualObject*> {"xyz", new XYZ()});
    mMap.insert(std::pair<std::string, VisualObject*> {"disc", new class Disc()});

//    Tetrahedron* testahedron = new Tetrahedron();
//    testahedron->writeFile("tetrahedronVertices.txt");

//    mObjects.push_back(new OctahedronBall(5));

//    testObject = new InteractiveObject();
//    mObjects.push_back(testObject);

//    // Oblig 1 Matte
//    //Oppgave 1
//    TwoVariableFunctionSpace* tvSpace = new TwoVariableFunctionSpace();
//    tvSpace->writeFile("planeVertices.txt");
//    //Oppgave 2
//    Curve* curve = new Curve();
//    curve->writeFile("curveVertices.txt");
//    testCurve = new LineSurface("curveVertices.txt");
//    mObjects.push_back(testCurve);
//    //Oppgave 3
//    qDebug() << tvSpace->numericIntegral();

//    testPlane = new TriangleSurface();
//    testPlane->readFile("planeVertices.txt", false);
//    mObjects.push_back(testPlane);

    // Oblig 2 Matte
    // Oppgave 1
//    ParabolaApproximation* points = new ParabolaApproximation(true);
//    mObjects.push_back(points);
//    ParabolaApproximation* pApprox = new ParabolaApproximation();
//    pApprox->fit(points->mVertices);
//    pApprox->replace(-1, 11);
//    mObjects.push_back(pApprox);
//    // Oppgave 2
//    mObjects.push_back(new PolyInterpolation(true));
//    PolyInterpolation* pInterp = new PolyInterpolation();
//    pInterp->replace(-3, 3);
//    mObjects.push_back(pInterp);

    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4}; // må gjøres ordentlig
    mQuadTree.init(a, b, c, d);

    // do last
    std::string navn{"navn"}; // VisualObject should maybe have own name variable
    for (auto& pair : mMap)
    {
        mObjects.push_back(pair.second);
    }
    for (auto& object : mObjects)
    {
        mQuadTree.insert(object->getPosition2D(), navn, object);
    }
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //debug showcase
//    int* bingus = nullptr;
//    *bingus += 10;

    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag", "../3Dprog22/plainshader.geom");

    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader
    //mShaderProgram->use();
    mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mPmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pmatrix" );
    mVmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vmatrix" );

    for (auto& object : mObjects)
    {
        object->init(mMatrixUniform);
    }
    //canvas code
//    for (auto it=mObjects.begin(); it!= mObjects.end(); it++)
//        (*it)->init(mMatrixUniform);

    glBindVertexArray(0);       //unbinds any VertexArray - good practice

    mCamera.translate(0, 0, 15);
    glPointSize(5);
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
//    Disc->move(1);
    mMap["disc"]->move(0.017f);

    mCamera.init(mPmatrixUniform, mVmatrixUniform);

//    qDebug() << *mPmatrix;
    mCamera.perspective(110.f, 16.0f/9.0f, 0.1f, 100.0f);
    mCamera.lookAt(mCamera.mEye, mCamera.mEye + QVector3D::crossProduct(mCamera.left, mCamera.up), mCamera.up);

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //what shader to use
    glUseProgram(mShaderProgram->getProgram() );

//    //Move camera
//    mVmatrix->translate(0, 5550, 0);
    mCamera.update();

//    for (auto& object : mObjects) {
//        object->draw();
//    }
    for (auto& object : mQuadTree.m_objects)
    {
        object.second->draw();
    }

    //canvas code
//    for (auto it=mObjects.begin(); it!= mObjects.end(); it++)
//        (*it)->draw();

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);

    if(mRotate)
    {
        for (auto& object : mObjects) {
            object->rotate();
        }
    }
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }
    //You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
//        if (testObject != nullptr)
//            testObject->move(0.f, moveDistance, 0.f);
        mCamera.translate(0,0,-1);
    }
    if(event->key() == Qt::Key_A)
    {
//        if (testObject != nullptr)
//            testObject->move(-moveDistance, 0.f, 0.f);
        mCamera.translate(-1,0,0);

    }
    if(event->key() == Qt::Key_S)
    {
//        if (testObject != nullptr)
//            testObject->move(0.f, -moveDistance, 0.f);
        mCamera.translate(0,0,1);
    }
    if(event->key() == Qt::Key_D)
    {
//        if (testObject != nullptr)
//            testObject->move(moveDistance, 0.0f, 0.f);
        mCamera.translate(1,0,0);
    }
    if(event->key() == Qt::Key_E)
    {
        mCamera.translate(0,1,0);
    }
    if(event->key() == Qt::Key_Q)
    {
        mCamera.translate(0,-1,0);
    }
    if(event->key() == Qt::Key_1)
    {
        if (mMap["testCurve"] != nullptr)
        {
            if (mMap["testCurve"]->hide)
                mMap["testCurve"]->hide = false;
            else
            {
                mMap["testCurve"]->hide = true;
            }
        }
    }
    if(event->key() == Qt::Key_2)
    {
        if (mMap["testPlane"] != nullptr)
        {
            if (mMap["testPlane"]->hide)
                mMap["testPlane"]->hide = false;
            else
            {
                mMap["testPlane"]->hide = true;
            }
        }
    }
}
