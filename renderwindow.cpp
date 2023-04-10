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
#include "house.h"
//#include "octahedronball.h"
//#include "parabolaapproximation.h"
#include "lightsource.h"
#include "player.h"
#include "polyinterpolation.h"
#include "mainwindow.h"
#include "logger.h"
#include "tetrahedron.h"
#include "trianglesurface.h"
#include "xyz.h"
#include "twovariablefunctionspace.h"
//#include "curve.h"
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

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    mMap.insert(std::pair<std::string, VisualObject*> {"xyz", new XYZ()});
    mMap.insert(std::pair<std::string, VisualObject*> {"disc", new class Disc()});
    mMap.insert(std::pair<std::string, VisualObject*> {"tetrahedron", new Tetrahedron()});
//    mMap.insert(std::pair<std::string, VisualObject*> {"floor", new TriangleSurface(60, 600)});
    mMap.insert(std::pair<std::string, VisualObject*>  {"oldFloor", new TwoVariableFunctionSpace()});
    mMap.insert(std::pair<std::string, VisualObject*>  {"player", new Player()});
    mMap.insert(std::pair<std::string, VisualObject*>  {"house", new House()});

    // Trophies
    mObjects.push_back(new Trophy(2.5f, 0, 4));
    mObjects.push_back(new Trophy(-7, 0, 2));
    mObjects.push_back(new Trophy(-9, 0, 4));
    mObjects.push_back(new Trophy(-11, 0, 6));
    mObjects.push_back(new Trophy(-11, 0, 2));
    mObjects.push_back(new Trophy(-9, 0, 1));

    light = new LightSource(-7, 3, 15);
    mObjects.push_back(light);

    // Oblig 1 Matte
    //Oppgave 1
    //    TwoVariableFunctionSpace* tvSpace = new TwoVariableFunctionSpace();
    //    tvSpace->writeFile("planeVertices.txt");
    //Oppgave 2
    //    Curve* curve = new Curve();
    //    curve->writeFile("curveVertices.txt");
    //    testCurve = new LineSurface("curveVertices.txt");
    //    mObjects.push_back(testCurve);
    //Oppgave 3
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

    // Oppgave 2
    mMap.insert(std::pair<std::string, VisualObject*> {"pInterp", new PolyInterpolation});
    static_cast<PolyInterpolation*>(mMap["pInterp"])->replace(-3, 3);

    //    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4}; // må gjøres ordentlig
    //    mQuadTree.init(a, b, c, d);

    // do last
    for (auto& pair : mMap)
    {
        mObjects.push_back(pair.second);
    }
    for (auto& object : mObjects)
    {
        if (dynamic_cast<Trophy*>(object) != nullptr)
        {
            trophies.push_back(static_cast<Trophy*>(object));
        }
    }

    //    std::string navn{"navn"}; // VisualObject should maybe have own name variable
    //    for (auto& object : mObjects)
    //    {
    //        mQuadTree.insert(object->getPosition2D(), navn, object);
    //    }

    // alternatively
    //    for (auto& pair : mMap)
    //    {
    //        mQuadTree.insert(pair.second->getPosition2D(), pair.first, pair.second);
    //    }
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests)
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
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    startOpenGLDebugger();

    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders: OLD CODE
    //    mShaderProgram = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag");

    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader
    //    mShaderProgram->use();
    //    mMatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "model");
    //    mPmatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "projection");
    //    mVmatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "view");

    //    vertexShader = new QOpenGLShaderProgram();
    //    vertexShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../3Dprog22/plainshader.vert");
    //    vertexShader->link();

    // Default plain shader - treats normal as rgb
    plainShader = new QOpenGLShaderProgram();
    plainShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../3Dprog22/plainshader.vert");
    plainShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../3Dprog22/plainshader.frag");
    plainShader->link();

    // Texture shader - supports textures
    textureShader = new QOpenGLShaderProgram();
    textureShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../3Dprog22/plainshader.vert");
    textureShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../3Dprog22/textureshader.frag");
    textureShader->link();

    // For rendering vertex normals
    vertexNormalShader = new QOpenGLShaderProgram();
    vertexNormalShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../3Dprog22/plainshader.vert");
    vertexNormalShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../3Dprog22/textureshader.frag");
    vertexNormalShader->addShaderFromSourceFile(QOpenGLShader::Geometry, "../3Dprog22/shader.geom");
    vertexNormalShader->link();

    modelMatrixUniform = glGetUniformLocation(plainShader->programId(), "model");
    projectionMatrixUniform = glGetUniformLocation(plainShader->programId(), "projection");
    viewMatrixUniform = glGetUniformLocation(plainShader->programId(), "view");

//    GLint location = glGetUniformLocation(shaderProgram, "myValue");


    plainObjects = new ObjectGroup(plainShader);
    texturedObjects = new ObjectGroup(textureShader);
    groups.push_back(plainObjects);
    groups.push_back(texturedObjects);

    for (auto& object : mObjects)
    {
        object->computeVertexNormals();
        object->init(); 
        if (object->hasTexture)
            texturedObjects->addObject(object);
        else
            plainObjects->addObject(object);
    }

    textureShader->bind();
    textureShader->setUniformValue("textureSampler", 0);
    textureShader->setUniformValue("heightmap", 1);
    textureShader->release();
    plainShader->bind();
    plainShader->setUniformValue("heightmap", 1);
    plainShader->release();

    glBindVertexArray(0);

    mCamera.translate(-15, 6, 15);
    glPointSize(5);
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //what shader to use OLD CODE
    //    glUseProgram(mShaderProgram->getProgram());

    for (auto& group : groups)
    {
        modelMatrixUniform = glGetUniformLocation(group->m_shaderProgram->programId(), "model");
        projectionMatrixUniform = glGetUniformLocation(group->m_shaderProgram->programId(), "projection");
        viewMatrixUniform = glGetUniformLocation(group->m_shaderProgram->programId(), "view");
        group->render(this);
    }

    // Quadtree working test
    //    for (auto& object : mQuadTree.m_objects)
    //    {
    //        object.second->draw();
    //    }

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

    Player* player = static_cast<Player*>(mMap["player"]);
    float rayLength = 100.0f;
    QVector3D playerPos = player->getPosition3D();
    QVector3D rayEnd = playerPos - QVector3D(0, rayLength, 0);
    House* house = static_cast<House*>(mMap["house"]);
//    TriangleSurface* floor = static_cast<TriangleSurface*>(mMap["floor"]);
    TwoVariableFunctionSpace* floor = static_cast<TwoVariableFunctionSpace*>(mMap["oldFloor"]);
    if (floor->intersectsLine(playerPos, rayEnd))
    {
        for (int i = 0; i < floor->mIndices.size(); i += 3)
        {
            QVector2D A(floor->mVertices[floor->mIndices[i  ]].m_xyz.x(), floor->mVertices[floor->mIndices[i  ]].m_xyz.z());
            QVector2D B(floor->mVertices[floor->mIndices[i+1]].m_xyz.x(), floor->mVertices[floor->mIndices[i+1]].m_xyz.z());
            QVector2D C(floor->mVertices[floor->mIndices[i+2]].m_xyz.x(), floor->mVertices[floor->mIndices[i+2]].m_xyz.z());

            // Calculate the barycentric coordinates of the player's position
            QVector3D bc = floor->barycentric(A, B, C, QVector2D(playerPos.x(), playerPos.z()));

            // Check if the player is above this triangle
            if (bc.x() >= 0 && bc.y() >= 0 && bc.x() + bc.y() <= 1)
            {
                QVector3D a = floor->mVertices[floor->mIndices[i  ]].m_xyz;
                QVector3D b = floor->mVertices[floor->mIndices[i+1]].m_xyz;
                QVector3D c = floor->mVertices[floor->mIndices[i+2]].m_xyz;

                // Interpolates to find height based on barycentric coordinates,
                float height = a.y() * bc.x() + b.y() * bc.y() + c.y() * bc.z();
//                height += 0.3f;
                float distance = playerPos.distanceToPoint(QVector3D(playerPos.x(), height, playerPos.z()));
                if (playerPos.y() < height)
                    distance *= -1;
                mMap["player"]->move(0, -distance, 0);
//                qDebug() << -distance;
            }
        }
    }
    if (house->contains(playerPos))
        mCamera.setPos(2.5f, 3, 8);
    if (house->doorContains(playerPos))
        house->open();
    else if (!house->doorContains(playerPos))
    {
        house->close();
        if (!house->contains(playerPos))
        {
            mCamera.setPos(-15, 6, 15);
        }
    }
    for (auto& trophy : trophies)
    {
        if (trophy->contains(playerPos) && !trophy->hide)
        {
            trophy->hide = true;
            player->score++;
            qDebug() << "Score is: " << player->score;
        }
    }


    // Hardcoded movement
    mMap["disc"]->move(0.017f);
    mMap["tetrahedron"]->move(-0.017f, static_cast<PolyInterpolation*>(mMap["pInterp"]));
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
    float moveDistance = 0.1f;
    // Move camera
    //    if(event->key() == Qt::Key_W)
    //    {
    //        mCamera.translate(0,0,-1);
    //    }
    //    if(event->key() == Qt::Key_A)
    //    {
    //        mCamera.translate(-1,0,0);
    //    }
    //    if(event->key() == Qt::Key_S)
    //    {
    //        mCamera.translate(0,0,1);
    //    }
    //    if(event->key() == Qt::Key_D)
    //    {
    //        mCamera.translate(1,0,0);
    //    }
    //    if(event->key() == Qt::Key_E)
    //    {
    //        mCamera.translate(0,1,0);
    //    }
    //    if(event->key() == Qt::Key_Q)
    //    {
    //        mCamera.translate(0,-1,0);
    //    }

    TwoVariableFunctionSpace* floor = static_cast<TwoVariableFunctionSpace*>(mMap["floor"]);
    // Move Player
    if(event->key() == Qt::Key_W)
    {
        if (mMap["player"] != nullptr)
            mMap["player"]->move(moveDistance, 0.f,  0.f/*, floor*/);
    }
    if(event->key() == Qt::Key_A)
    {
        if (mMap["player"] != nullptr)
            mMap["player"]->move(0.f, 0.f, -moveDistance/*, floor*/);
    }
    if(event->key() == Qt::Key_S)
    {
        if (mMap["player"] != nullptr)
            mMap["player"]->move(-moveDistance, 0.f, 0.f/*, floor*/);
    }
    if(event->key() == Qt::Key_D)
    {
        if (mMap["player"] != nullptr)
            mMap["player"]->move(0.f, 0.0f, moveDistance/*, floor*/);
    }
    if(event->key() == Qt::Key_E)
    {
        if (mMap["player"] != nullptr)
            static_cast<Player*>(mMap["player"])->turn(-5);
    }
    if(event->key() == Qt::Key_Q)
    {
        if (mMap["player"] != nullptr)
            static_cast<Player*>(mMap["player"])->turn(5);
    }
    if(event->key() == Qt::Key_Space)
    {
        if (mMap["player"] != nullptr)
            (mMap["player"])->move(0, 1, 0);
    }

    //Toggle objects
    if(event->key() == Qt::Key_1)
    {
        if (mMap["pInterp"] != nullptr)
        {
            static_cast<PolyInterpolation*>(mMap["pInterp"])->toggleFunction();
            mMap["pInterp"]->init();
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
