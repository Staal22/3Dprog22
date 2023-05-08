QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17 geometryshader

TARGET      = 3D-programmering

SOURCES += main.cpp \
    camera.cpp \
    curve.cpp \
    disc.cpp \
    enemy.cpp \
    graphfunction.cpp \
    house.cpp \
    interactiveobject.cpp \
    lightsource.cpp \
    lightswitch.cpp \
    linesurface.cpp \
    logger.cpp \
    mainwindow.cpp \
    objectgroup.cpp \
    objectloader.cpp \
    octahedronball.cpp \
    parabolaapproximation.cpp \
    player.cpp \
    polyinterpolation.cpp \
    renderwindow.cpp \
    shader.cpp \
    tetrahedron.cpp \
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
    enemy.h \
    graphfunction.h \
    house.h \
    interactiveobject.h \
    lightsource.h \
    lightswitch.h \
    linesurface.h \
    logger.h \
    mainwindow.h \
    objectgroup.h \
    objectloader.h \
    octahedronball.h \
    parabolaapproximation.h \
    player.h \
    polyinterpolation.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    tetrahedron.h \
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

RESOURCES +=



