QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    interactiveobject.cpp \
    logger.cpp \
    mainwindow.cpp \
    renderwindow.cpp \
    shader.cpp \
    triangle.cpp \
    trianglesurface.cpp \
    twovariablefunctionspace.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    interactiveobject.h \
    logger.h \
    mainwindow.h \
    renderwindow.h \
    shader.h \
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
