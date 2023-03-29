QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17 geometryshader

TARGET      = 3D-programmering

SOURCES += main.cpp \
    camera.cpp \
    curve.cpp \
    disc.cpp \
    graphfunction.cpp \
    house.cpp \
    interactiveobject.cpp \
    linesurface.cpp \
    logger.cpp \
    mainwindow.cpp \
    objectgroup.cpp \
    octahedronball.cpp \
    parabolaapproximation.cpp \
    player.cpp \
    polyinterpolation.cpp \
    renderwindow.cpp \
    shader.cpp \
    tetrahedron.cpp \
    texture.cpp \
    triangle.cpp \
    trianglesurface.cpp \
    trophy.cpp \
    twovariablefunctionspace.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    curve.h \
    disc.h \
    graphfunction.h \
    house.h \
    interactiveobject.h \
    linesurface.h \
    logger.h \
    mainwindow.h \
    objectgroup.h \
    octahedronball.h \
    parabolaapproximation.h \
    player.h \
    polyinterpolation.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    tetrahedron.h \
    texture.h \
    triangle.h \
    trianglesurface.h \
    trophy.h \
    twovariablefunctionspace.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainshader.frag \
    plainshader.gsh \
    plainshader.vert \
    shader.geom \
    textureshader.frag

## GLEW
#INCLUDEPATH += "E:/Misc/glew-2.1.0/include"
#LIBS += -L "E:/Misc/glew-2.1.0/lib" - lGLEW
## GLFW
#INCLUDEPATH += "E:/Misc/glfw-3.3.8.bin.WIN64/include"
#LIBS += -L "E:/Misc/glfw-3.3.8.bin.WIN64/lib-mingw-w64" - lglfw3
## FMT
#INCLUDEPATH += "E:/Misc/fmt-9.1.0/include"
## Eigen
#INCLUDEPATH += "E:/Misc/eigen-3.4.0"
## Open3D
#INCLUDEPATH += "E:/Misc/open3d-devel-windows-amd64-0.17.0/include"

RESOURCES +=



