QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

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
    plainshader.geom \
    plainshader.vert \
    textureshader.frag
