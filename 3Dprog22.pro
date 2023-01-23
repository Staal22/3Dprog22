QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    curve.cpp \
    interactiveobject.cpp \
    linesurface.cpp \
    logger.cpp \
    mainwindow.cpp \
    renderwindow.cpp \
    shader.cpp \
    tetrahedron.cpp \
    triangle.cpp \
    trianglesurface.cpp \
    twovariablefunctionspace.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    curve.h \
    interactiveobject.h \
    linesurface.h \
    logger.h \
    mainwindow.h \
    renderwindow.h \
    shader.h \
    tetrahedron.h \
    triangle.h \
    trianglesurface.h \
    twovariablefunctionspace.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainshader.frag \
    plainshader.vert
